#include "kenpch.h"
#include "buffer.h"

#include "renderer.h"
#include "kEn/core/assert.h"
#include "platform/opengl/opengl_buffer.h"

namespace kEn
{
	std::unique_ptr<vertex_buffer> vertex_buffer::create(float* vertices, uint32_t size)
	{
		switch(renderer::get_api())
		{
		case renderer_api::none:
			KEN_CORE_ASSERT(false, "Renderer has no api!");
			return nullptr;
		case renderer_api::opengl:
			return std::make_unique<opengl_vertex_buffer>(vertices, size);
		}

		KEN_CORE_ASSERT(false, "Unknown api!");
		return nullptr;
	}

	std::unique_ptr<index_buffer> index_buffer::create(uint32_t* indices, uint32_t size)
	{
		switch (renderer::get_api())
		{
		case renderer_api::none:
			KEN_CORE_ASSERT(false, "Renderer has no api!");
			return nullptr;
		case renderer_api::opengl:
			return std::make_unique<opengl_index_buffer>(indices, size);
		}

		KEN_CORE_ASSERT(false, "Unknown api!");
		return nullptr;
	}
}
