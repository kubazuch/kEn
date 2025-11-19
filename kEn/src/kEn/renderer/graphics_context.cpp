#include "graphics_context.hpp"

#include <kEn/core/assert.hpp>
#include <kEn/renderer/renderer_api.hpp>

#include <platform/opengl/opengl_context.hpp>

namespace kEn {

std::unique_ptr<GraphicsContext> GraphicsContext::create(void* native_window) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_unique<OpenglContext>(native_window);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

}  // namespace kEn
