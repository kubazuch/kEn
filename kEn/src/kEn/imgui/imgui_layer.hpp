#pragma once

#include <kEn/core/layer.hpp>
#include <kEn/event/event.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * Layer that owns the Dear ImGui backend lifecycle and handles input capture.
 *
 * Responsibilities:
 *   - on_attach(): initializes the ImGui context, style, DPI scaling, and the
 *     GLFW + OpenGL3 backend (ImGui_ImplGlfw / ImGui_ImplOpenGL3).
 *   - on_detach(): shuts down the backends and destroys the ImGui context.
 *   - on_imgui(): override point for the ImguiLayer's own widgets (currently empty).
 *   - on_event(): intercepts mouse and keyboard events when ImGui wants to capture
 *     them (io.WantCaptureMouse / io.WantCaptureKeyboard), preventing them from
 *     reaching layers below.
 *
 * Frame rendering is NOT handled here. Use ImguiFrame (a separate RAII guard) to
 * bracket on_imgui() calls in the render loop.
 *
 * @note Creating multiple ImguiLayer instances is undefined behavior: the ImGui
 *       context is global and can only be initialized once per process.
 */
class ImguiLayer final : public Layer {
 public:
  ImguiLayer();
  ~ImguiLayer() override = default;

  /** Initializes the ImGui context and GLFW/OpenGL3 backends. */
  void on_attach() override;

  /** Shuts down GLFW/OpenGL3 backends and destroys the ImGui context. */
  void on_detach() override;

  /** Override point for ImguiLayer's own Dear ImGui widgets. Currently a no-op. */
  void on_imgui() override;

  /**
   * Dispatches the event to mouse/keyboard capture handlers.
   * @return true if ImGui wants to capture this event type, preventing propagation.
   */
  bool on_event(BaseEvent& event) override;

 private:
  /**
   * Consumes mouse events when ImGui has focus.
   * @return io.WantCaptureMouse.
   */
  bool on_mouse_event(BaseEvent&);

  /**
   * Consumes keyboard events when ImGui has focus.
   * @return io.WantCaptureKeyboard.
   */
  bool on_keyboard_event(BaseEvent&);

  EventDispatcher dispatcher_;
};

}  // namespace kEn
