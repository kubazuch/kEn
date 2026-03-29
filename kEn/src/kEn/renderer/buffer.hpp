#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <span>

#include <kEn/core/core.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/util/flags.hpp>

namespace kEn {

/** GPU buffer binding target (vertex, index, uniform, shader storage). */
enum class BufferTarget : std::uint8_t { Vertex, Index, Uniform, Storage };

/** Convenience using-declarations for BufferTarget enumerators. */
namespace buffer_target {

using enum BufferTarget;

}  // namespace buffer_target

/**
 * Bitmask of pipeline stages a buffer may be bound to.
 *
 * Multiple flags may be combined; e.g. Vertex | Uniform allows the same
 * allocation to serve as both a VBO and a UBO slot.
 */
enum class BufferBind : std::uint8_t { Vertex = 1U << 0, Index = 1U << 1, Uniform = 1U << 2, Storage = 1U << 3 };

KEN_ENABLE_FLAGS(::kEn::BufferBind);

/** Bitmask type combining one or more BufferBind flags. */
using BufferBinds = util::Flags<BufferBind>;

/** Convenience using-declarations for BufferBind enumerators. */
namespace buffer_bind {

using enum BufferBind;

}  // namespace buffer_bind

/**
 * CPU-to-GPU data-flow hint, analogous to D3D11_USAGE.
 *
 * - Immutable: written once at creation; no further CPU access.
 * - Default:   written once; no CPU mapping (GL_STATIC_DRAW).
 * - Dynamic:   updated frequently; write-only CPU mapping allowed.
 * - Staging:   CPU read-back target; full read/write mapping allowed.
 */
enum class BufferUsage : std::uint8_t { Immutable, Default, Dynamic, Staging };

/** Convenience using-declarations for BufferUsage enumerators. */
namespace buffer_usage {

using enum BufferUsage;

}  // namespace buffer_usage

/**
 * CPU mapping access mode for MutableBuffer::map().
 *
 * - Read:             read-only CPU access.
 * - Write:            write-only; existing GPU content is preserved.
 * - ReadWrite:        full read/write access.
 * - WriteDiscard:     write-only; previous content is discarded (preferred for streaming).
 * - WriteNoOverwrite: write-only; caller guarantees no overlap with in-flight GPU reads.
 *
 * Validity depends on BufferUsage; see MutableBuffer::map().
 */
enum class MapMode : std::uint8_t { Read, Write, ReadWrite, WriteDiscard, WriteNoOverwrite };

/** Convenience using-declarations for MapMode enumerators. */
namespace map_mode {

using enum MapMode;

}  // namespace map_mode

/** Descriptor passed to Device::create_buffer() / Device::create_mutable_buffer(). */
struct BufferDesc {
  std::size_t size{};                      /**< Total size in bytes. */
  BufferUsage usage{BufferUsage::Default}; /**< CPU/GPU access pattern hint. */
  BufferBinds bind_flags;                  /**< Which pipeline stages may bind this buffer. */
  std::size_t stride{};                    /**< Element stride in bytes; 0 if not applicable. */
};

/**
 * Opaque GPU buffer handle.
 *
 * Holds an immutable descriptor and a convenience size() accessor.
 * Mutable CPU-side access is provided by MutableBuffer; per-slot binding
 * views by UniformBuffer and ShaderStorageBuffer.
 */
struct Buffer {
  virtual ~Buffer() = default;

  /** Return the descriptor this buffer was created with. */
  [[nodiscard]] virtual const BufferDesc& desc() const = 0;
  /** Return the buffer size in bytes (shorthand for desc().size). */
  [[nodiscard]] std::size_t size() const { return desc().size; }
  /** Return the platform-native GPU buffer handle. */
  [[nodiscard]] virtual std::uintptr_t native_handle() const noexcept = 0;
};

/**
 * RAII handle for a CPU-mapped GPU buffer region.
 *
 * Constructed by MutableBuffer::map(). The mapped region is automatically
 * unmapped on destruction or on an explicit reset() call. Move-only.
 */
class MappedBuffer {
 public:
  MappedBuffer() = default;
  MappedBuffer(std::span<std::byte> data, std::function<void()> unmap) : data_(data), unmap_(std::move(unmap)) {}
  ~MappedBuffer() { reset(); }

  MappedBuffer(const MappedBuffer&)            = delete;
  MappedBuffer& operator=(const MappedBuffer&) = delete;
  MappedBuffer(MappedBuffer&& other) noexcept : data_(other.data_), unmap_(std::move(other.unmap_)) {
    other.unmap_ = nullptr;
  }
  MappedBuffer& operator=(MappedBuffer&& other) noexcept {
    if (this == &other) {
      return *this;
    }

    reset();
    data_        = other.data_;
    unmap_       = std::move(other.unmap_);
    other.unmap_ = nullptr;
    other.data_  = {};
    return *this;
  }

  void reset() {
    if (unmap_) {
      unmap_();
      unmap_ = nullptr;
      data_  = {};
    }
  }

  [[nodiscard]] std::span<std::byte> data() const { return data_; }
  [[nodiscard]] explicit operator bool() const { return unmap_ != nullptr; }

 private:
  std::span<std::byte> data_;
  std::function<void()> unmap_;
};

/**
 * Write interface for a GPU buffer that supports CPU updates.
 *
 * Does not inherit Buffer; use underlying_buffer() to obtain the raw allocation.
 * Implementations must reject Immutable buffers at construction time.
 */
struct MutableBuffer {
  virtual ~MutableBuffer() = default;

  /** Replace the entire buffer contents; reallocates if size differs. */
  virtual void set_data(const void* data, std::size_t size) = 0;
  /** Write size bytes at the given byte offset without reallocation. */
  virtual void update_data(std::size_t offset, const void* data, std::size_t size) = 0;
  /** Reallocate to the given size; data may be null to leave contents undefined. */
  virtual void resize(std::size_t size, const void* data) = 0;

  /** Reallocate to size, leaving contents undefined. */
  void resize(std::size_t size) { resize(size, nullptr); }

  /** Map with WriteDiscard mode (convenience overload). */
  [[nodiscard]] MappedBuffer map() const { return map(MapMode::WriteDiscard); }
  /**
   * Map the buffer for CPU access with the given mode.
   *
   * The validity of mode depends on the underlying BufferUsage;
   * an assertion fires for incompatible combinations.
   */
  [[nodiscard]] virtual MappedBuffer map(MapMode mode) const = 0;

  /** Return the underlying GPU buffer allocation. */
  [[nodiscard]] virtual std::shared_ptr<Buffer> underlying_buffer() const = 0;
};

/** Binding view over a GPU buffer used as a uniform buffer object. */
struct UniformBuffer {
  virtual ~UniformBuffer() = default;

  /** Return the underlying GPU buffer allocation. */
  [[nodiscard]] virtual std::shared_ptr<Buffer> underlying_buffer() const = 0;
  /** Return the size of the underlying buffer in bytes. */
  [[nodiscard]] std::size_t size() const { return underlying_buffer()->size(); }
};

/** Binding view over a GPU buffer used as a shader storage buffer. */
struct ShaderStorageBuffer {
  virtual ~ShaderStorageBuffer() = default;

  /** Return the underlying GPU buffer allocation. */
  [[nodiscard]] virtual std::shared_ptr<Buffer> underlying_buffer() const = 0;
  /** Return the size of the underlying buffer in bytes. */
  [[nodiscard]] std::size_t size() const { return underlying_buffer()->size(); }
};

}  // namespace kEn
