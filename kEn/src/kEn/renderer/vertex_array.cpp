#include "kenpch.h"
#include "vertex_array.h"

#include "renderer.h"
#include "platform/opengl/opengl_vertex_array.h"

namespace kEn
{
	std::unique_ptr<vertex_array> vertex_array::create()
	{
		switch (renderer::get_api())
		{
		case renderer_api::none:
			KEN_CORE_ASSERT(false, "Renderer has no api!");
			return nullptr;
		case renderer_api::opengl:
			return std::make_unique<opengl_vertex_array>();
		}

		KEN_CORE_ASSERT(false, "Unknown api!");
		return nullptr;
	}
}
