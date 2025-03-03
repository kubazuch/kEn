#include <glad/gl.h>

#include <kenpch.hpp>
#include <platform/opengl/opengl_vertex_array.hpp>

namespace kEn {

static GLenum get_opengl_type(shader_data_type type) {
  switch (type) {
    case shader_data_types::float_:
    case shader_data_types::float2:
    case shader_data_types::float3:
    case shader_data_types::float4:
    case shader_data_types::mat3:
    case shader_data_types::mat4:
      return GL_FLOAT;
    case shader_data_types::int_:
    case shader_data_types::int2:
    case shader_data_types::int3:
    case shader_data_types::int4:
      return GL_INT;
    case shader_data_types::bool_:
      return GL_BOOL;
    default:
      KEN_CORE_ASSERT(false, "Unknown shader data type!");
      return 0;
  }
}

OpenglVertexArray::OpenglVertexArray() : renderer_id_(0) { glGenVertexArrays(1, &renderer_id_); }

OpenglVertexArray::~OpenglVertexArray() { glDeleteVertexArrays(1, &renderer_id_); }

void OpenglVertexArray::bind() const { glBindVertexArray(renderer_id_); }

void OpenglVertexArray::unbind() const { glBindVertexArray(0); }

void OpenglVertexArray::add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vertex_buf) {
  KEN_CORE_ASSERT(!vertex_buf->layout().elements().empty(), "Vertex buffer must have layout!");

  glBindVertexArray(renderer_id_);
  vertex_buf->bind();

  const auto& layout = vertex_buf->layout();
  for (const auto& element : layout) {
    switch (element.type) {
      case shader_data_types::float_:
      case shader_data_types::float2:
      case shader_data_types::float3:
      case shader_data_types::float4: {
        glEnableVertexAttribArray(vertex_buffer_index_);
        glVertexAttribPointer(vertex_buffer_index_,
                              static_cast<GLint>(shader_data_types::get_component_count(element.type)),
                              get_opengl_type(element.type), element.normalized ? GL_TRUE : GL_FALSE,
                              static_cast<GLsizei>(layout.stride()), (const void*)element.offset);  // NOLINT
        vertex_buffer_index_++;
        break;
      }

      case shader_data_types::int_:
      case shader_data_types::int2:
      case shader_data_types::int3:
      case shader_data_types::int4:
      case shader_data_types::bool_: {
        glEnableVertexAttribArray(vertex_buffer_index_);
        glVertexAttribIPointer(vertex_buffer_index_,
                               static_cast<GLint>(shader_data_types::get_component_count(element.type)),
                               get_opengl_type(element.type), static_cast<GLsizei>(layout.stride()),
                               (const void*)element.offset);  // NOLINT
        vertex_buffer_index_++;
        break;
      }

      case shader_data_types::mat3:
      case shader_data_types::mat4: {
        uint8_t count = shader_data_types::get_component_count(element.type);
        for (uint8_t i = 0; i < count; ++i) {
          glEnableVertexAttribArray(vertex_buffer_index_);
          glVertexAttribPointer(
              vertex_buffer_index_, count, get_opengl_type(element.type), element.normalized ? GL_TRUE : GL_FALSE,
              static_cast<GLsizei>(layout.stride()),
              (const void*)(element.offset + get_size(shader_data_types::float_) * count * i));  // NOLINT
          glVertexAttribDivisor(vertex_buffer_index_, 1);
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

void OpenglVertexArray::set_index_buffer(const std::shared_ptr<kEn::IndexBuffer>& index_buf) {
  glBindVertexArray(renderer_id_);
  index_buf->bind();

  index_buffer_ = index_buf;
}

}  // namespace kEn
