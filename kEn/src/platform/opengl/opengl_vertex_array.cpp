#include "opengl_vertex_array.hpp"

#include <glad/gl.h>

#include <cstddef>
#include <cstdint>
#include <memory>

#include <kEn/core/assert.hpp>
#include <kEn/renderer/buffer.hpp>

#include "opengl_buffer.hpp"

namespace kEn {

OpenglVertexArray::OpenglVertexArray() { glGenVertexArrays(1, &renderer_id_); }

OpenglVertexArray::~OpenglVertexArray() { glDeleteVertexArrays(1, &renderer_id_); }

void OpenglVertexArray::bind() const { glBindVertexArray(renderer_id_); }

void OpenglVertexArray::unbind() const { glBindVertexArray(0); }

void OpenglVertexArray::add_vertex_buffer(const std::shared_ptr<Buffer>& vertex_buf) {
  add_vertex_buffer(vertex_buf, 0);
}

void OpenglVertexArray::add_vertex_buffer(const std::shared_ptr<Buffer>& vertex_buf, size_t divisor) {
  KEN_CORE_ASSERT(!vertex_buf->layout().elements().empty(), "Vertex buffer must have layout!");

  glBindVertexArray(renderer_id_);
  vertex_buf->bind(buffer_type::Vertex);

  const auto& layout = vertex_buf->layout();
  for (const auto& element : layout) {
    switch (element.type) {
      case shader_data_type::Float:
      case shader_data_type::Float2:
      case shader_data_type::Float3:
      case shader_data_type::Float4: {
        glEnableVertexAttribArray(vertex_buffer_index_);
        glVertexAttribPointer(vertex_buffer_index_,
                              static_cast<GLint>(shader_data_type::get_component_count(element.type)),
                              shader_data_type::get_opengl_type(element.type),
                              static_cast<GLboolean>(element.normalized ? GL_TRUE : GL_FALSE),
                              static_cast<GLsizei>(layout.stride()), (const void*)element.offset);  // NOLINT
        glVertexAttribDivisor(vertex_buffer_index_, static_cast<GLuint>(divisor));
        vertex_buffer_index_++;
        break;
      }

      case shader_data_type::Int:
      case shader_data_type::Int2:
      case shader_data_type::Int3:
      case shader_data_type::Int4:
      case shader_data_type::Bool: {
        glEnableVertexAttribArray(vertex_buffer_index_);
        glVertexAttribIPointer(vertex_buffer_index_,
                               static_cast<GLint>(shader_data_type::get_component_count(element.type)),
                               shader_data_type::get_opengl_type(element.type), static_cast<GLsizei>(layout.stride()),
                               (const void*)element.offset);  // NOLINT
        glVertexAttribDivisor(vertex_buffer_index_, static_cast<GLuint>(divisor));
        vertex_buffer_index_++;
        break;
      }

      case shader_data_type::Mat3:
      case shader_data_type::Mat4: {
        const uint8_t count = shader_data_type::get_component_count(element.type);
        for (uint8_t i = 0; i < count; ++i) {
          glEnableVertexAttribArray(vertex_buffer_index_);
          glVertexAttribPointer(
              vertex_buffer_index_, count, shader_data_type::get_opengl_type(element.type),
              static_cast<GLboolean>(element.normalized ? GL_TRUE : GL_FALSE), static_cast<GLsizei>(layout.stride()),
              // NOLINTNEXTLINE
              (const void*)(element.offset + (shader_data_type::get_size(shader_data_type::Float) * count * i)));
          glVertexAttribDivisor(vertex_buffer_index_, static_cast<GLuint>(divisor));
          vertex_buffer_index_++;
        }
        break;
      }
      default:
        KEN_CORE_ASSERT(false, "Unknown shader data type!");
    }
  }

  vertex_buffers_.push_back(vertex_buf);
}

void OpenglVertexArray::set_index_buffer(const std::shared_ptr<kEn::Buffer>& index_buf) {
  glBindVertexArray(renderer_id_);
  index_buf->bind(buffer_type::Index);

  index_buffer_ = index_buf;
}

size_t OpenglVertexArray::element_count() const {
  if (index_buffer_) {
    return index_buffer_->size() / sizeof(uint32_t);
  }

  // If no index buffer is set, return the number of vertices in the first vertex buffer.
  if (!vertex_buffers_.empty()) {
    auto& vertex_buf = *vertex_buffers_[0];
    return vertex_buf.size() / vertex_buf.layout().stride();
  }

  KEN_CORE_ASSERT(false, "No index buffer or vertex buffer found!");
  return 0;
}

}  // namespace kEn
