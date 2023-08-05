#include "kenpch.h"
#include "opengl_context.h"
#include "kEn/core/assert.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gl/GL.h>

kEn::opengl_context::opengl_context(GLFWwindow* window)
	: window_ptr_(window)
{
	KEN_CORE_ASSERT(window, "Window handle is null");
}

void kEn::opengl_context::init()
{
	glfwMakeContextCurrent(window_ptr_);
	const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	KEN_CORE_ASSERT(status, "Unable to init Glad");
	
	KEN_CORE_INFO("OpenGL Renderer:");
	KEN_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	KEN_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	KEN_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
}

void kEn::opengl_context::swap_buffers()
{
	glfwSwapBuffers(window_ptr_);
}
