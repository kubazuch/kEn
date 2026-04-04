#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <mEn/vec2.hpp>
#include <mEn/vec3.hpp>
#include <mEn/vec4.hpp>

#include <kEn/core/transform.hpp>
#include <kEn/renderer/material.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/vertex_input.hpp>

namespace kEn {

struct Vertex {
  mEn::Vec3 pos;
  mEn::Vec3 normal;
  mEn::Vec4 tangent;  // xyz = tangent, w = bitangent sign
  mEn::Vec2 texture_coord;
};

class Mesh {
 public:
  std::string name;
  kEn::Material material;

  Mesh(std::string_view name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
       kEn::Material material);

  Mesh(const Mesh&)            = delete;
  Mesh& operator=(const Mesh&) = delete;
  Mesh(Mesh&&)                 = default;
  Mesh& operator=(Mesh&&)      = default;

  void render(Shader& shader, const Transform& transform) const;

  static constexpr auto kVertexLayout = make_vertex_layout<Vertex>({
      {shader_data_type::Float3, "a_Position", offsetof(Vertex, pos)},
      {shader_data_type::Float3, "a_Normal", offsetof(Vertex, normal)},
      {shader_data_type::Float4, "a_Tangent", offsetof(Vertex, tangent)},
      {shader_data_type::Float2, "a_TexCoord", offsetof(Vertex, texture_coord)},
  });

 private:
  std::unique_ptr<VertexInput> vao_;
};

}  // namespace kEn
