#pragma once

#include <memory>
#include <string_view>
#include <unordered_map>

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
enum class TextureType : std::uint8_t { AmbientOcclusion, Diffuse, Height, Normal, Specular, Count };

namespace texture_type {

/** @brief C++20 convenience: brings enumerators into `kEn::texture_type` namespace. */
using enum TextureType;

namespace detail {

inline constexpr util::EnumMap kNames{{
    std::pair{AmbientOcclusion, "ambient_occlusion"},
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
 * surface flags. Call @ref load to upload uniforms, then @ref bind to activate
 * texture units before drawing.
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
   * @return Const reference to the stored shared pointer.
   * @throws std::out_of_range if @p type or @p id is not present.
   */
  const std::shared_ptr<Texture>& texture(TextureType type, std::size_t id = 0) const;

  /**
   * @brief Upload Phong uniforms and texture-unit indices to @p shader.
   *
   * Sets `<name>.ka/kd/ks/m/emissive/surface_color` and one sampler uniform
   * per stored texture. Texture units are assigned in iteration order of the
   * internal map; call @ref bind afterwards to activate the same units.
   *
   * @param shader Target shader program.
   * @param name   GLSL struct name prefix (default @c "u_Material").
   */
  void load(Shader& shader, std::string_view name = "u_Material") const;

  /**
   * @brief Bind all stored textures to consecutive texture units via @p context.
   *
   * Textures are bound to ShaderStage::Fragment slots in the same order used by @ref load.
   *
   * @param context The active RenderContext to route texture binding through.
   */
  void bind(RenderContext& context) const;

  /** @brief Render an ImGui inspector panel for this material. */
  void imgui();

  float ambient_factor   = 0.5F;
  float diffuse_factor   = 0.5F;
  float specular_factor  = 0.5F;
  float shininess_factor = 50.F;

  mEn::Vec3 surface_color = mEn::Vec3(1.F, 1.F, 1.F);

  bool transparent = false;
  bool emissive    = false;

 private:
  friend class Shader;

  std::unordered_map<TextureType, std::vector<std::shared_ptr<kEn::Texture>>> textures_;
};

}  // namespace kEn
