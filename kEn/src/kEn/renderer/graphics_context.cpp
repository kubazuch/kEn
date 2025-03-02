#include <kEn/core/assert.hpp>
#include <kEn/renderer/graphics_context.hpp>
#include <kEn/renderer/renderer_api.hpp>
#include <kenpch.hpp>
#include <platform/opengl/opengl_context.hpp>

namespace kEn {

std::unique_ptr<graphics_context> graphics_context::create(void* native_window) {
  switch (renderer_api::get_api()) {
    case renderer_api::api::none:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case renderer_api::api::opengl:
      return std::make_unique<opengl_context>(native_window);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

}  // namespace kEn
