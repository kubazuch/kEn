#include "imgui_frame.hpp"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imguizmo/ImGuizmo.h>

#include <kEn/core/application.hpp>

namespace kEn {

ImguiFrame::ImguiFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGuizmo::BeginFrame();
}

ImguiFrame::~ImguiFrame() {
  ImGuiIO& io            = ImGui::GetIO();
  const Application& app = Application::instance();
  io.DisplaySize =
      ImVec2(static_cast<float>(app.main_window().width()), static_cast<float>(app.main_window().height()));

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(context);
  }
}

}  // namespace kEn
