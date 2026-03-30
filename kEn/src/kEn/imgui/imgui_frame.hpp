#pragma once

#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief RAII guard for a single Dear ImGui render frame.
 *
 * Construction calls the active ImguiBackend's new_frame(), then ImGui::NewFrame()
 * and ImGuizmo::BeginFrame() to start the frame.
 * Destruction calls ImGui::Render() and delegates final draw submission to the
 * active ImguiBackend's render().
 *
 * Usage:
 * @code
 *   {
 *     ImguiFrame frame;
 *     for (auto& layer : layer_stack_) layer->on_imgui();
 *   }  // frame is rendered here
 * @endcode
 *
 * @warning Creating more than one ImguiFrame at a time is undefined behavior.
 *          ImguiLayer::on_attach() must have been called before the first ImguiFrame
 *          is constructed; ImguiLayer::on_detach() must not be called while one is live.
 */
class ImguiFrame {
 public:
  /** @brief Starts the Dear ImGui frame. Must be followed by a matching destructor before the next frame. */
  ImguiFrame();

  /** @brief Finalizes and renders the accumulated Dear ImGui draw data.
   *  @note Updates `ImGuiIO::DisplaySize` from the current window dimensions before rendering.
   */
  ~ImguiFrame();

  DELETE_COPY_MOVE(ImguiFrame);
};

}  // namespace kEn
