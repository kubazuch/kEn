#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <kEn/core/application.hpp>
#include <kEn/core/assert.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/renderer/render_command.hpp>
#include <kEn/renderer/renderer.hpp>
#include <kenpch.hpp>

namespace kEn {
Application* Application::instance_ = nullptr;

Application::Application() {
  KEN_CORE_ASSERT(!instance_, "App already exists!");
  instance_ = this;

  window_ = std::unique_ptr<Window>(Window::create());
  window_->set_event_handler([this](auto& event) { window_event_handler(event); });

  dispatcher_ = std::make_unique<EventDispatcher>();
  dispatcher_->subscribe<WindowCloseEvent>(KEN_EVENT_SUBSCRIBER(on_window_close));
  dispatcher_->subscribe<WindowResizeEvent>(KEN_EVENT_SUBSCRIBER(on_window_resize));

  RenderCommand::init();

  imgui_layer_ = new ImguiLayer();
  push_overlay(imgui_layer_);
}

void Application::push_layer(Layer* layer) { layer_stack_.push_layer(layer); }

void Application::push_overlay(Layer* overlay) { layer_stack_.push_overlay(overlay); }

void Application::run() {
  double previous = glfwGetTime();  // TODO(): platform
  double lag      = 0.0;

  // TODO(): TEMP
  int frames           = 0;
  int ticks            = 0;
  double frame_counter = 0.0;

  vsync_ = window_->vsync();

  while (running_) {
    double current = glfwGetTime();  // TODO(): platform
    double delta   = current - previous;
    previous       = current;

    lag += delta;
    frame_counter += delta;

    while (lag >= KEN_TIME_PER_UPDATE) {
      update(KEN_TIME_PER_UPDATE);

      lag -= KEN_TIME_PER_UPDATE;
      time_ += KEN_TIME_PER_UPDATE;
      ++ticks;
    }

    ++frames;
    if (frame_counter >= 1.0) {
      fps_          = frames;
      tps_          = ticks;
      frames        = 0;
      ticks         = 0;
      frame_counter = 0;
    }

    render();
  }
}

void Application::update(double delta) {
  if (vsync_ != window_->vsync()) {
    window_->set_vsync(vsync_);
  }

  if (!minimized_) {
    for (Layer* layer : layer_stack_) {
      layer->on_update(delta, time_);
    }
  }
}

void Application::render() {
  for (Layer* layer : layer_stack_) {
    layer->on_render();
  }

  imgui_layer_->begin();
  {
    for (Layer* layer : layer_stack_) {
      layer->on_imgui();
    }

    ImGui::Begin("DEBUG");
    ImGui::Text("FPS: %d", fps_);
    ImGui::Text("TPS: %d", tps_);
    ImGui::Checkbox("VSync", &vsync_);
    ImGui::End();
  }
  imgui_layer_->end();

  window_->on_update();
}

void Application::window_event_handler(BaseEvent& e) {
  dispatcher_->dispatch(e);

  for (auto it = layer_stack_.rbegin(); it != layer_stack_.rend(); ++it) {
    if (e.handled) {
      break;
    }
    (*it)->on_event(e);
  }
}

bool Application::on_window_close(WindowCloseEvent& /*e*/) {
  running_ = false;
  return true;
}

bool Application::on_window_resize(WindowResizeEvent& e) {
  if (e.width() == 0 || e.height() == 0) {
    minimized_ = true;
    return true;
  }

  minimized_ = false;
  RenderCommand::set_viewport(0, 0, e.width(), e.height());
  return e.height() == 0;
}
}  // namespace kEn
