#pragma once

#include <kEn/core/core.hpp>

struct GLFWwindow;

namespace kEn {

class OpenglContext final {
 public:
  explicit OpenglContext(GLFWwindow* window);

  void init();
  void swap_buffers();

 private:
  GLFWwindow* window_ptr_;  // non-owning; lifetime governed by the Window that constructed this context
};

}  // namespace kEn
