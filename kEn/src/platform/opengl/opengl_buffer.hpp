#pragma once

#include <glad/gl.h>

#include <cstddef>
#include <cstdint>
#include <memory>

#include <kEn/renderer/buffer.hpp>
#include <kEn/util/enum_map.hpp>

namespace kEn {

namespace buffer_target {

namespace detail {

inline constexpr auto kOpenglTypes = util::make_enum_map<GLenum>({
    std::pair{Vertex, GL_ARRAY_BUFFER},
    std::pair{Index, GL_ELEMENT_ARRAY_BUFFER},
    std::pair{Uniform, GL_UNIFORM_BUFFER},
    std::pair{Storage, GL_SHADER_STORAGE_BUFFER},
});

}  // namespace detail

/** @brief Return the GL binding target (e.g. GL_ARRAY_BUFFER) for a BufferTarget. */
[[nodiscard]] constexpr GLenum opengl_target(BufferTarget type) { return detail::kOpenglTypes[type]; }

}  // namespace buffer_target

namespace buffer_usage {

namespace detail {

inline constexpr auto kOpenglUsages = util::make_enum_map<GLenum>({
    std::pair{Immutable, GL_STATIC_DRAW},
    std::pair{Default, GL_STATIC_DRAW},
    std::pair{Dynamic, GL_DYNAMIC_DRAW},
    std::pair{Staging, GL_DYNAMIC_READ},
});

}  // namespace detail

/** @brief Return the GL usage hint (e.g. GL_DYNAMIC_DRAW) for a BufferUsage. */
[[nodiscard]] constexpr GLenum opengl_usage(BufferUsage usage) { return detail::kOpenglUsages[usage]; }

}  // namespace buffer_usage

namespace map_mode {

namespace detail {

inline constexpr auto kOpenglModes = util::make_enum_map<GLbitfield>({
    std::pair{Read, GL_MAP_READ_BIT},
    std::pair{Write, GL_MAP_WRITE_BIT},
    std::pair{ReadWrite, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT},
    std::pair{WriteDiscard, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT},
    std::pair{WriteNoOverwrite, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT},
});

}  // namespace detail

/** @brief Return the GL access bitfield for a MapMode (for use with glMapNamedBufferRange). */
[[nodiscard]] constexpr GLenum opengl_mode(MapMode mode) { return detail::kOpenglModes[mode]; }

}  // namespace map_mode

/**
 * @brief OpenGL implementation of Buffer.
 *
 * Wraps a single named buffer object (glCreateBuffers / glDeleteBuffers).
 * Move-only; copy is deleted. Exposed to OpenglMutableBuffer via friendship
 * for internal resize and allocation operations.
 */
class OpenglBuffer final : public Buffer {
 public:
  explicit OpenglBuffer(const BufferDesc& desc, const void* data = nullptr);
  OpenglBuffer(const void* data, std::size_t size, BufferUsage usage = BufferUsage::Default,
               BufferBinds bind_flags = BufferBinds{});
  ~OpenglBuffer() override;

  [[nodiscard]] const BufferDesc& desc() const override { return desc_; }
  [[nodiscard]] std::uintptr_t native_handle() const noexcept override {
    return static_cast<std::uintptr_t>(renderer_id_);
  }
  [[nodiscard]] std::uint32_t renderer_id() const { return renderer_id_; }

  void bind(BufferTarget target) const;
  void unbind(BufferTarget target) const;

  OpenglBuffer(const OpenglBuffer&)            = delete;
  OpenglBuffer& operator=(const OpenglBuffer&) = delete;
  OpenglBuffer(OpenglBuffer&& other) noexcept;
  OpenglBuffer& operator=(OpenglBuffer&& other) noexcept;

 private:
  void allocate(const void* data) const;
  void set_size(std::size_t size) { desc_.size = size; }

  BufferDesc desc_{};
  std::uint32_t renderer_id_{};

  friend class OpenglMutableBuffer;
};

/**
 * @brief OpenGL implementation of MutableBuffer.
 *
 * Owns a shared OpenglBuffer and provides set_data(), update_data(), resize(),
 * and map() operations. Immutable buffers are rejected at construction.
 *
 * Mapping is permitted for Dynamic (write modes) and Staging (all modes) usages;
 * invalid combinations trigger an assertion via can_map().
 */
class OpenglMutableBuffer final : public MutableBuffer {
 public:
  explicit OpenglMutableBuffer(std::shared_ptr<OpenglBuffer> buffer);
  explicit OpenglMutableBuffer(const BufferDesc& desc, const void* data = nullptr);
  OpenglMutableBuffer(const void* data, std::size_t size, BufferUsage usage = BufferUsage::Dynamic,
                      BufferBinds bind_flags = BufferBinds{});

  [[nodiscard]] MappedBuffer map(MapMode mode) const override;
  void set_data(const void* data, std::size_t size) override;
  void update_data(std::size_t offset, const void* data, std::size_t size) override;
  void resize(std::size_t size, const void* data) override;
  [[nodiscard]] std::shared_ptr<Buffer> underlying_buffer() const override { return buffer_; }

 private:
  [[nodiscard]] bool can_map(MapMode mode) const;

  std::shared_ptr<OpenglBuffer> buffer_;
};

class OpenglUniformBuffer final : public UniformBuffer {
 public:
  explicit OpenglUniformBuffer(std::shared_ptr<OpenglBuffer> buffer);

  [[nodiscard]] std::shared_ptr<Buffer> underlying_buffer() const override {
    return std::static_pointer_cast<Buffer>(buffer_);
  }

 private:
  std::shared_ptr<OpenglBuffer> buffer_;
};

class OpenglShaderStorageBuffer final : public ShaderStorageBuffer {
 public:
  explicit OpenglShaderStorageBuffer(std::shared_ptr<OpenglBuffer> buffer);

  [[nodiscard]] std::shared_ptr<Buffer> underlying_buffer() const override {
    return std::static_pointer_cast<Buffer>(buffer_);
  }

 private:
  std::shared_ptr<OpenglBuffer> buffer_;
};

}  // namespace kEn