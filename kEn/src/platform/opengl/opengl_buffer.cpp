#include <glad/gl.h>

#include <kenpch.hpp>
#include <platform/opengl/opengl_buffer.hpp>
#include <utility>

namespace kEn {

constexpr GLenum get_buffer_type(BufferType type) {
  switch (type) {
    case BufferType::Vertex:
      return GL_ARRAY_BUFFER;
    case BufferType::Index:
      return GL_ELEMENT_ARRAY_BUFFER;
    case BufferType::Uniform:
      return GL_UNIFORM_BUFFER;
    default:
      KEN_CORE_ASSERT(false, "Unknown buffer type!");
      return 0;
  }
}

OpenglBuffer::OpenglBuffer(const void* data, size_t size) : size_(size), renderer_id_(0) {
  glCreateBuffers(1, &renderer_id_);
  set_data_int(data, size);
}

OpenglBuffer::~OpenglBuffer() { glDeleteBuffers(1, &renderer_id_); }

void OpenglBuffer::bind(BufferType type) const { glBindBuffer(get_buffer_type(type), renderer_id_); }

void OpenglBuffer::unbind(BufferType type) const { glBindBuffer(get_buffer_type(type), 0); }

void OpenglBuffer::set_data_int(const void* data, size_t size) const {
  glNamedBufferData(renderer_id_, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);
}

// ---------------- //

void OpenglMutableBuffer::modify_data(std::function<void(void*)> fn) const {
  void* ptr = glMapNamedBuffer(renderer_id_, GL_WRITE_ONLY);
  fn(ptr);
  glUnmapNamedBuffer(renderer_id_);
}

void OpenglMutableBuffer::set_data(const void* data, size_t size) {
  size_ = size;
  set_data_int(data, size);
}

void OpenglMutableBuffer::set_data_int(const void* data, size_t size) const {
  glNamedBufferData(renderer_id_, static_cast<GLsizeiptr>(size), data, GL_DYNAMIC_DRAW);
}

// ---------------- //

OpenglUniformBuffer::OpenglUniformBuffer(std::shared_ptr<OpenglBuffer> buffer, size_t binding_point)
    : buffer_(std::move(buffer)), binding_point_(binding_point) {
  glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(binding_point), buffer->renderer_id_);
}

}  // namespace kEn
