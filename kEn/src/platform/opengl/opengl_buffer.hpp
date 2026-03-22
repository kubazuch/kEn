#pragma once

#include <glad/gl.h>

#include <memory>

#include <kEn/renderer/buffer.hpp>
#include <kEn/util/enum_map.hpp>

namespace kEn {

namespace buffer_type {

inline constexpr auto kOpenglTypes = util::make_enum_map<GLenum>({
    std::pair{Vertex, GL_ARRAY_BUFFER},
    std::pair{Index, GL_ELEMENT_ARRAY_BUFFER},
    std::pair{Uniform, GL_UNIFORM_BUFFER},
    std::pair{ShaderStorage, GL_SHADER_STORAGE_BUFFER},
});

[[nodiscard]] constexpr GLenum get_opengl_type(BufferType type) { return kOpenglTypes[type]; }

}  // namespace buffer_type

class OpenglBuffer : virtual public Buffer {
 public:
  OpenglBuffer(const void* data, size_t size);
  ~OpenglBuffer() override;

  void bind(BufferType type) const override;
  void unbind(BufferType type) const override;

  size_t size() const override { return size_; }

 protected:
  virtual void set_data_int(const void* data, size_t size) const;

  size_t size_;
  uint32_t renderer_id_{};

  friend class OpenglUniformBuffer;
  friend class OpenglShaderStorageBuffer;
};

class OpenglMutableBuffer final : public MutableBuffer, public OpenglBuffer {
 public:
  OpenglMutableBuffer(const void* data, size_t size) : OpenglBuffer(data, size) {}

  void bind(BufferType type) const override { OpenglBuffer::bind(type); }
  void unbind(BufferType type) const override { OpenglBuffer::unbind(type); }
  size_t size() const override { return OpenglBuffer::size(); }

  void modify_data(std::function<void(void*)> fn) const override;
  void set_data(const void* data, size_t size) override;

 protected:
  void set_data_int(const void* data, size_t size) const override;
};

class OpenglUniformBuffer final : public UniformBuffer {
 public:
  OpenglUniformBuffer(std::shared_ptr<OpenglBuffer> buffer, size_t binding_point);

  void bind() const override { buffer_->bind(buffer_type::Uniform); }
  void unbind() const override { buffer_->unbind(buffer_type::Uniform); }

  std::shared_ptr<Buffer> underlying_buffer() const override { return std::static_pointer_cast<Buffer>(buffer_); }
  size_t binding_point() const override { return binding_point_; }

 private:
  std::shared_ptr<OpenglBuffer> buffer_;
  size_t binding_point_;
};

class OpenglShaderStorageBuffer final : public ShaderStorageBuffer {
 public:
  OpenglShaderStorageBuffer(std::shared_ptr<OpenglBuffer> buffer, size_t binding_point);

  void bind() const override { buffer_->bind(buffer_type::ShaderStorage); }
  void unbind() const override { buffer_->unbind(buffer_type::ShaderStorage); }

  std::shared_ptr<Buffer> underlying_buffer() const override { return std::static_pointer_cast<Buffer>(buffer_); }
  size_t binding_point() const override { return binding_point_; }

 private:
  std::shared_ptr<OpenglBuffer> buffer_;
  size_t binding_point_;
};

}  // namespace kEn