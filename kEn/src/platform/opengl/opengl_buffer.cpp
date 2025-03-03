#include <glad/gl.h>

#include <kenpch.hpp>
#include <platform/opengl/opengl_buffer.hpp>

namespace kEn {
/*
 *		VERTEX BUFFER
 */

OpenglVertexBuffer::OpenglVertexBuffer(void* vertices, uint32_t size) : renderer_id_(0) {
  glCreateBuffers(1, &renderer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenglVertexBuffer::~OpenglVertexBuffer() { glDeleteBuffers(1, &renderer_id_); }

void OpenglVertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, renderer_id_); }

void OpenglVertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

/*
 *		MUTABLE VERTEX BUFFER
 */

OpenglMutableVertexBuffer::OpenglMutableVertexBuffer(void* vertices, uint32_t size) : renderer_id_(0) {
  glCreateBuffers(1, &renderer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

OpenglMutableVertexBuffer::~OpenglMutableVertexBuffer() { glDeleteBuffers(1, &renderer_id_); }

void OpenglMutableVertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, renderer_id_); }

void OpenglMutableVertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void OpenglMutableVertexBuffer::modify_data(std::function<void(void*)> fn) const {
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
  void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  fn(ptr);
  glUnmapBuffer(GL_ARRAY_BUFFER);
}

/*
 *		INDEX BUFFER
 */

OpenglIndexBuffer::OpenglIndexBuffer(uint32_t* indices, uint32_t count) : renderer_id_(0), count_(count) {
  glGenBuffers(1, &renderer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(uint32_t)), indices, GL_STATIC_DRAW);
}

OpenglIndexBuffer::~OpenglIndexBuffer() { glDeleteBuffers(1, &renderer_id_); }

void OpenglIndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_); }

void OpenglIndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

}  // namespace kEn
