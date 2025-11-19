#include "renderer_api.hpp"

#include <memory>

#include <kEn/core/assert.hpp>

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
