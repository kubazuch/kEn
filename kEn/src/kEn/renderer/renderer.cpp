#include "kenpch.h"
#include "renderer.h"

#include "render_command.h"

namespace kEn
{
	void renderer::begin_scene()
	{
	}

	void renderer::end_scene()
	{
	}

	void renderer::submit(const std::shared_ptr<vertex_array>& vertex_array)
	{
		render_command::draw_indexed(vertex_array);
	}
}
