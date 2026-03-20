#pragma once

#include <deque>
#include <unordered_map>

#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/scene/mesh/mesh.hpp>

// NOLINTBEGIN(readability-identifier-naming)
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
// NOLINTEND(readability-identifier-naming)

namespace kEn {

class Model {
 public:
  explicit Model(const std::filesystem::path& path, const TextureSpec& spec = TextureSpec(), bool flip_uvs = false) {
    load_model(kModelPath / path, spec, flip_uvs);
  }

  void render(Shader& shader, const Transform& transform) const;

  std::deque<Mesh> meshes_;

  void imgui();

 private:
  std::filesystem::path directory_;

  void load_model(const std::filesystem::path& path, const TextureSpec& spec, bool flip_uvs);
  void process_node(aiNode* node, const aiScene* scene, const TextureSpec& spec);
  Mesh process_mesh(aiMesh* mesh, const aiScene* scene, TextureSpec spec);
  void load_material_textures(aiMaterial* mat, TextureType type, kEn::Material& material,
                              const TextureSpec& spec) const;

 public:
  static const std::filesystem::path kModelPath;

  static std::shared_ptr<Model> load(const std::filesystem::path& path, const TextureSpec& spec = TextureSpec(),
                                     bool flip_uvs = false);

 private:
  static std::unordered_map<std::filesystem::path, std::shared_ptr<Model>> loaded_resources_;
};

}  // namespace kEn
