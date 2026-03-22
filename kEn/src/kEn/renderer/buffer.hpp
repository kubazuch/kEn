#pragma once

#include <functional>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include <kEn/core/assert.hpp>
#include <kEn/core/core.hpp>
#include <kEn/util/enum_map.hpp>

namespace kEn {

enum class ShaderDataType : uint8_t { Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool, Count };

namespace shader_data_type {

using enum ShaderDataType;

inline constexpr std::uint32_t kFloatSize = 4;
inline constexpr std::uint32_t kIntSize   = 4;
inline constexpr std::uint32_t kBoolSize  = 1;

inline constexpr util::EnumMap kSizes{{
    std::pair{Float, 1 * kFloatSize},
    std::pair{Float2, 2 * kFloatSize},
    std::pair{Float3, 3 * kFloatSize},
    std::pair{Float4, 4 * kFloatSize},
    std::pair{Mat3, 9 * kFloatSize},
    std::pair{Mat4, 16 * kFloatSize},
    std::pair{Int, 1 * kIntSize},
    std::pair{Int2, 2 * kIntSize},
    std::pair{Int3, 3 * kIntSize},
    std::pair{Int4, 4 * kIntSize},
    std::pair{Bool, kBoolSize},
}};

inline constexpr auto kComponents = util::make_enum_map<std::uint8_t>({
    std::pair{Float, 1},
    std::pair{Float2, 2},
    std::pair{Float3, 3},
    std::pair{Float4, 4},
    std::pair{Mat3, 3},
    std::pair{Mat4, 4},
    std::pair{Int, 1},
    std::pair{Int2, 2},
    std::pair{Int3, 3},
    std::pair{Int4, 4},
    std::pair{Bool, 1},
});

[[nodiscard]] constexpr std::uint32_t get_size(ShaderDataType type) { return kSizes[type]; }

[[nodiscard]] constexpr std::uint8_t get_component_count(ShaderDataType type) { return kComponents[type]; }

}  // namespace shader_data_type

enum class BufferType : std::uint8_t { Vertex, Index, Uniform, ShaderStorage };

namespace buffer_type {

using enum BufferType;

}  // namespace buffer_type

struct BufferElement {
  std::string_view name;
  ShaderDataType type{};
  std::uint32_t size{};
  size_t offset{};
  bool normalized{};

  constexpr BufferElement(ShaderDataType type, std::string_view name, bool normalized = false)
      : name(name), type(type), size(shader_data_type::get_size(type)), normalized(normalized) {}
};

struct BufferLayout {
  BufferLayout() = default;

  BufferLayout(std::initializer_list<BufferElement> elements) noexcept : elements_(elements) {
    size_t offset = 0;
    for (auto& element : elements_) {
      element.offset = offset;
      offset += element.size;
    }

    stride_ = offset;
  }

  size_t stride() const { return stride_; }
  const std::vector<BufferElement>& elements() const { return elements_; }

  std::vector<BufferElement>::iterator begin() { return elements_.begin(); }
  std::vector<BufferElement>::iterator end() { return elements_.end(); }
  std::vector<BufferElement>::const_iterator begin() const { return elements_.begin(); }
  std::vector<BufferElement>::const_iterator end() const { return elements_.end(); }

 private:
  std::vector<BufferElement> elements_;
  size_t stride_ = 0;
};

struct Buffer {
  virtual ~Buffer() = default;

  virtual void bind(BufferType) const   = 0;
  virtual void unbind(BufferType) const = 0;

  virtual const BufferLayout& layout() const          = 0;
  virtual void set_layout(const BufferLayout& layout) = 0;
  virtual size_t size() const                         = 0;
};

struct MutableBuffer : virtual public Buffer {
  virtual void modify_data(std::function<void(void*)> fn) const = 0;
  virtual void set_data(const void* data, size_t size)          = 0;
};

struct UniformBuffer {
  virtual ~UniformBuffer() = default;

  virtual void bind() const   = 0;
  virtual void unbind() const = 0;

  virtual std::shared_ptr<Buffer> underlying_buffer() const = 0;
  virtual size_t binding_point() const                      = 0;
};

struct ShaderStorageBuffer {
  virtual ~ShaderStorageBuffer() = default;

  virtual void bind() const   = 0;
  virtual void unbind() const = 0;

  virtual std::shared_ptr<Buffer> underlying_buffer() const = 0;
  virtual size_t binding_point() const                      = 0;
};

}  // namespace kEn