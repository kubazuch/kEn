#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imguizmo/ImGuizmo.h>

#include <kEn/core/application.hpp>
#include <kEn/event/key_events.hpp>
#include <kEn/event/mouse_events.hpp>
#include <kEn/imgui/imgui_layer.hpp>
#include <kenpch.hpp>

namespace kEn {

ImguiLayer::ImguiLayer() : Layer("ImGuiLayer") {}

// https://github.com/ocornut/imgui/wiki/Getting-Started#example-if-you-are-using-glfw--openglwebgl
void ImguiLayer::on_attach() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // IF using Docking Branch
  io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  Application& app   = Application::instance();
  GLFWwindow* window = static_cast<GLFWwindow*>(app.main_window().native_window());
  ImGui_ImplGlfw_InitForOpenGL(
      window, true);  // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init("#version 410");
}

void ImguiLayer::on_detach() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImguiLayer::on_imgui() {
  // static bool show = true;
  // if(show)
  // 	ImGui::ShowDemoWindow(&show); // Show demo window! :)
}

void ImguiLayer::on_event(BaseEvent& event) {
  ImGuiIO& io = ImGui::GetIO();
  EventDispatcher dispatcher;

  auto on_mouse_event = [&](BaseEvent&) { return io.WantCaptureMouse; };

  auto on_keyboard_event = [&](BaseEvent&) { return io.WantCaptureKeyboard; };

  dispatcher.subscribe<MouseButtonPressedEvent>(on_mouse_event);
  dispatcher.subscribe<MouseButtonReleasedEvent>(on_mouse_event);
  dispatcher.subscribe<MouseMoveEvent>(on_mouse_event);
  dispatcher.subscribe<MouseScrollEvent>(on_mouse_event);
  dispatcher.subscribe<KeyPressedEvent>(on_keyboard_event);
  dispatcher.subscribe<KeyReleasedEvent>(on_keyboard_event);
  dispatcher.subscribe<KeyTypedEvent>(on_keyboard_event);

  dispatcher.dispatch(event);
}

void ImguiLayer::begin() {  // NOLINT
  // (Your code calls glfwPollEvents())
  // ...
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGuizmo::BeginFrame();
}

void ImguiLayer::end() {  // NOLINT
  // Rendering
  // (Your code clears your framebuffer, renders your other stuff etc.)
  ImGuiIO& io      = ImGui::GetIO();
  Application& app = Application::instance();
  io.DisplaySize =
      ImVec2(static_cast<float>(app.main_window().width()), static_cast<float>(app.main_window().height()));

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  // (Your code calls glfwSwapBuffers() etc.)

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(context);
  }
}

}  // namespace kEn
