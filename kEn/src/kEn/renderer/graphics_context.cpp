#include "kenpch.h"
#include "graphics_context.h"

#include "platform/opengl/opengl_context.h"

namespace kEn
{
	std::unique_ptr<graphics_context> graphics_context::create(void* native_window)
	{
		//TODO: Switch(Renderer::API()) -- different context for different apis

		return std::make_unique<opengl_context>(static_cast<GLFWwindow*>(native_window));
	}
}
