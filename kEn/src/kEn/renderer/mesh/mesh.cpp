#include "kenpch.h"
#include "mesh.h"

#include <map>
#include <utility>

#include "imgui.h"
#include "kEn/renderer/renderer.h"
#include "kEn/renderer/render_command.h"

namespace kEn
{
	buffer_layout mesh::vertex_layout = {
		{shader_data_types::float3, "a_Position"},
		{shader_data_types::float3, "a_Normal"},
		{shader_data_types::float2, "a_TexCoord"}
	};

	mesh::mesh(const std::string& name, const std::vector<vertex>& vertices, const std::vector<uint32_t>& indices, kEn::material material)
		: name(name), vertices(vertices), indices(indices), material(std::move(material))
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

	void mesh::render(shader& shader, const transform& transform) const
	{
		shader.set_material("u_Material", material);
		material.bind();
		renderer::submit(shader, *vao_, transform);
	}

	void mesh::imgui()
	{
		if(ImGui::CollapsingHeader(name.c_str()))
		{
			material.imgui();
		}
	}
}
