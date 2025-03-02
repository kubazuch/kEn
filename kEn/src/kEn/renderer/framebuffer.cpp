#include <kEn/renderer/framebuffer.hpp>
#include <kEn/renderer/renderer.hpp>
#include <kEn/renderer/renderer_api.hpp>
#include <kenpch.hpp>
#include <platform/opengl/opengl_framebuffer.hpp>


namespace kEn {

std::shared_ptr<framebuffer> framebuffer::create(const framebuffer_spec& spec) {
  switch (renderer_api::get_api()) {
    case renderer_api::api::none:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case renderer_api::api::opengl:
      return std::make_shared<opengl_framebuffer>(spec);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

}  // namespace kEn
