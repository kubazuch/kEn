#include "kenpch.h"
#include "mesh.h"

#include <map>

#include "kEn/renderer/renderer.h"
#include "kEn/renderer/render_command.h"

namespace kEn
{
	buffer_layout mesh::vertex_layout = {
		{shader_data_types::float3, "a_Position"},
		{shader_data_types::float3, "a_Normal"},
		{shader_data_types::float2, "a_TexCoord"}
	};

	mesh::mesh(const std::vector<vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<std::shared_ptr<texture2D>>& textures)
		: vertices(vertices), indices(indices), textures(textures)
	{
		setup_mesh();
	}

	void mesh::setup_mesh()
	{
		vao_ = vertex_array::create();

		std::shared_ptr<vertex_buffer> vbo = vertex_buffer::create(this->vertices.data(), vertices.size() * sizeof(vertex));
		vbo->set_layout(vertex_layout);
		vao_->add_vertex_buffer(vbo);

		std::shared_ptr<index_buffer> ebo = index_buffer::create(indices.data(), indices.size());
		vao_->set_index_buffer(ebo);
	}

	void mesh::render(shader& shader)
	{
		std::map<texture_type_t, unsigned int> ids;

		shader.bind();
		for(int i = 0; i < textures.size(); i++)
		{
			texture_type_t type = textures[i]->type();
			unsigned int id = ids[type]++;
			shader.set_int("u_Material." + std::string(texture_type::name_of(type)) + std::to_string(id), i);
			textures[i]->bind(i);
		}

		vao_->bind();
		transform trans;
		trans.set_scale(glm::vec3(0.01f));
		trans.set_pos(glm::vec3(0, -1, 0));
		renderer::submit(shader, *vao_, trans);
		vao_->unbind();
	}

}
