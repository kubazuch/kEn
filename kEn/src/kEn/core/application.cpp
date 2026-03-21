#include "application.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <ranges>
#include <utility>

#include <kEn/core/assert.hpp>
#include <kEn/core/layer.hpp>
#include <kEn/core/timestep.hpp>
#include <kEn/core/window.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/event/event.hpp>
#ifdef KEN_DEBUG_BUILD
#include <kEn/imgui/debug_layer.hpp>
#endif
#include <kEn/imgui/imgui_frame.hpp>
#include <kEn/imgui/imgui_layer.hpp>
#include <kEn/renderer/render_command.hpp>

namespace kEn {
Application* Application::instance_ = nullptr;

Application::Application() {
  KEN_CORE_ASSERT(!instance_, "App already exists!");
  instance_ = this;

  // TODO(kuzu): main window name?

  window_ = std::make_unique<Window>();
  window_->set_event_handler([this](auto& event) { window_event_handler(event); });

  dispatcher_.subscribe(this, &Application::on_window_close);
  dispatcher_.subscribe(this, &Application::on_window_resize);

  RenderCommand::init();

  push_overlay(std::make_unique<ImguiLayer>());
#ifdef KEN_DEBUG_BUILD
  push_overlay(std::make_unique<DebugLayer>());
#endif
}

void Application::push_layer(std::unique_ptr<Layer> layer) { layer_stack_.push_layer(std::move(layer)); }

void Application::push_overlay(std::unique_ptr<Layer> overlay) { layer_stack_.push_overlay(std::move(overlay)); }

void Application::run() {
  using clock = std::chrono::high_resolution_clock;
  duration_t lag{};
  duration_t second{};
  auto previous_time = clock::now();

  size_t frames = 0;
  size_t ticks  = 0;

  while (running_) {
    window_->poll_events();

    auto current_time = clock::now();
    auto delta        = current_time - previous_time;
    previous_time     = current_time;

    lag += std::chrono::duration_cast<duration_t>(delta);
    second += std::chrono::duration_cast<duration_t>(delta);

    while (lag >= kTickTime) {
      update();

      lag -= kTickTime;
      time_ += kTickTime;
      ++ticks;
    }

    ++frames;
    if (second > std::chrono::seconds(1)) {
      const uint16_t seconds = static_cast<uint16_t>(std::chrono::duration_cast<std::chrono::seconds>(second).count());

      fps_   = static_cast<uint16_t>(frames / seconds);
      tps_   = static_cast<uint16_t>(ticks / seconds);
      frames = 0;
      ticks  = 0;
      second = duration_t{};
    }

    render(std::chrono::duration<double>(lag).count() / std::chrono::duration<double>(kTickTime).count());
    window_->swap_buffers();
  }
}

void Application::update() {
  for (const auto& layer : layer_stack_) {
    layer->on_update(Timestep{kTickTime}, Timestep{time_});
  }
}

void Application::render(double alpha) {
  if (!minimized_) {
    for (const auto& layer : layer_stack_) {
      layer->on_render(alpha);
    }

    {
      const ImguiFrame frame;

      for (const auto& layer : layer_stack_) {
        layer->on_imgui();
      }
    }
  }
}

void Application::window_event_handler(BaseEvent& e) {
  if (dispatcher_.dispatch(e)) {
    return;
  }

  for (const auto& it : std::ranges::reverse_view(layer_stack_)) {
    if (it->on_event(e)) {
      break;
    }
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
  return false;
}
}  // namespace kEn
