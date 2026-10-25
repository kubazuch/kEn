#pragma once

#include <glad/gl.h>

#include <kEn/renderer/fence.hpp>

namespace kEn {

/** @brief OpenGL sync object (@c glFenceSync) inserted at construction. */
class OpenglFence final : public Fence {
 public:
  OpenglFence();
  OpenglFence(const OpenglFence&)            = delete;
  OpenglFence& operator=(const OpenglFence&) = delete;
  OpenglFence(OpenglFence&&)                 = delete;
  OpenglFence& operator=(OpenglFence&&)      = delete;
  ~OpenglFence() override;

  [[nodiscard]] bool signaled() override;
  void wait() override;

 private:
  GLsync sync_;
  bool signaled_ = false;
};

}  // namespace kEn
