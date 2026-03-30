#pragma once

#include <kEn/core/core.hpp>

struct GLFWwindow;

namespace kEn {

class OpenglSwapChain final {
 public:
  explicit OpenglSwapChain(GLFWwindow* window);

  void init();
  void swap_buffers();

 private:
  GLFWwindow* window_ptr_;  // non-owning; lifetime governed by the Window that constructed this context
};

}  // namespace kEn
