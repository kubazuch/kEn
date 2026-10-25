#include "opengl_fence.hpp"

#include <glad/gl.h>

#include <kEn/core/assert.hpp>
#include <kEn/core/profile.hpp>

namespace kEn {

namespace {

constexpr GLuint64 kWaitStep = 1'000'000;  // ns

}  // namespace

OpenglFence::OpenglFence() : sync_(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0)) {
  KEN_CORE_ASSERT(sync_ != nullptr, "glFenceSync failed");
}

OpenglFence::~OpenglFence() { glDeleteSync(sync_); }

bool OpenglFence::signaled() {
  if (!signaled_) {
    const GLenum status = glClientWaitSync(sync_, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
    signaled_           = status == GL_ALREADY_SIGNALED || status == GL_CONDITION_SATISFIED;
  }
  return signaled_;
}

void OpenglFence::wait() {
  KEN_PROFILE_FUNCTION();
  while (!signaled_) {
    const GLenum status = glClientWaitSync(sync_, GL_SYNC_FLUSH_COMMANDS_BIT, kWaitStep);
    KEN_CORE_ASSERT(status != GL_WAIT_FAILED, "glClientWaitSync failed");
    signaled_ = status == GL_ALREADY_SIGNALED || status == GL_CONDITION_SATISFIED;
  }
}

}  // namespace kEn
