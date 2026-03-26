#pragma once

#include <imgui/imgui.h>

#include <algorithm>
#include <bit>
#include <cstdint>
#include <span>

#include <kEn/core/core.hpp>
#include <kEn/renderer/texture_format.hpp>
#include <kEn/util/enum_map.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

struct SamplerDesc {
  enum class Filter : std::uint8_t { Nearest, Linear };
  enum class MipFilter : std::uint8_t { None, Nearest, Linear };
  enum class Wrap : std::uint8_t { Repeat, Clamp, MirroredRepeat };
  enum class CompareOp : std::uint8_t { Never, Less, Equal, LessEqual, Greater, NotEqual, GreaterEqual, Always };

  Filter min_filter    = Filter::Linear;
  Filter mag_filter    = Filter::Linear;
  MipFilter mip_filter = MipFilter::None;

  Wrap wrap_u = Wrap::Repeat;
  Wrap wrap_v = Wrap::Repeat;
  Wrap wrap_w = Wrap::Repeat;

  bool compare_enabled = false;
  CompareOp compare_op = CompareOp::LessEqual;
};

inline constexpr std::uint32_t kFullMipChain = 0;

enum class TextureKind : std::uint8_t {
  Tex2D,
  Tex2DArray,
  Tex3D,
  Cube,
  CubeArray,
};

struct TextureDesc {
  TextureKind kind = TextureKind::Tex2D;

  std::uint32_t width  = 0;
  std::uint32_t height = 1;
  std::uint32_t depth  = 1;
  std::uint32_t layers = 1;  // For CubeArray, this is cube count.

  TextureFormat format     = TextureFormat::RGBA8;
  std::uint32_t mip_levels = 1;  // Use kFullMipChain for a full chain.

  [[nodiscard]] constexpr std::uint32_t max_mip_levels() const noexcept {
    const std::uint32_t max_dim =
        (kind == TextureKind::Tex3D) ? std::max({width, height, depth}) : std::max(width, height);
    return static_cast<uint32_t>(std::bit_width(max_dim));
  }

  [[nodiscard]] constexpr std::uint32_t resolved_mip_levels() const noexcept {
    return mip_levels == kFullMipChain ? max_mip_levels() : mip_levels;
  }

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

  [[nodiscard]] constexpr bool has_valid_mip_request() const noexcept {
    return mip_levels == kFullMipChain || mip_levels <= max_mip_levels();
  }

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

class Texture {
 public:
  virtual ~Texture() = default;

  [[nodiscard]] virtual const TextureDesc& desc() const                                                = 0;
  virtual void set_data(std::span<const std::byte> data, std::uint32_t mip_level, std::uint32_t layer) = 0;
  void set_data(std::span<const std::byte> data, std::uint32_t mip_level = 0) { set_data(data, mip_level, 0); }

  [[nodiscard]] std::uint32_t width() const noexcept { return desc().width; }
  [[nodiscard]] std::uint32_t height() const noexcept { return desc().height; }
  [[nodiscard]] std::uint32_t depth() const noexcept { return desc().depth; }
  [[nodiscard]] std::uint32_t layers() const noexcept { return desc().layers; }
  [[nodiscard]] TextureKind kind() const noexcept { return desc().kind; }
  [[nodiscard]] TextureFormat format() const noexcept { return desc().format; }
  [[nodiscard]] std::uint32_t mip_levels() const noexcept { return desc().resolved_mip_levels(); }

  // TODO: [TEX-BIND] transitional -- remove when device/context pattern is implemented
  virtual void bind(std::uint32_t slot) const = 0;

  /** @brief Returns a handle suitable for passing to ImGui::Image(). */
  [[nodiscard]] virtual ImTextureID imgui_id() const noexcept = 0;
};

}  // namespace kEn
