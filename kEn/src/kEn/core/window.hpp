#pragma once

#include <GLFW/glfw3.h>

#include <array>
#include <functional>
#include <string>

#include <mEn/vec2.hpp>

#include <kEn/core/mod_keys.hpp>
#include <kEn/event/event.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/** @brief Construction parameters for a Window.
 *
 *  All fields have sensible defaults matching the engine's standard 1280x720
 *  configuration.  Passed by const-reference to the Window constructor.
 */
struct WindowProperties {
  std::string title;                 /**< Window title bar text. */
  unsigned int width;                /**< Window width in pixels. */
  unsigned int height;               /**< Window height in pixels. */
  bool opengl_debug_context = false; /**< Request an OpenGL debug context (GLFW_OPENGL_DEBUG_CONTEXT). */

  explicit WindowProperties(std::string title = "kEngine", unsigned int width = 1280, unsigned int height = 720)
      : title(std::move(title)), width(width), height(height) {}
};

/** @brief GLFW-backed OS window with an attached graphics context.
 *
 *  @details
 *  Window manages the full GLFW lifecycle for a single native window:
 *  - Initialises GLFW on first construction and terminates it when the last
 *    Window is destroyed (tracked via @c glfw_window_count_).
 *  - Creates and owns a @ref GraphicsContext (graphics context + swap chain).
 *  - Registers GLFW callbacks that translate OS input into kEn events and
 *    forward them to the registered @ref handler_t callback.
 *
 *  **Events produced** (dispatched through the installed handler):
 *  - @c WindowCloseEvent, @c WindowResizeEvent
 *  - @c KeyPressedEvent (with repeat flag), @c KeyReleasedEvent, @c KeyTypedEvent
 *  - @c MouseButtonPressedEvent, @c MouseButtonReleasedEvent
 *  - @c MouseMoveEvent, @c MouseScrollEvent
 *  - @c MouseDragEvent -- synthesised while a button is held and the cursor moves;
 *    carries the original press position alongside the current position.
 *
 *  4x MSAA is requested unconditionally.
 *  In debug builds a graphics API debug context is requested.
 *
 *  @note Only one Window is expected per application in the current design
 *        (Application holds a single @c std::unique_ptr<Window>).
 */
class Window {
 public:
  /** @brief Callable type for the event handler installed via @ref set_event_handler(). */
  using handler_t = std::function<void(BaseEvent&)>;

  /** @brief Creates the native window and initialises the graphics context.
   *
   *  Initialises GLFW the first time a Window is constructed.  Requests a
   *  4x MSAA framebuffer and (in debug builds) a graphics API debug context.
   *
   *  @param properties  Title and dimensions for the new window.
   */
  explicit Window(const WindowProperties& properties = WindowProperties());

  /** @brief Destroys the native window and shuts down GLFW if this was the last window. */
  ~Window();

  DELETE_COPY_MOVE(Window);

  /** @brief Processes all pending OS events via @c glfwPollEvents(). */
  void poll_events();

  /** @brief Current window width in pixels. */
  [[nodiscard]] unsigned int width() const { return data_.width; }

  /** @brief Current window height in pixels. */
  [[nodiscard]] unsigned int height() const { return data_.height; }

  /** @brief Current window dimensions as a Vec2 (width, height). */
  [[nodiscard]] mEn::Vec2 size() const { return {data_.width, data_.height}; }

  /** @brief Installs the callback that receives all kEn events from this window.
   *
   *  Application sets this in its constructor to route events into its own
   *  dispatcher and layer stack.
   *
   *  @param handler  Callable invoked with each @ref BaseEvent as it occurs.
   */
  void set_event_handler(handler_t handler) { data_.handler = std::move(handler); }

  /** @brief Enables or disables VSync (swap interval 1 or 0).
   *  @param enabled  @c true to enable VSync, @c false to disable.
   */
  void set_vsync(bool enabled);

  /** @brief Returns @c true if VSync is currently enabled. */
  [[nodiscard]] bool vsync() const;

  /** @brief Returns the underlying GLFWwindow pointer for platform-specific use (e.g., ImGui). */
  [[nodiscard]] GLFWwindow* native_window() const { return window_ptr_; }

 private:
  /** @brief Registers all GLFW input/window callbacks that produce kEn events. */
  void set_glfw_callbacks();

  /** @brief Number of live Window instances; controls GLFW init/shutdown. */
  static uint8_t glfw_window_count_;

  GLFWwindow* window_ptr_;

  /** @brief Per-button drag tracking: records whether a drag is active and where it started. */
  struct DragState {
    bool active{};    /**< True while the mouse button is held after the initial press. */
    mEn::Vec2 from{}; /**< Cursor position at the time the button was pressed. */
  };

  /** @brief POD bundle stored as the GLFW window user pointer for use inside callbacks. */
  struct Data {
    std::string title;
    unsigned int width{}, height{};

    bool vsync{};

    std::array<DragState, GLFW_MOUSE_BUTTON_LAST> drag_state{};
    ModKeys active_mods; /**< Modifier key state updated on every key/mouse-button event. */

    handler_t handler;
  };

  Data data_;
};

}  // namespace kEn
