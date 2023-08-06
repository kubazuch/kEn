#include "kenpch.h"
#include "shader.h"

#include "renderer.h"
#include "kEn/core/assert.h"
#include "platform/opengl/opengl_shader.h"

namespace kEn
{
	std::unique_ptr<shader> shader::create(const std::string& name, const std::string& vertex_src,
		const std::string& fragment_src)
	{
		switch (renderer::get_api())
		{
		case renderer_api::none:
			KEN_CORE_ASSERT(false, "Renderer has no api!");
			return nullptr;
		case renderer_api::opengl:
			return std::make_unique<opengl_shader>(name, vertex_src, fragment_src);
		}

		KEN_CORE_ASSERT(false, "Unknown api!");
		return nullptr;
	}
}
