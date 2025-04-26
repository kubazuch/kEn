#include <imgui/imgui.h>

#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/material.hpp>
#include <kEn/renderer/render_command.hpp>
#include <kEn/renderer/renderer.hpp>
#include <kEn/scene/mesh/mesh.hpp>
#include <kenpch.hpp>
#include <utility>

namespace kEn {

BufferLayout Mesh::vertex_layout_ = {{shader_data_types::float3, "a_Position"},
                                     {shader_data_types::float3, "a_Normal"},
                                     {shader_data_types::float2, "a_TexCoord"}};

Mesh::Mesh(std::string_view name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
           kEn::Material material)
    : name(name), vertices(vertices), indices(indices), material(std::move(material)) {
  setup_mesh();
}

void Mesh::setup_mesh() {
  vao_ = VertexArray::create();

  std::shared_ptr<Buffer> vbo = Buffer::create(this->vertices.data(), vertices.size() * sizeof(Vertex));
  vbo->set_layout(vertex_layout_);
  vao_->add_vertex_buffer(vbo);

  std::shared_ptr<Buffer> ebo = Buffer::create(indices.data(), indices.size() * sizeof(uint32_t));
  vao_->set_index_buffer(ebo);
}

void Mesh::render(Shader& shader, const Transform& transform) const {
  shader.set_material("u_Material", material);
  material.bind();
  Renderer::submit(shader, *vao_, transform);
}

void Mesh::imgui() {
  if (ImGui::CollapsingHeader(name.c_str())) {
    material.imgui();
  }
}

}  // namespace kEn
