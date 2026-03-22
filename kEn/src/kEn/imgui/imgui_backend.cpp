#include "imgui_backend.hpp"

#include <kEn/core/assert.hpp>

namespace kEn {

ImguiBackend* ImguiBackend::instance_ = nullptr;

ImguiBackend& ImguiBackend::instance() {
  KEN_CORE_ASSERT(instance_, "ImguiBackend::instance() called before ImguiLayer was attached");
  return *instance_;
}

void ImguiBackend::set_instance(ImguiBackend* backend) { instance_ = backend; }

}  // namespace kEn
