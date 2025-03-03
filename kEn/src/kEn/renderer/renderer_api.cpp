#include <kEn/renderer/renderer_api.hpp>
#include <kEn/renderer/vertex_array.hpp>
#include <kenpch.hpp>
#include <platform/opengl/opengl_renderer_api.hpp>

namespace kEn {

std::unique_ptr<RendererApi> RendererApi::create() {
  switch (get_api()) {
    case Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case Api::OpenGL:
      return std::make_unique<OpenglRendererApi>();
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

}  // namespace kEn
