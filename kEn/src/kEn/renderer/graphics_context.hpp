#pragma once

#include <memory>

#include <kEn/core/core.hpp>

struct GLFWwindow;

namespace kEn {

class GraphicsContext {
 public:
  virtual ~GraphicsContext() = default;

  virtual void init()         = 0;
  virtual void swap_buffers() = 0;

  static std::unique_ptr<GraphicsContext> create(GLFWwindow* native_window);
};

}  // namespace kEn
