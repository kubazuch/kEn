#include <kEn/core/assert.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/renderer_api.hpp>
#include <kenpch.hpp>
#include <platform/opengl/opengl_buffer.hpp>

namespace kEn {

std::shared_ptr<vertex_buffer> vertex_buffer::create(void* vertices, uint32_t size) {
  switch (renderer_api::get_api()) {
    case renderer_api::api::none:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case renderer_api::api::opengl:
      return std::make_shared<opengl_vertex_buffer>(vertices, size);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

std::shared_ptr<mutable_vertex_buffer> mutable_vertex_buffer::create(void* vertices, uint32_t size) {
  switch (renderer_api::get_api()) {
    case renderer_api::api::none:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case renderer_api::api::opengl:
      return std::make_shared<opengl_mutable_vertex_buffer>(vertices, size);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

std::shared_ptr<index_buffer> index_buffer::create(uint32_t* indices, uint32_t size) {
  switch (renderer_api::get_api()) {
    case renderer_api::api::none:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case renderer_api::api::opengl:
      return std::make_shared<opengl_index_buffer>(indices, size);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}
}  // namespace kEn
