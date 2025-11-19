#include "framebuffer.hpp"

#include <memory>

#include <kEn/core/assert.hpp>
#include <kEn/renderer/renderer_api.hpp>

#include <platform/opengl/opengl_framebuffer.hpp>

namespace kEn {

std::shared_ptr<Framebuffer> Framebuffer::create(const FramebufferSpec& spec) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglFramebuffer>(spec);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

}  // namespace kEn
