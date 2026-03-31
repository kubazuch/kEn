#include "mesh.hpp"

#include <imgui/imgui.h>

#include <cstdint>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include <kEn/core/application.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/imgui/material.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/material.hpp>
#include <kEn/renderer/renderer.hpp>
#include <kEn/renderer/vertex_input.hpp>
#include <kEn/scene/mesh/vertex.hpp>

namespace kEn {

Mesh::Mesh(std::string_view name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
           kEn::Material material)
    : name(name), vertices(vertices), indices(indices), material(std::move(material)) {
  setup_mesh();
}

void Mesh::setup_mesh() {
  auto& dev = device();
  vao_      = dev.create_vertex_input();

  const std::shared_ptr<Buffer> vbo = dev.create_buffer(
      {.size = vertices.size() * sizeof(Vertex), .usage = BufferUsage::Immutable, .bind_flags = BufferBind::Vertex},
      vertices.data());
  vao_->add_vertex_stream({.buffer = vbo, .layout = kVertexLayout});

  const std::shared_ptr<Buffer> ebo = dev.create_buffer(
      {.size = indices.size() * sizeof(uint32_t), .usage = BufferUsage::Immutable, .bind_flags = BufferBind::Index},
      indices.data());
  vao_->set_index_buffer(ebo);
}

void Mesh::render(Shader& shader, const Transform& transform) const {
  material.apply(shader, device().context());
  Renderer::submit(shader, *vao_, transform);
}

void Mesh::imgui() {
  if (ImGui::CollapsingHeader(name.c_str())) {
    ui::Material(material);
  }
}

}  // namespace kEn
