#include <kEn/renderer/renderer_api.hpp>
#include <kEn/renderer/vertex_array.hpp>
#include <kenpch.hpp>
#include <platform/opengl/opengl_vertex_array.hpp>

namespace kEn {

std::unique_ptr<vertex_array> vertex_array::create() {
  switch (renderer_api::get_api()) {
    case renderer_api::api::none:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case renderer_api::api::opengl:
      return std::make_unique<opengl_vertex_array>();
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

}  // namespace kEn
