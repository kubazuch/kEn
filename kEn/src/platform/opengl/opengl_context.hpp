#pragma once
#include <kEn/renderer/graphics_context.hpp>
#include <platform/win/win_window.hpp>

namespace kEn {
class OpenglContext final : public GraphicsContext {
 public:
  explicit OpenglContext(void* window);

  void init() override;
  void swap_buffers() override;

 private:
  GLFWwindow* window_ptr_;
};
}  // namespace kEn
