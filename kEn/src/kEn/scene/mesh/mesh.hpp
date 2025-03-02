#pragma once
#include <kEn/core/transform.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/renderer/vertex_array.hpp>
#include <kEn/scene/mesh/vertex.hpp>

namespace kEn {

class mesh {
 public:
  const std::string name;
  std::vector<vertex> vertices;
  std::vector<uint32_t> indices;
  kEn::material material;

  mesh(const std::string& name, const std::vector<vertex>& vertices, const std::vector<uint32_t>& indices,
       kEn::material material);
  void render(shader& shader, const transform& transform) const;

  void imgui();

 private:
  std::unique_ptr<vertex_array> vao_;
  void setup_mesh();

 public:
  static buffer_layout vertex_layout;
  static const std::filesystem::path model_path;
};

}  // namespace kEn
