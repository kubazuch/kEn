#pragma once

#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <kEn/core/transform.hpp>
#include <kEn/renderer/material.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/renderer/vertex_input.hpp>
#include <kEn/scene/mesh/vertex.hpp>

namespace kEn {

class Mesh {
 public:
  std::string name;
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
  kEn::Material material;

  Mesh(std::string_view name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
       kEn::Material material);
  void render(Shader& shader, const Transform& transform) const;

  void imgui();

  static constexpr auto kVertexLayout = make_vertex_layout<Vertex>({
      {shader_data_type::Float3, "a_Position", offsetof(Vertex, pos)},
      {shader_data_type::Float3, "a_Normal", offsetof(Vertex, normal)},
      {shader_data_type::Float2, "a_TexCoord", offsetof(Vertex, texture_coord)},
  });

  static const std::filesystem::path kModelPath;

 private:
  std::unique_ptr<VertexInput> vao_;
  void setup_mesh();
};

}  // namespace kEn
