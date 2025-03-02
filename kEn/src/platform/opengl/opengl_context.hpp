#pragma once
#include <kEn/renderer/graphics_context.hpp>
#include <platform/win/win_window.hpp>

namespace kEn {
class opengl_context final : public graphics_context {
 public:
  opengl_context(void* window);

  void init() override;
  void swap_buffers() override;

 private:
  GLFWwindow* window_ptr_;
};
}  // namespace kEn
