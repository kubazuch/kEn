#include "imgui_backend.hpp"

#include <memory>

#include <kEn/core/assert.hpp>
#include <kEn/renderer/renderer_api.hpp>

#include <platform/opengl/opengl_imgui_backend.hpp>

namespace kEn {

ImguiBackend* ImguiBackend::instance_ = nullptr;

ImguiBackend& ImguiBackend::instance() {
  KEN_CORE_ASSERT(instance_, "ImguiBackend::instance() called before ImguiLayer was attached");
  return *instance_;
}

void ImguiBackend::set_instance(ImguiBackend* backend) { instance_ = backend; }

std::unique_ptr<ImguiBackend> ImguiBackend::create() {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_UNREACHABLE();
    case RendererApi::Api::OpenGL:
      return std::make_unique<OpenglImguiBackend>();
  }

  KEN_UNREACHABLE();
}

}  // namespace kEn
