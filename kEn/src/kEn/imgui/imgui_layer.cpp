#include "imgui_layer.hpp"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>

#include <kEn/core/application.hpp>
#include <kEn/core/layer.hpp>
#include <kEn/event/event.hpp>
#include <kEn/event/key_events.hpp>
#include <kEn/event/mouse_events.hpp>
#include <kEn/imgui/imgui_backend.hpp>

namespace kEn {

ImguiLayer::ImguiLayer() : Layer("ImGuiLayer") {
  dispatcher_.subscribe<MouseButtonPressedEvent>(KEN_BIND_EVENT_HANDLER(on_mouse_event));
  dispatcher_.subscribe<MouseButtonReleasedEvent>(KEN_BIND_EVENT_HANDLER(on_mouse_event));
  dispatcher_.subscribe<MouseMoveEvent>(KEN_BIND_EVENT_HANDLER(on_mouse_event));
  dispatcher_.subscribe<MouseScrollEvent>(KEN_BIND_EVENT_HANDLER(on_mouse_event));
  dispatcher_.subscribe<MouseDragEvent>(KEN_BIND_EVENT_HANDLER(on_mouse_event));

  dispatcher_.subscribe<KeyPressedEvent>(KEN_BIND_EVENT_HANDLER(on_keyboard_event));
  dispatcher_.subscribe<KeyReleasedEvent>(KEN_BIND_EVENT_HANDLER(on_keyboard_event));
  dispatcher_.subscribe<KeyTypedEvent>(KEN_BIND_EVENT_HANDLER(on_keyboard_event));
}

ImguiLayer::~ImguiLayer() = default;

// https://github.com/ocornut/imgui/wiki/Getting-Started#example-if-you-are-using-glfw--openglwebgl
void ImguiLayer::on_attach() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;        // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;  // Don't change cursor automatically
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;      // Enable Multi-Viewport / Platform Windows

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup scaling
  const float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
  ImGuiStyle& style      = ImGui::GetStyle();
  style.ScaleAllSizes(main_scale);
  style.FontScaleDpi     = main_scale;  // Set initial font scale.
  io.ConfigDpiScaleFonts = true;  // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor
                                  // DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
  io.ConfigDpiScaleViewports = true;  // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

  // Setup renderer backend
  backend_ = ImguiBackend::create();
  backend_->init(Application::instance().main_window().native_window());
  ImguiBackend::set_instance(backend_.get());
}

void ImguiLayer::on_detach() {
  ImguiBackend::set_instance(nullptr);
  backend_->shutdown();
  backend_.reset();
  ImGui::DestroyContext();
}

void ImguiLayer::on_imgui() {
  // static bool show = true;
  // if(show)
  // 	ImGui::ShowDemoWindow(&show); // Show demo window! :)
}

bool ImguiLayer::on_event(BaseEvent& event) { return dispatcher_.dispatch(event); }

bool ImguiLayer::on_mouse_event(BaseEvent&) {  // NOLINT(readability-convert-member-functions-to-static)
  const ImGuiIO& io = ImGui::GetIO();
  return io.WantCaptureMouse;
}

bool ImguiLayer::on_keyboard_event(BaseEvent&) {  // NOLINT(readability-convert-member-functions-to-static)
  const ImGuiIO& io = ImGui::GetIO();
  return io.WantCaptureKeyboard;
}

}  // namespace kEn
