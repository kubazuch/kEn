#include "renderer_api.hpp"

#include <memory>

#include <kEn/core/assert.hpp>

#include <platform/opengl/opengl_renderer_api.hpp>

namespace kEn {

std::unique_ptr<RendererApi> RendererApi::create() noexcept {
  switch (get_api()) {
    case Api::None:
      KEN_UNREACHABLE();
    case Api::OpenGL:
      return std::make_unique<OpenglRendererApi>();
  }

  KEN_UNREACHABLE();
}

}  // namespace kEn
