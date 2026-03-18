#pragma once

#include <kEn/core/core.hpp>

namespace kEn {

// RAII guard for a single Dear ImGui render frame.
// Constructor starts the frame (NewFrame + BeginFrame); destructor finalizes and renders it.
// Exactly one ImguiFrame must be in scope while on_imgui() callbacks are being driven.
class ImguiFrame {
 public:
  ImguiFrame();
  ~ImguiFrame();

  ImguiFrame(const ImguiFrame&)            = delete;
  ImguiFrame& operator=(const ImguiFrame&) = delete;
  ImguiFrame(ImguiFrame&&)                 = delete;
  ImguiFrame& operator=(ImguiFrame&&)      = delete;
};

}  // namespace kEn
