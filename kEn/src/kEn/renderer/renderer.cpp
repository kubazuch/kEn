#include "kenpch.h"
#include "renderer.h"

#include "render_command.h"

namespace kEn
{
	std::unique_ptr<renderer::scene_data> renderer::scene_data_ = std::make_unique<scene_data>();

	void renderer::begin_scene(std::shared_ptr<camera> camera)
	{
		scene_data_->VP_matrix = camera->view_projection_matrix();
	}

	void renderer::end_scene()
	{
	}

	void renderer::submit(shader& shader, const vertex_array& vertex_array, const transform& transform)
	{
		shader.bind();
		shader.set_mat4("u_VP", scene_data_->VP_matrix);
		shader.set_mat4("u_M", transform.local_to_world_matrix());

		render_command::draw_indexed(vertex_array);
	}

	void renderer::submit_tessellated(shader& shader, const vertex_array& vertex_array, const uint32_t& count, const transform& transform)
	{
		shader.bind();
		shader.set_mat4("u_VP", scene_data_->VP_matrix);
		shader.set_mat4("u_M", transform.local_to_world_matrix());

		render_command::draw_patches(vertex_array, count);
	}
}
