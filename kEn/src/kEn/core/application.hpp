#pragma once

#include <chrono>
#include <kEn/core/core.hpp>
#include <kEn/core/layer_stack.hpp>
#include <kEn/core/window.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/event/event.hpp>
#include <kEn/imgui/imgui_layer.hpp>

int main(int argc, char** argv);

namespace kEn {

using namespace std::chrono_literals;
using duration_t = std::chrono::nanoseconds;

class Application {
 public:
  Application();
  virtual ~Application() = default;

  void window_event_handler(BaseEvent& e);

  void push_layer(Layer* layer);
  void push_overlay(Layer* overlay);

  Window& main_window() const { return *window_; }
  static Application& instance() { return *instance_; }

  VIRTUAL_FIVE(Application);

 private:
  void run();
  void update(duration_t delta);
  void render(duration_t delta);

  bool on_window_close(WindowCloseEvent& e);
  bool on_window_resize(WindowResizeEvent& e);

 public:
  static constexpr duration_t kTickTime = 16667us;  // 60 TPS = 16.(6) ms/t

 private:
  std::unique_ptr<Window> window_;
  std::unique_ptr<EventDispatcher> dispatcher_;
  ImguiLayer* imgui_layer_;
  bool running_   = true;
  bool minimized_ = false;
  LayerStack layer_stack_;

  bool vsync_      = true;
  duration_t time_ = 0ns;
  uint16_t fps_ = 0, tps_ = 0;

 private:
  static Application* instance_;
  friend int ::main(int argc, char** argv);
};

// Client must define this!
Application* create_application();
}  // namespace kEn
