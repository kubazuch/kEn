#pragma once

#include <kEn/renderer/graphics_context.hpp>

struct GLFWwindow;

namespace kEn {
class OpenglContext final : public GraphicsContext {
 public:
  explicit OpenglContext(GLFWwindow* window);

  void init() override;
  void swap_buffers() override;

 private:
  GLFWwindow* window_ptr_;
};
}  // namespace kEn
