#include "kenpch.h"
#include "buffer.h"

#include "renderer_api.h"
#include "kEn/core/assert.h"
#include "platform/opengl/opengl_buffer.h"

namespace kEn
{
	std::shared_ptr<vertex_buffer> vertex_buffer::create(float* vertices, uint32_t size)
	{
		switch(renderer_api::get_api())
		{
		case renderer_api::api:: none:
			KEN_CORE_ASSERT(false, "Renderer has no api!");
			return nullptr;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_vertex_buffer>(vertices, size);
		}

		KEN_CORE_ASSERT(false, "Unknown api!");
		return nullptr;
	}

	std::shared_ptr<mutable_vertex_buffer> mutable_vertex_buffer::create(float* vertices, uint32_t size)
	{
		switch (renderer_api::get_api())
		{
		case renderer_api::api::none:
			KEN_CORE_ASSERT(false, "Renderer has no api!");
			return nullptr;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_mutable_vertex_buffer>(vertices, size);
		}

		KEN_CORE_ASSERT(false, "Unknown api!");
		return nullptr;
	}

	std::shared_ptr<index_buffer> index_buffer::create(uint32_t* indices, uint32_t size)
	{
		switch (renderer_api::get_api())
		{
		case renderer_api::api::none:
			KEN_CORE_ASSERT(false, "Renderer has no api!");
			return nullptr;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_index_buffer>(indices, size);
		}

		KEN_CORE_ASSERT(false, "Unknown api!");
		return nullptr;
	}
}
