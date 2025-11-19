#pragma once

#include <memory>

namespace kEn {  // NOLINT

class GraphicsContext {
 public:
  virtual ~GraphicsContext() = default;

  virtual void init()         = 0;
  virtual void swap_buffers() = 0;

  static std::unique_ptr<GraphicsContext> create(void* native_window);
};

}  // namespace kEn
