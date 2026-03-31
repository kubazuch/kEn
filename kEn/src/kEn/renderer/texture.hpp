#pragma once

#include <imgui/imgui.h>

#include <algorithm>
#include <bit>
#include <cstdint>
#include <span>

#include <kEn/core/core.hpp>
#include <kEn/renderer/render_context.hpp>
#include <kEn/renderer/texture_format.hpp>
#include <kEn/util/enum_map.hpp>

/** @file
 *  @ingroup ken
 *  @brief Abstract texture interface, sampler descriptor, and texture descriptor types.
 */

namespace kEn {

/**
 * @brief Describes how a texture is sampled: filtering, wrap modes, and optional depth comparison.
 *
 * Pass alongside a @ref TextureDesc when constructing a backend texture object.
 * The sampler state is applied once at construction and is immutable thereafter.
 */
struct SamplerDesc {
  /** @brief Minification and magnification filter mode. */
  enum class Filter : std::uint8_t {
    Nearest, /**< @brief Nearest-neighbour (point) filtering. */
    Linear,  /**< @brief Bilinear filtering. */
  };
  /** @brief Mip-level transition filter.  @c None disables mipmapping entirely. */
  enum class MipFilter : std::uint8_t {
    None,    /**< @brief Mipmapping disabled; only the base level is sampled. */
    Nearest, /**< @brief Snap to the nearest mip level. */
    Linear,  /**< @brief Linearly interpolate between adjacent mip levels. */
  };
  /** @brief Texture coordinate wrap mode applied outside the [0, 1] range. */
  enum class Wrap : std::uint8_t {
    Repeat,         /**< @brief Repeat the texture. */
    Clamp,          /**< @brief Clamp to the edge texel. */
    MirroredRepeat, /**< @brief Mirror the texture on each repeat. */
  };

  Filter min_filter    = Filter::Linear;  /**< @brief Filter applied when the texture is minified. */
  Filter mag_filter    = Filter::Linear;  /**< @brief Filter applied when the texture is magnified. */
  MipFilter mip_filter = MipFilter::None; /**< @brief Mip-level transition filter. */

  Wrap wrap_u = Wrap::Repeat; /**< @brief Wrap mode along the U (S) axis. */
  Wrap wrap_v = Wrap::Repeat; /**< @brief Wrap mode along the V (T) axis. */
  Wrap wrap_w = Wrap::Repeat; /**< @brief Wrap mode along the W (R) axis (3D textures only). */

  bool compare_enabled = false; /**< @brief Enables depth comparison mode (e.g. for shadow maps). */
  CompareOp compare_op =
      CompareOp::LessEqual; /**< @brief Comparison function used when @ref compare_enabled is true. */
};

/** @brief Sentinel value for @ref TextureDesc::mip_levels requesting a full mip chain down to 1x1. */
inline constexpr std::uint32_t kFullMipChain = 0;

/** @brief Dimensionality and topology of a texture resource. */
enum class TextureKind : std::uint8_t {
  Tex2D,      /**< @brief Standard 2D texture. */
  Tex2DArray, /**< @brief Array of 2D texture layers. */
  Tex3D,      /**< @brief Volumetric 3D texture. */
  Cube,       /**< @brief Cube-map texture (6 faces). */
  CubeArray,  /**< @brief Array of cube-map textures. */
};

/**
 * @brief Immutable descriptor used to allocate a GPU texture resource.
 *
 * Describes the full shape of the texture: kind, dimensions, pixel format, and mip level count.
 * Pass this to a backend constructor or use the static factory helpers for common cases.
 *
 * @note Set @ref mip_levels to @ref kFullMipChain to let the backend compute the full chain depth.
 */
struct TextureDesc {
  TextureKind kind = TextureKind::Tex2D;

  std::uint32_t width  = 0;
  std::uint32_t height = 1;
  std::uint32_t depth  = 1;
  std::uint32_t layers = 1;  // For CubeArray, this is cube count.

  TextureFormat format     = TextureFormat::RGBA8;
  std::uint32_t mip_levels = 1;  // Use kFullMipChain for a full chain.

  /** @brief Maximum mip levels supported by the texture dimensions. */
  [[nodiscard]] constexpr std::uint32_t max_mip_levels() const noexcept {
    const std::uint32_t max_dim =
        (kind == TextureKind::Tex3D) ? std::max({width, height, depth}) : std::max(width, height);
    return static_cast<uint32_t>(std::bit_width(max_dim));
  }

  /** @brief Actual mip level count after resolving @ref kFullMipChain to the full chain depth. */
  [[nodiscard]] constexpr std::uint32_t resolved_mip_levels() const noexcept {
    return mip_levels == kFullMipChain ? max_mip_levels() : mip_levels;
  }

  /** @brief Total face count: 6 for Cube, 6 * layers for CubeArray, 0 for all other kinds. */
  [[nodiscard]] constexpr std::uint32_t cube_face_count() const noexcept {
    switch (kind) {
      case TextureKind::Cube:
        return 6U;
      case TextureKind::CubeArray:
        return 6U * layers;
      default:
        return 0;
    }
  }

  /** @brief Returns true if @ref mip_levels is either @ref kFullMipChain or does not exceed @ref max_mip_levels(). */
  [[nodiscard]] constexpr bool has_valid_mip_request() const noexcept {
    return mip_levels == kFullMipChain || mip_levels <= max_mip_levels();
  }

  /** @brief Creates a descriptor for a standard 2D texture. */
  [[nodiscard]] static constexpr TextureDesc texture_2d(std::uint32_t width, std::uint32_t height, TextureFormat format,
                                                        std::uint32_t mip_levels = 1) noexcept {
    return {
        .kind       = TextureKind::Tex2D,
        .width      = width,
        .height     = height,
        .format     = format,
        .mip_levels = mip_levels,
    };
  }

  /** @brief Creates a descriptor for a 2D texture array with the given layer count. */
  [[nodiscard]] static constexpr TextureDesc texture_2d_array(std::uint32_t width, std::uint32_t height,
                                                              std::uint32_t layers, TextureFormat format,
                                                              std::uint32_t mip_levels = 1) noexcept {
    return {
        .kind       = TextureKind::Tex2DArray,
        .width      = width,
        .height     = height,
        .layers     = layers,
        .format     = format,
        .mip_levels = mip_levels,
    };
  }

  /** @brief Creates a descriptor for a volumetric 3D texture. */
  [[nodiscard]] static constexpr TextureDesc texture_3d(std::uint32_t width, std::uint32_t height, std::uint32_t depth,
                                                        TextureFormat format, std::uint32_t mip_levels = 1) noexcept {
    return {
        .kind       = TextureKind::Tex3D,
        .width      = width,
        .height     = height,
        .depth      = depth,
        .format     = format,
        .mip_levels = mip_levels,
    };
  }

  /** @brief Creates a descriptor for a cube-map texture with equal width and height (@p size). */
  [[nodiscard]] static constexpr TextureDesc texture_cube(std::uint32_t size, TextureFormat format,
                                                          std::uint32_t mip_levels = 1) noexcept {
    return {
        .kind       = TextureKind::Cube,
        .width      = size,
        .height     = size,
        .format     = format,
        .mip_levels = mip_levels,
    };
  }

  /** @brief Creates a descriptor for an array of @p cube_count cube-map textures. */
  [[nodiscard]] static constexpr TextureDesc texture_cube_array(std::uint32_t size, std::uint32_t cube_count,
                                                                TextureFormat format,
                                                                std::uint32_t mip_levels = 1) noexcept {
    return {
        .kind       = TextureKind::CubeArray,
        .width      = size,
        .height     = size,
        .layers     = cube_count,
        .format     = format,
        .mip_levels = mip_levels,
    };
  }
};

/**
 * @brief Abstract GPU texture resource.
 *
 * Wraps an immutable GPU allocation described by a @ref TextureDesc.
 * Pixel data can be written after construction via @ref set_data().
 * Backend-specific subclasses own the underlying API object.
 */
class Texture {
 public:
  virtual ~Texture() = default;

  /** @brief Returns the immutable descriptor that describes this texture's shape and format. */
  [[nodiscard]] virtual const TextureDesc& desc() const = 0;
  /**
   * @brief Uploads pixel data to a specific mip level and array layer.
   *
   * @param data       Raw pixel bytes; size must exactly match the target mip level dimensions.
   * @param mip_level  Zero-based mip level index.
   * @param layer      Zero-based array layer (or cube face) index.
   */
  virtual void set_data(std::span<const std::byte> data, std::uint32_t mip_level, std::uint32_t layer) = 0;
  /** @brief Uploads pixel data to the given mip level of layer 0. */
  void set_data(std::span<const std::byte> data, std::uint32_t mip_level = 0) { set_data(data, mip_level, 0); }

  /** @brief Returns the texture width in texels. */
  [[nodiscard]] std::uint32_t width() const noexcept { return desc().width; }
  /** @brief Returns the texture height in texels. */
  [[nodiscard]] std::uint32_t height() const noexcept { return desc().height; }
  /** @brief Returns the texture depth in texels (1 for non-3D textures). */
  [[nodiscard]] std::uint32_t depth() const noexcept { return desc().depth; }
  /** @brief Returns the number of array layers (1 for non-array textures; cube count for CubeArray). */
  [[nodiscard]] std::uint32_t layers() const noexcept { return desc().layers; }
  /** @brief Returns the texture dimensionality/topology. */
  [[nodiscard]] TextureKind kind() const noexcept { return desc().kind; }
  /** @brief Returns the pixel/texel format. */
  [[nodiscard]] TextureFormat format() const noexcept { return desc().format; }
  /** @brief Returns the resolved mip level count (resolves @ref kFullMipChain to the actual chain depth). */
  [[nodiscard]] std::uint32_t mip_levels() const noexcept { return desc().resolved_mip_levels(); }

  /** @brief Return the platform-native GPU texture handle. */
  [[nodiscard]] virtual std::uintptr_t native_handle() const noexcept = 0;

  /** @brief Returns a handle suitable for passing to ImGui::Image(). */
  [[nodiscard]] virtual ImTextureID imgui_id() const noexcept = 0;
};

}  // namespace kEn
