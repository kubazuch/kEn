#pragma once

#include <cstdint>

#include <kEn/core/core.hpp>
#include <kEn/util/enum_map.hpp>
#include <kEn/util/flags.hpp>

namespace kEn {

enum class Aspect : std::uint8_t {
  Color   = 1 << 0,
  Depth   = 1 << 1,
  Stencil = 1 << 2,
};

KEN_ENABLE_FLAGS(::kEn::Aspect);

using Aspects = util::Flags<Aspect>;

enum class TextureFormat : std::uint8_t {
  R8,
  RG8,
  RGB8,
  RGBA8,
  RGB8sRGB,
  RGBA8sRGB,

  R32F,
  RG32F,
  RGB32F,
  RGBA32F,

  R32UI,

  Depth24,
  Stencil8,
  Depth24Stencil8,
  Count,
};

struct TextureFormatInfo {
  Aspects aspects;
  std::uint8_t components;
  std::uint8_t bytes_per_texel;
  bool integer;
  bool srgb;
};

namespace texture_format {

using enum TextureFormat;

using TF = std::pair<TextureFormat, TextureFormatInfo>;

inline constexpr util::EnumMap kTextureFormatInfo{{
    TF{TextureFormat::R8,
       {.aspects = Aspect::Color, .components = 1, .bytes_per_texel = 1, .integer = false, .srgb = false}},
    TF{TextureFormat::RG8,
       {.aspects = Aspect::Color, .components = 2, .bytes_per_texel = 2, .integer = false, .srgb = false}},
    TF{TextureFormat::RGB8,
       {.aspects = Aspect::Color, .components = 3, .bytes_per_texel = 3, .integer = false, .srgb = false}},
    TF{TextureFormat::RGBA8,
       {.aspects = Aspect::Color, .components = 4, .bytes_per_texel = 4, .integer = false, .srgb = false}},
    TF{TextureFormat::RGB8sRGB,
       {.aspects = Aspect::Color, .components = 3, .bytes_per_texel = 3, .integer = false, .srgb = true}},
    TF{TextureFormat::RGBA8sRGB,
       {.aspects = Aspect::Color, .components = 4, .bytes_per_texel = 4, .integer = false, .srgb = true}},

    TF{TextureFormat::R32F,
       {.aspects = Aspect::Color, .components = 1, .bytes_per_texel = 4, .integer = false, .srgb = false}},
    TF{TextureFormat::RG32F,
       {.aspects = Aspect::Color, .components = 2, .bytes_per_texel = 8, .integer = false, .srgb = false}},
    TF{TextureFormat::RGB32F,
       {.aspects = Aspect::Color, .components = 3, .bytes_per_texel = 12, .integer = false, .srgb = false}},
    TF{TextureFormat::RGBA32F,
       {.aspects = Aspect::Color, .components = 4, .bytes_per_texel = 16, .integer = false, .srgb = false}},

    TF{TextureFormat::R32UI,
       {.aspects = Aspect::Color, .components = 1, .bytes_per_texel = 4, .integer = true, .srgb = false}},

    TF{TextureFormat::Depth24,
       {.aspects = Aspect::Depth, .components = 1, .bytes_per_texel = 4, .integer = false, .srgb = false}},
    TF{TextureFormat::Stencil8,
       {.aspects = Aspect::Stencil, .components = 1, .bytes_per_texel = 1, .integer = true, .srgb = false}},
    TF{TextureFormat::Depth24Stencil8,
       {.aspects         = Aspect::Depth | Aspect::Stencil,
        .components      = 2,
        .bytes_per_texel = 4,
        .integer         = false,
        .srgb            = false}},
}};

[[nodiscard]] constexpr TextureFormatInfo describe(TextureFormat fmt) noexcept { return kTextureFormatInfo[fmt]; }

[[nodiscard]] constexpr Aspects aspects(TextureFormat fmt) noexcept { return describe(fmt).aspects; }
[[nodiscard]] constexpr bool is_integer(TextureFormat fmt) noexcept { return describe(fmt).integer; }
[[nodiscard]] constexpr bool is_srgb(TextureFormat fmt) noexcept { return describe(fmt).srgb; }
[[nodiscard]] constexpr std::uint8_t component_count(TextureFormat fmt) noexcept { return describe(fmt).components; }
[[nodiscard]] constexpr std::uint8_t bytes_per_texel(TextureFormat fmt) noexcept {
  return describe(fmt).bytes_per_texel;
}

[[nodiscard]] constexpr bool supports_color(TextureFormat fmt) noexcept { return aspects(fmt).test(Aspect::Color); }
[[nodiscard]] constexpr bool supports_depth(TextureFormat fmt) noexcept { return aspects(fmt).test(Aspect::Depth); }
[[nodiscard]] constexpr bool supports_stencil(TextureFormat fmt) noexcept { return aspects(fmt).test(Aspect::Stencil); }
[[nodiscard]] constexpr bool supports_depthstencil(TextureFormat fmt) noexcept {
  const auto a = aspects(fmt);
  return a.test(Aspect::Depth) && a.test(Aspect::Stencil);
}

}  // namespace texture_format

}  // namespace kEn