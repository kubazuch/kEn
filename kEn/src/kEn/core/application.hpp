#pragma once
#include <kEn/core/core.hpp>
#include <kEn/core/layer_stack.hpp>
#include <kEn/core/window.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/event/event.hpp>
#include <kEn/imgui/imgui_layer.hpp>

#define KEN_TIME_PER_UPDATE (1.0 / 120.0)

int main(int argc, char** argv);

namespace kEn {

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
  void update(double delta);
  void render();

  bool on_window_close(WindowCloseEvent& e);
  bool on_window_resize(WindowResizeEvent& e);

 private:
  std::unique_ptr<Window> window_;
  std::unique_ptr<EventDispatcher> dispatcher_;
  ImguiLayer* imgui_layer_;
  bool running_   = true;
  bool minimized_ = false;
  LayerStack layer_stack_;

  bool vsync_  = true;
  double time_ = 0.0;
  int fps_ = 0, tps_ = 0;

 private:
  static Application* instance_;
  friend int ::main(int argc, char** argv);
};

// Client must define this!
Application* create_application();
}  // namespace kEn
