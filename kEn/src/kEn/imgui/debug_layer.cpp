#include "debug_layer.hpp"

#include <imgui/imgui.h>

#include <kEn/core/application.hpp>

namespace kEn {

DebugLayer::DebugLayer() : Layer("DebugLayer") {}

void DebugLayer::on_imgui() {
  if (ImGui::Begin("DEBUG")) {
    Application& app = Application::instance();
    ImGui::Text("FPS: %d", app.fps());
    ImGui::Text("TPS: %d", app.tps());
    bool vsync = app.main_window().vsync();
    if (ImGui::Checkbox("VSync", &vsync)) {
      app.main_window().set_vsync(vsync);
    }
  }
  ImGui::End();
}

}  // namespace kEn