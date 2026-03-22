#pragma once

#include <functional>
#include <memory>

#include <kEn/core/assert.hpp>
#include <kEn/core/core.hpp>

namespace kEn {

enum class BufferType : std::uint8_t { Vertex, Index, Uniform, ShaderStorage };

namespace buffer_type {

using enum BufferType;

}  // namespace buffer_type

struct Buffer {
  virtual ~Buffer() = default;

  virtual void bind(BufferType) const   = 0;
  virtual void unbind(BufferType) const = 0;

  [[nodiscard]] virtual size_t size() const = 0;
};

struct MutableBuffer : virtual public Buffer {
  virtual void modify_data(std::function<void(void*)> fn) const = 0;
  virtual void set_data(const void* data, size_t size)          = 0;
};

struct UniformBuffer {
  virtual ~UniformBuffer() = default;

  virtual void bind() const   = 0;
  virtual void unbind() const = 0;

  [[nodiscard]] virtual std::shared_ptr<Buffer> underlying_buffer() const = 0;
  [[nodiscard]] virtual size_t binding_point() const                      = 0;
};

struct ShaderStorageBuffer {
  virtual ~ShaderStorageBuffer() = default;

  virtual void bind() const   = 0;
  virtual void unbind() const = 0;

  [[nodiscard]] virtual std::shared_ptr<Buffer> underlying_buffer() const = 0;
  [[nodiscard]] virtual size_t binding_point() const                      = 0;
};

}  // namespace kEn