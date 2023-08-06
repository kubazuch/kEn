#include "kenpch.h"
#include "shader.h"

#include "platform/opengl/opengl_shader.h"

namespace kEn
{
	std::unique_ptr<shader> shader::create(const std::string& name, const std::string& vertex_src,
		const std::string& fragment_src)
	{
		//TODO: Switch(Renderer::API()) -- different context for different apis

		return std::make_unique<opengl_shader>(name, vertex_src, fragment_src);
	}
}
