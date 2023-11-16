#include "kenpch.h"
#include "framebuffer.h"

#include "renderer_api.h"
#include "platform/opengl/opengl_framebuffer.h"

namespace kEn
{
	std::shared_ptr<framebuffer> framebuffer::create(const framebuffer_spec& spec)
	{
		switch (renderer_api::get_api())
		{
		case renderer_api::api::none:
			KEN_CORE_ASSERT(false, "Renderer has no api!");
			return nullptr;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_framebuffer>(spec);
		}

		KEN_CORE_ASSERT(false, "Unknown api!");
		return nullptr;
	}
}
