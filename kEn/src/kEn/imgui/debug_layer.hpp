#pragma once

#include <kEn/core/layer.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * Internal engine overlay that renders a debug panel via Dear ImGui.
 *
 * Shows FPS, TPS, and a VSync toggle applied synchronously.
 * Registered automatically by Application in debug builds.
 */
class DebugLayer final : public Layer {
 public:
  DebugLayer();

  /** Renders the DEBUG panel: FPS, TPS, and a VSync checkbox. */
  void on_imgui() override;

  DELETE_COPY_MOVE(DebugLayer);
};

}  // namespace kEn