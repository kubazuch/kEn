#include "graphics_context.hpp"

#include <GLFW/glfw3.h>

#include <memory>

#include <kEn/core/assert.hpp>
#include <kEn/renderer/renderer_api.hpp>

#include <platform/opengl/opengl_context.hpp>

namespace kEn {

std::unique_ptr<GraphicsContext> GraphicsContext::create(GLFWwindow* native_window) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_UNREACHABLE();
    case RendererApi::Api::OpenGL:
      return std::make_unique<OpenglContext>(native_window);
  }

  KEN_UNREACHABLE();
}

}  // namespace kEn
