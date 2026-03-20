#include "imgui_frame.hpp"

#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>

#include <kEn/core/application.hpp>
#include <kEn/imgui/imgui_backend.hpp>

namespace kEn {

ImguiFrame::ImguiFrame() {
  ImguiBackend::instance().new_frame();
  ImGui::NewFrame();
  ImGuizmo::BeginFrame();
}

ImguiFrame::~ImguiFrame() {
  ImGuiIO& io            = ImGui::GetIO();
  const Application& app = Application::instance();
  io.DisplaySize =
      ImVec2(static_cast<float>(app.main_window().width()), static_cast<float>(app.main_window().height()));

  ImGui::Render();
  ImguiBackend::instance().render();
}

}  // namespace kEn
