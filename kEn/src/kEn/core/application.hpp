#pragma once

#include <chrono>
#include <memory>

#include <kEn/core/core.hpp>
#include <kEn/core/layer_stack.hpp>
#include <kEn/core/timestep.hpp>
#include <kEn/core/window.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/event/event.hpp>

/** @file
 *  @ingroup ken
 */

int main(int argc, char** argv);

namespace kEn {

class Application {
 public:
  Application();
  virtual ~Application() = default;

  void window_event_handler(BaseEvent& e);

  void push_layer(std::unique_ptr<Layer> layer);
  void push_overlay(std::unique_ptr<Layer> overlay);

  Window& main_window() const { return *window_; }
  static Application& instance() { return *instance_; }

  DELETE_COPY_MOVE(Application);

 private:
  void run();
  void update();
  void render(double alpha);

  bool on_window_close(WindowCloseEvent& e);
  bool on_window_resize(WindowResizeEvent& e);

 public:
  static constexpr duration_t kTickTime = std::chrono::microseconds(16667);  // 60 TPS = 16.(6) ms/t

 private:
  std::unique_ptr<Window> window_;
  EventDispatcher dispatcher_;
  bool running_   = true;
  bool minimized_ = false;
  LayerStack layer_stack_;

  bool vsync_ = true;
  duration_t time_{};
  uint16_t fps_ = 0, tps_ = 0;

 private:
  static Application* instance_;
  friend int ::main(int argc, char** argv);
};

// Client must define this!
Application* create_application();
}  // namespace kEn
