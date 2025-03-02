#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <kEn/core/assert.hpp>
#include <kenpch.hpp>
#include <platform/opengl/opengl_context.hpp>

kEn::opengl_context::opengl_context(void* window) : window_ptr_(static_cast<GLFWwindow*>(window)) {
  KEN_CORE_ASSERT(window, "Window handle is null");
}

void kEn::opengl_context::init() {
  glfwMakeContextCurrent(window_ptr_);
  const int glad_version = gladLoadGL(glfwGetProcAddress);
  KEN_CORE_ASSERT(glad_version, "Unable to init Glad");

  KEN_CORE_INFO("OpenGL Renderer:");
  KEN_CORE_INFO("\tVendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
  KEN_CORE_INFO("\tRenderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
  KEN_CORE_INFO("\tVersion: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
}

void kEn::opengl_context::swap_buffers() { glfwSwapBuffers(window_ptr_); }
