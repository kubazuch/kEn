#pragma once

/**
 * @file model.hpp
 * @brief Multi-mesh 3D model loaded from a file via Assimp.
 * @ingroup ken
 */

#include <filesystem>
#include <span>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <kEn/core/core.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/scene/assets/mesh.hpp>

// NOLINTBEGIN(readability-identifier-naming)
struct aiNode;
struct aiScene;
// NOLINTEND(readability-identifier-naming)

namespace kEn {

/**
 * @brief A collection of @ref Mesh objects loaded from a single asset file.
 *
 * Models are loaded via Assimp and split at construction into two ordered
 * vectors: @ref opaque_meshes_ for fully opaque surfaces and
 * @ref transparent_meshes_ for surfaces that carry an opacity map.  Opaque
 * meshes are rendered before transparent ones; proper back-to-front depth
 * sorting of transparent meshes is not yet implemented (see MESH-08).
 *
 * Tangent space is sourced from the file when present.  When the file does
 * not embed tangents, MikkTSpace is run as a fallback.
 *
 * Use the @ref load factory to share a single @c Model instance across
 * multiple consumers.  The per-path cache is keyed on the path passed to
 * @ref load, so the same relative path always returns the same object.
 *
 * @note Copy and move are deleted; models are always shared via
 *       @c std::shared_ptr.
 */
class Model {
 public:
  /**
   * @brief Load and process an asset file synchronously.
   *
   * Resolves @p path relative to @ref kModelPath, imports the scene through
   * Assimp, generates MikkTSpace tangents for any mesh that lacks embedded
   * tangents, and uploads all geometry to immutable GPU buffers.
   *
   * @param path      Path relative to @c assets/models/.
   * @param sampler   Sampler description forwarded to each texture load.
   * @param flip_uvs  Pass @c true for assets whose UVs are vertically flipped
   *                  (e.g. OBJ files exported from certain tools).
   * @throws std::runtime_error if Assimp cannot open or parse the file.
   */
  explicit Model(const std::filesystem::path& path, const SamplerDesc& sampler = {}, bool flip_uvs = false) {
    load_model(kModelPath / path, sampler, flip_uvs);
  }

  DELETE_COPY_MOVE(Model);

  /**
   * @brief Render all opaque meshes followed by all transparent meshes.
   *
   * @param shader     The active shader program.
   * @param transform  World transform of this model.
   */
  void render(Shader& shader, const Transform& transform) const;

  /**
   * @brief Return a read-only view of the opaque mesh list.
   *
   * The span is valid as long as this @c Model is alive and unmodified.
   */
  [[nodiscard]] std::span<const Mesh> opaque_meshes() const noexcept;
  [[nodiscard]] std::span<Mesh> opaque_meshes() noexcept;

  /**
   * @brief Return a read-only view of the transparent mesh list.
   *
   * The span is valid as long as this @c Model is alive and unmodified.
   */
  [[nodiscard]] std::span<const Mesh> transparent_meshes() const noexcept;
  [[nodiscard]] std::span<Mesh> transparent_meshes() noexcept;

  /**
   * @brief Return a cached @c Model, loading it on first access.
   *
   * Looks up @p path in a process-wide cache.  On a cache miss the model is
   * constructed and inserted before being returned.  A failed construction
   * does not pollute the cache.
   *
   * @param path      Path relative to @c assets/models/.
   * @param sampler   Sampler description forwarded on a cache miss.
   * @param flip_uvs  UV-flip flag forwarded on a cache miss.
   * @return Shared ownership of the loaded model.
   * @throws std::runtime_error (forwarded from the constructor) on load failure.
   */
  static std::shared_ptr<Model> load(const std::filesystem::path& path, const SamplerDesc& sampler = {},
                                     bool flip_uvs = false);

  /** @brief Root directory for all model assets. */
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
