#pragma once

#include <array>
#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>

#include <mEn.hpp>

#include <kEn/renderer/texture.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

class RenderContext;
class Shader;

/**
 * @brief Strongly-typed texture semantic category.
 */
enum class TextureType : std::uint8_t { Diffuse, Height, Normal, Specular, Count };

namespace texture_type {

/** @brief C++20 convenience: brings enumerators into `kEn::texture_type` namespace. */
using enum TextureType;

namespace detail {

inline constexpr util::EnumMap kNames{{
    std::pair{Diffuse, "diffuse"},
    std::pair{Height, "height"},
    std::pair{Normal, "normal"},
    std::pair{Specular, "specular"},
}};

}  // namespace detail

/**
 * @brief Get a stable, human-readable name for a texture type.
 * @param type Texture type enum value.
 * @return A string view describing the type (e.g. @c "diffuse").
 */
[[nodiscard]] constexpr std::string_view name_of(TextureType type) noexcept { return detail::kNames[type]; }

}  // namespace texture_type

/**
 * @brief Phong-shaded surface description passed to a @ref Shader.
 *
 * Stores per-type texture arrays alongside scalar Phong coefficients and
 * surface flags. Call @ref apply to upload uniforms and activate texture
 * units in a single step before drawing.
 */
class Material {
 public:
  Material() = default;

  /**
   * @brief Assign a texture to a slot within a given type array.
   *
   * Textures must be added in ascending @p id order; gaps are not allowed.
   * Assigning to an existing @p id replaces it; assigning to `id == size`
   * appends.
   *
   * @param type    Semantic category (e.g. @c TextureType::Diffuse).
   * @param texture Shared ownership of the texture to store.
   * @param id      Index within the type's array (default 0).
   * @throws std::runtime_error if @p id skips beyond the current array size.
   */
  void set_texture(TextureType type, std::shared_ptr<Texture> texture, std::size_t id = 0);

  /**
   * @brief Retrieve a texture by type and index.
   *
   * @param type Semantic category.
   * @param id   Index within the type's array (default 0).
   * @return Shared pointer to the stored texture.
   * @throws std::out_of_range if @p id is out of range for the given @p type.
   */
  [[nodiscard]] std::shared_ptr<Texture> texture(TextureType type, std::size_t id = 0) const;

  /**
   * @brief Return the number of textures stored for a given @p type.
   *
   * @param type Semantic category.
   * @return Number of textures in the type's array (0 if none have been set).
   */
  [[nodiscard]] std::size_t texture_count(TextureType type) const noexcept;

  /**
   * @brief Upload Phong uniforms and bind all textures to consecutive units.
   *
   * Uploads `<name>.ka/kd/ks/m/emissive/surface_color` and one sampler
   * uniform per stored texture, then activates each texture unit via
   * @p context. Texture units are assigned in @ref TextureType declaration
   * order (AmbientOcclusion first, Specular last).
   *
   * @param shader  Target shader program.
   * @param context The active RenderContext to route texture binding through.
   * @param name    GLSL struct name prefix (default @c "u_Material").
   */
  void apply(Shader& shader, RenderContext& context, std::string_view name = "u_Material") const;

  float ambient_factor   = 0.5F;
  float diffuse_factor   = 0.5F;
  float specular_factor  = 0.5F;
  float shininess_factor = 50.F;

  mEn::Vec3 surface_color = mEn::Vec3(1.F, 1.F, 1.F);

  bool transparent = false;
  bool emissive    = false;

 private:
  [[nodiscard]] static constexpr std::size_t to_index(TextureType type) noexcept {
    return static_cast<std::size_t>(type);
  }

  std::array<std::vector<std::shared_ptr<Texture>>, static_cast<std::size_t>(TextureType::Count)> textures_;
};

}  // namespace kEn
