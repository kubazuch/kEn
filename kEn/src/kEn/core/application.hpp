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
#include <kEn/renderer/device.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/** @brief Configuration parameters for the Application.
 *
 *  Passed to the Application constructor to configure the main window.
 *  All fields have sensible defaults; only override what differs.
 */
struct ApplicationSpec {
  std::string title          = "kEngine";           /**< Window title bar text. */
  unsigned int window_width  = 1280;                /**< Initial window width in pixels. */
  unsigned int window_height = 720;                 /**< Initial window height in pixels. */
  Device::Api api            = Device::Api::OpenGL; /**< Graphics API to use. */
  bool enable_debug          = false; /**< Enable GPU debug output and push the internal DebugLayer overlay. */
};

/** @brief Core singleton that owns the main loop, window, and layer stack.
 *
 *  @details
 *  Application implements a fixed-timestep game loop decoupled from rendering:
 *  - **Update** runs at a fixed 60 TPS (@ref kTickTime).  Each tick advances
 *    every layer via Layer::on_update() with a constant @ref Timestep.
 *  - **Render** runs as fast as possible.  The `alpha` parameter passed to
 *    Layer::on_render() is the fractional tick interpolation factor
 *    `lag / kTickTime`, allowing smooth sub-tick interpolation.
 *  - **ImGui** passes are issued once per frame after all render passes.
 *
 *  Only one Application may exist at a time (asserted in the constructor).
 *  The global instance is accessible via @ref instance().
 *
 *  Layers are processed front-to-back for updates/renders and back-to-front
 *  for event delivery.  Overlays (pushed via @ref push_overlay()) always sit
 *  on top of regular layers in the stack.
 *
 *  @note Subclass this and implement @ref create_application() to define the
 *        entry point for a kEn application.
 */
class Application {
 public:
  /** @brief Constructs the application, creates the window, and initialises the renderer.
   *
   *  Automatically pushes an ImguiLayer overlay (and a DebugLayer when `spec.enable_debug` is set).
   *  Asserts that no other Application instance exists.
   *
   *  @param spec  Window and title configuration.  Defaults to @ref ApplicationSpec{}.
   */
  explicit Application(ApplicationSpec spec = {});

  virtual ~Application() = default;

  /** @brief Runs the main loop until the window is closed.
   *
   *  Processes window events, drives fixed-rate updates, and renders each frame.
   *  Updates @ref fps() and @ref tps() counters once per wall-clock second.
   *  Returns only after @ref running_ is set to false (e.g., on WindowCloseEvent).
   */
  void run();

  /** @brief Pushes a layer onto the layer stack below all overlays.
   *  @param layer  Ownership is transferred to the stack; on_attach() is called immediately.
   */
  void push_layer(std::unique_ptr<Layer> layer);

  /** @brief Pushes an overlay on top of all regular layers in the stack.
   *  @param overlay  Ownership is transferred to the stack; on_attach() is called immediately.
   */
  void push_overlay(std::unique_ptr<Layer> overlay);

  /** @brief Returns a reference to the application's main window. */
  Window& main_window() const { return *window_; }

  /** @brief Returns a reference to the GPU device (command dispatch + resource creation). */
  Device& device() const { return *device_; }

  /** @brief Returns the global Application singleton.
   *  @pre An Application instance must have been constructed.
   */
  static Application& instance() { return *instance_; }

  /** @brief Frames per second measured over the last completed wall-clock second. */
  [[nodiscard]] uint16_t fps() const { return fps_; }

  /** @brief Ticks per second measured over the last completed wall-clock second. */
  [[nodiscard]] uint16_t tps() const { return tps_; }

  DELETE_COPY_MOVE(Application);

  /** @brief Fixed tick duration targeting 60 TPS (approx. 16.667 ms per tick). */
  static constexpr duration_t kTickTime = std::chrono::microseconds(16667);  // 60 TPS = 16.(6) ms/t

 private:
  /** @brief Advances all layers by one fixed tick. */
  void update();

  /** @brief Renders all layers with the given interpolation factor.
   *  @param alpha  Fractional position within the current tick in [0, 1).
   */
  void render(double alpha);

  bool on_window_close(WindowCloseEvent& e);
  bool on_window_resize(WindowResizeEvent& e);

  /** @brief Dispatches a window event to the Application's own handlers first,
   *         then propagates it down the layer stack in reverse order until handled.
   */
  void window_event_handler(BaseEvent& e);

  std::unique_ptr<Window> window_;
  std::unique_ptr<Device> device_;
  EventDispatcher dispatcher_;
  bool running_   = true;
  bool minimized_ = false;
  LayerStack layer_stack_;

  ApplicationSpec spec_;
  duration_t time_{};
  uint16_t fps_ = 0, tps_ = 0;

  static Application* instance_;
};

/** @brief Convenience accessor -- returns the GPU device of the current application.
 *
 *  Equivalent to @c Application::instance().device() but shorter at call sites.
 *
 *  @deprecated Will be removed, when complete kEn refactor is finished.
 */
inline Device& device() { return Application::instance().device(); }

/** @brief Factory function that the client application must define.
 *
 *  Called from main() (defined in entry_point.hpp) to create the concrete
 *  Application subclass.  The returned pointer is wrapped in a unique_ptr
 *  and ownership is managed by the runtime.
 *
 *  @param args  Command-line arguments forwarded from @c main().
 *  @return      Heap-allocated Application subclass instance.
 */
Application* create_application(std::vector<std::string_view> args);
}  // namespace kEn
