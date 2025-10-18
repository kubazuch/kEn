#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <chrono>
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
  using clock = std::chrono::high_resolution_clock;
  duration_t lag(0ns);
  duration_t second(0ns);
  auto previous_time = clock::now();

  size_t frames = 0;
  size_t ticks  = 0;

  vsync_ = window_->vsync();

  while (running_) {
    auto current_time = clock::now();
    auto delta        = current_time - previous_time;
    previous_time     = current_time;

    lag += std::chrono::duration_cast<duration_t>(delta);
    second += std::chrono::duration_cast<duration_t>(delta);

    while (lag >= tick_time_) {
      update();

      lag -= tick_time_;
      time_ += tick_time_;
      ++ticks;
    }

    ++frames;
    if (second > 1s) {
      uint16_t seconds = static_cast<uint16_t>(std::chrono::duration_cast<std::chrono::seconds>(second).count());

      fps_   = static_cast<uint16_t>(frames / seconds);
      tps_   = static_cast<uint16_t>(ticks / seconds);
      frames = 0;
      ticks  = 0;
      second = 0ns;
    }

    render(std::chrono::duration<double>(lag).count() / std::chrono::duration<double>(tick_time_).count());
  }
}

void Application::update() {
  if (vsync_ != window_->vsync()) {
    window_->set_vsync(vsync_);
  }

  if (!minimized_) {
    for (Layer* layer : layer_stack_) {
      layer->on_update(tick_time_, time_);
    }
  }
}

void Application::render(double alpha) {
  for (Layer* layer : layer_stack_) {
    layer->on_render(alpha);
  }

  imgui_layer_->begin();
  {
    for (Layer* layer : layer_stack_) {
      layer->on_imgui();
    }

    if (ImGui::Begin("DEBUG")) {
      float ms = std::chrono::duration<float, std::milli>(tick_time_).count();
      if (ImGui::SliderFloat("Tick time", &ms, 1, 1000, "%.3f ms", ImGuiSliderFlags_Logarithmic)) {
        tick_time_ = std::chrono::duration_cast<duration_t>(std::chrono::duration<float, std::milli>(ms));
      }

      ImGui::Text("FPS: %d", fps_);
      ImGui::Text("TPS: %d", tps_);
      ImGui::Checkbox("VSync", &vsync_);
    }
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
