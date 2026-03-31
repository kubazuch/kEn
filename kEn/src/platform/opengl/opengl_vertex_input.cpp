#include "opengl_vertex_input.hpp"

#include <glad/gl.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <utility>

#include <kEn/core/assert.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/shader_data_type.hpp>
#include <kEn/renderer/vertex_input.hpp>

#include "opengl_buffer.hpp"
#include "opengl_shader_data_type.hpp"

namespace kEn {

OpenglVertexInput::OpenglVertexInput() { glGenVertexArrays(1, &renderer_id_); }

OpenglVertexInput::~OpenglVertexInput() { glDeleteVertexArrays(1, &renderer_id_); }

void OpenglVertexInput::add_vertex_stream(const VertexStreamBinding& stream) {
  KEN_CORE_ASSERT(!stream.layout.empty(), "Vertex stream must have a layout!");

  glBindVertexArray(renderer_id_);
  std::dynamic_pointer_cast<OpenglBuffer>(stream.buffer)->bind(buffer_target::Vertex);

  const auto& layout   = stream.layout;
  const GLuint divisor = stream.input_rate == VertexInputRate::PerInstance ? stream.instance_step_rate : 0;

  for (const auto& element : layout) {
    const std::size_t base_offset = stream.buffer_offset + element.offset;

    switch (element.type) {
      case shader_data_type::Float:
      case shader_data_type::Float2:
      case shader_data_type::Float3:
      case shader_data_type::Float4: {
        glEnableVertexAttribArray(attrib_index_);
        glVertexAttribPointer(attrib_index_, static_cast<GLint>(shader_data_type::component_count(element.type)),
                              shader_data_type::opengl_type(element.type),
                              static_cast<GLboolean>(element.normalized ? GL_TRUE : GL_FALSE),
                              static_cast<GLsizei>(layout.stride),
                              reinterpret_cast<const void*>(base_offset));  // NOLINT
        glVertexAttribDivisor(attrib_index_, divisor);
        attrib_index_++;
        break;
      }

      case shader_data_type::Int:
      case shader_data_type::Int2:
      case shader_data_type::Int3:
      case shader_data_type::Int4:
      case shader_data_type::Bool: {
        glEnableVertexAttribArray(attrib_index_);
        glVertexAttribIPointer(attrib_index_, static_cast<GLint>(shader_data_type::component_count(element.type)),
                               shader_data_type::opengl_type(element.type), static_cast<GLsizei>(layout.stride),
                               reinterpret_cast<const void*>(base_offset));  // NOLINT
        glVertexAttribDivisor(attrib_index_, divisor);
        attrib_index_++;
        break;
      }

      case shader_data_type::Mat3:
      case shader_data_type::Mat4: {
        const uint8_t count = shader_data_type::component_count(element.type);
        for (uint8_t i = 0; i < count; ++i) {
          const std::size_t col_offset = base_offset + (shader_data_type::size(shader_data_type::Float) * count * i);
          glEnableVertexAttribArray(attrib_index_);
          glVertexAttribPointer(attrib_index_, count, shader_data_type::opengl_type(element.type),
                                static_cast<GLboolean>(element.normalized ? GL_TRUE : GL_FALSE),
                                static_cast<GLsizei>(layout.stride),
                                reinterpret_cast<const void*>(col_offset));  // NOLINT
          glVertexAttribDivisor(attrib_index_, divisor);
          attrib_index_++;
        }
        break;
      }

      default:
        KEN_CORE_ASSERT(false, "Unknown shader data type!");
    }
  }

  streams_.push_back(stream);
}

void OpenglVertexInput::set_index_buffer_impl(std::shared_ptr<Buffer> index_buf, IndexType index_type,
                                              std::size_t index_offset) {
  glBindVertexArray(renderer_id_);
  std::dynamic_pointer_cast<OpenglBuffer>(index_buf)->bind(buffer_target::Index);

  index_buffer_        = std::move(index_buf);
  index_type_          = index_type;
  index_buffer_offset_ = index_offset;
}

std::size_t OpenglVertexInput::element_count() const {
  if (index_buffer_) {
    const std::size_t index_size = index_type_ == IndexType::UInt16 ? sizeof(uint16_t) : sizeof(uint32_t);
    return (index_buffer_->size() - index_buffer_offset_) / index_size;
  }

  if (!streams_.empty()) {
    return streams_[0].buffer->size() / streams_[0].layout.stride;
  }

  KEN_CORE_ASSERT(false, "No index buffer or vertex stream found!");
  return 0;
}

std::span<const VertexStreamBinding> OpenglVertexInput::vertex_streams() const { return streams_; }

const std::shared_ptr<Buffer>& OpenglVertexInput::index_buffer() const { return index_buffer_; }

IndexType OpenglVertexInput::index_type() const { return index_type_; }

std::size_t OpenglVertexInput::index_buffer_offset() const { return index_buffer_offset_; }

}  // namespace kEn
