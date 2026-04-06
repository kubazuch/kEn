#pragma once

/**
 * @file mesh.hpp
 * @brief GPU mesh primitive and its vertex layout.
 * @ingroup ken
 */

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

/**
 * @brief Per-vertex data uploaded to the GPU.
 *
 * The tangent is stored as a @c Vec4: @c xyz holds the tangent direction and
 * @c w holds the bitangent handedness sign (+1 or -1).  The bitangent is
 * reconstructed in the vertex shader as @c cross(N, T) * sign.
 */
struct Vertex {
  mEn::Vec3 pos;            ///< Object-space position.
  mEn::Vec3 normal;         ///< Object-space normal (unit length).
  mEn::Vec4 tangent;        ///< xyz = tangent (unit), w = bitangent sign.
  mEn::Vec2 texture_coord;  ///< UV coordinates.
};

/**
 * @brief A single draw call unit: a VAO plus a @ref Material.
 *
 * Vertex and index data are uploaded to immutable GPU buffers at construction
 * and the CPU copies are discarded immediately.  The mesh is therefore
 * move-only; copying is deleted because the VAO cannot be duplicated cheaply.
 *
 * @ref render applies the material uniforms and issues a single indexed draw
 * call via @ref Renderer::submit.
 */
class Mesh {
 public:
  std::string name;        ///< Debug name from the source asset.
  kEn::Material material;  ///< Surface description applied before each draw.

  /**
   * @brief Upload vertex and index data to immutable GPU buffers.
   *
   * @param name      Debug name (stored as-is).
   * @param vertices  Vertex data; copied into a GPU buffer and then discarded.
   * @param indices   Index data; copied into a GPU buffer and then discarded.
   * @param material  Surface material moved into the mesh.
   */
  Mesh(std::string_view name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
       kEn::Material material);

  Mesh(const Mesh&)            = delete;
  Mesh& operator=(const Mesh&) = delete;
  Mesh(Mesh&&)                 = default;
  Mesh& operator=(Mesh&&)      = default;

  /**
   * @brief Apply the material and issue an indexed draw call.
   *
   * @param shader     The active shader program.
   * @param transform  World transform uploaded as model matrix uniforms.
   */
  void render(Shader& shader, const Transform& transform) const;

  /**
   * @brief Compile-time vertex attribute layout for VAO setup.
   *
   * Attributes: @c a_Position (Float3), @c a_Normal (Float3),
   * @c a_Tangent (Float4), @c a_TexCoord (Float2).
   */
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
