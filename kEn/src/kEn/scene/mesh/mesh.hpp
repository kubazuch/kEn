#pragma once

#include <kEn/core/transform.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/material.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/renderer/vertex_array.hpp>
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

 private:
  std::unique_ptr<VertexArray> vao_;
  void setup_mesh();

 public:
  static BufferLayout vertex_layout_;
  static const std::filesystem::path kModelPath;
};

}  // namespace kEn
