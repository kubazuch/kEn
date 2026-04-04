#pragma once

#include <filesystem>
#include <span>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <kEn/core/core.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/scene/mesh/mesh.hpp>

// NOLINTBEGIN(readability-identifier-naming)
struct aiNode;
struct aiScene;
// NOLINTEND(readability-identifier-naming)

namespace kEn {

class Model {
 public:
  explicit Model(const std::filesystem::path& path, const SamplerDesc& sampler = {}, bool flip_uvs = false) {
    load_model(kModelPath / path, sampler, flip_uvs);
  }

  DELETE_COPY_MOVE(Model);

  void render(Shader& shader, const Transform& transform) const;

  [[nodiscard]] std::span<const Mesh> opaque_meshes() const noexcept;
  [[nodiscard]] std::span<Mesh> opaque_meshes() noexcept;
  [[nodiscard]] std::span<const Mesh> transparent_meshes() const noexcept;
  [[nodiscard]] std::span<Mesh> transparent_meshes() noexcept;
  static std::shared_ptr<Model> load(const std::filesystem::path& path, const SamplerDesc& sampler = {},
                                     bool flip_uvs = false);

  static constexpr std::string_view kModelPath = "assets/models";

 private:
  void load_model(const std::filesystem::path& path, const SamplerDesc& sampler, bool flip_uvs);
  void process_node(aiNode* node, const aiScene* scene, const SamplerDesc& sampler,
                    const std::filesystem::path& directory);

  // TODO(kuzu): move towards central asset manager
  static std::unordered_map<std::filesystem::path, std::shared_ptr<Model>> loaded_resources_;

  std::vector<Mesh> opaque_meshes_;
  std::vector<Mesh> transparent_meshes_;
};

}  // namespace kEn
