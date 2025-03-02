#include <kEn/renderer/renderer_api.hpp>
#include <kEn/renderer/vertex_array.hpp>
#include <kenpch.hpp>
#include <platform/opengl/opengl_renderer_api.hpp>

namespace kEn {

std::unique_ptr<renderer_api> renderer_api::create() {
  switch (get_api()) {
    case api::none:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case api::opengl:
      return std::make_unique<opengl_renderer_api>();
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

}  // namespace kEn
