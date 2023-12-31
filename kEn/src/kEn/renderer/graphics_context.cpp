#include "kenpch.h"
#include "graphics_context.h"

#include "renderer_api.h"
#include "kEn/core/assert.h"
#include "platform/opengl/opengl_context.h"

namespace kEn
{
	std::unique_ptr<graphics_context> graphics_context::create(void* native_window)
	{
		switch (renderer_api::get_api())
		{
		case renderer_api::api::none:
			KEN_CORE_ASSERT(false, "Renderer has no api!");
			return nullptr;
		case renderer_api::api::opengl:
			return std::make_unique<opengl_context>(static_cast<GLFWwindow*>(native_window));
		}

		KEN_CORE_ASSERT(false, "Unknown api!");
		return nullptr;
	}
}
