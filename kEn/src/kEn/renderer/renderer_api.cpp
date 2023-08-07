#include "kenpch.h"
#include "renderer_api.h"

#include "vertex_array.h"

#include "platform/opengl/opengl_renderer_api.h"

namespace kEn
{
	std::unique_ptr<renderer_api> renderer_api::create()
	{
		switch (get_api())
		{
		case api::none:
			KEN_CORE_ASSERT(false, "Renderer has no api!");
			return nullptr;
		case api::opengl:
			return std::make_unique<opengl_renderer_api>();
		}

		KEN_CORE_ASSERT(false, "Unknown api!");
		return nullptr;
	}
}
