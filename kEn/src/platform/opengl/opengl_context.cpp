#include "opengl_context.hpp"

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <kEn/core/assert.hpp>

namespace kEn {

OpenglContext::OpenglContext(void* window) : window_ptr_(static_cast<GLFWwindow*>(window)) {
  KEN_CORE_ASSERT(window, "Window handle is null");
}

void OpenglContext::init() {
  glfwMakeContextCurrent(window_ptr_);
  const int glad_version = gladLoadGL(glfwGetProcAddress);
  KEN_CORE_ASSERT(glad_version, "Unable to init Glad");

  KEN_CORE_INFO("OpenGL Renderer:");
  KEN_CORE_INFO("\tVendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
  KEN_CORE_INFO("\tRenderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
  KEN_CORE_INFO("\tVersion: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
}

void OpenglContext::swap_buffers() { glfwSwapBuffers(window_ptr_); }

}  // namespace kEn