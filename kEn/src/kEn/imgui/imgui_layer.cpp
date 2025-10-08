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

ImguiLayer::ImguiLayer() : Layer("ImGuiLayer") {
  dispatcher_.subscribe<MouseButtonPressedEvent>(KEN_EVENT_SUBSCRIBER(on_mouse_event));
  dispatcher_.subscribe<MouseButtonReleasedEvent>(KEN_EVENT_SUBSCRIBER(on_mouse_event));
  dispatcher_.subscribe<MouseMoveEvent>(KEN_EVENT_SUBSCRIBER(on_mouse_event));
  dispatcher_.subscribe<MouseScrollEvent>(KEN_EVENT_SUBSCRIBER(on_mouse_event));
  dispatcher_.subscribe<MouseDragEvent>(KEN_EVENT_SUBSCRIBER(on_mouse_event));

  dispatcher_.subscribe<KeyPressedEvent>(KEN_EVENT_SUBSCRIBER(on_keyboard_event));
  dispatcher_.subscribe<KeyReleasedEvent>(KEN_EVENT_SUBSCRIBER(on_keyboard_event));
  dispatcher_.subscribe<KeyTypedEvent>(KEN_EVENT_SUBSCRIBER(on_keyboard_event));
}

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

void ImguiLayer::on_event(BaseEvent& event) { dispatcher_.dispatch(event); }

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

bool ImguiLayer::on_mouse_event(BaseEvent&) {  // NOLINT
  const ImGuiIO& io = ImGui::GetIO();
  return io.WantCaptureMouse;
}

bool ImguiLayer::on_keyboard_event(BaseEvent&) {  // NOLINT
  const ImGuiIO& io = ImGui::GetIO();
  return io.WantCaptureKeyboard;
}

}  // namespace kEn
