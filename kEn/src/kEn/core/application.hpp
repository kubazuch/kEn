#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <kEn/core/core.hpp>
#include <kEn/core/layer_stack.hpp>
#include <kEn/core/timestep.hpp>
#include <kEn/core/window.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/event/event.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

struct ApplicationSpec {
  std::string title          = "kEngine";
  unsigned int window_width  = 1280;
  unsigned int window_height = 720;
};

class Application {
 public:
  explicit Application(ApplicationSpec spec = {});
  virtual ~Application() = default;
  void run();

  void push_layer(std::unique_ptr<Layer> layer);
  void push_overlay(std::unique_ptr<Layer> overlay);

  Window& main_window() const { return *window_; }
  static Application& instance() { return *instance_; }

  [[nodiscard]] uint16_t fps() const { return fps_; }
  [[nodiscard]] uint16_t tps() const { return tps_; }

  DELETE_COPY_MOVE(Application);

  static constexpr duration_t kTickTime = std::chrono::microseconds(16667);  // 60 TPS = 16.(6) ms/t

 private:
  void update();
  void render(double alpha);

  bool on_window_close(WindowCloseEvent& e);
  bool on_window_resize(WindowResizeEvent& e);
  void window_event_handler(BaseEvent& e);

  std::unique_ptr<Window> window_;
  EventDispatcher dispatcher_;
  bool running_   = true;
  bool minimized_ = false;
  LayerStack layer_stack_;

  ApplicationSpec spec_;
  duration_t time_{};
  uint16_t fps_ = 0, tps_ = 0;

  static Application* instance_;
};

// Client must define this!
Application* create_application(std::vector<std::string_view> args);
}  // namespace kEn
