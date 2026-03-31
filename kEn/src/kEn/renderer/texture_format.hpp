#pragma once

#include <cstdint>
#include <utility>

#include <kEn/core/core.hpp>
#include <kEn/util/enum_map.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief Pixel/texel format identifier.
 *
 * Enumerators follow the convention: channels + bits-per-channel + type suffix.
 * Type suffixes: none = unsigned normalized (UNorm), @c F = float, @c UI = unsigned integer,
 * @c I = signed integer, @c sRGB = UNorm with sRGB transfer function.
 *
 * @c Count is a sentinel used by @ref util::EnumMap to enforce that lookup tables
 * cover every format at compile time.
 */
enum class TextureFormat : std::uint8_t {
  // Unsigned Normalized
  R8,
  RG8,
  RGBA8,
  RGBA8sRGB,

  // Floating point
  R16F,
  RG16F,
  RGBA16F,
  R32F,
  RG32F,
  RGBA32F,

  // Unsigned Integer
  R8UI,
  R16UI,
  R32UI,

  // Signed Integer
  R8I,
  R16I,
  R32I,

  // Depth & stencil
  Depth16,
  Depth32F,
  Depth24Stencil8,
  Depth32FStencil8,
  Count,
};

/**
 * @brief Coarse classification of a @ref TextureFormat by its intended attachment role.
 *
 * Used to determine which pipeline stages and framebuffer attachment points a format is
 * compatible with, independently of the underlying numeric representation.
 */
enum class FormatClass : std::uint8_t {
  Color,        /**< Color attachment / sampled color texture. */
  Depth,        /**< Depth-only attachment (no stencil). */
  DepthStencil, /**< Combined depth + stencil attachment. */
};

/**
 * @brief Numeric representation of the data stored in a texel.
 *
 * Describes how raw bit patterns are interpreted by the GPU.
 * This is orthogonal to @ref FormatClass: for example, depth formats can be either
 * @c UNorm (Depth16, Depth24Stencil8) or @c Float (Depth32F, Depth32FStencil8).
 */
enum class NumericKind : std::uint8_t {
  UNorm, /**< Unsigned normalized: [0, 2^N - 1] maps to [0.0, 1.0]. */
  UInt,  /**< Unsigned integer: raw unsigned value, not normalized. */
  Int,   /**< Signed integer: raw signed value, not normalized. */
  Float, /**< IEEE 754 floating point. */
};

/**
 * @brief Static metadata describing a @ref TextureFormat.
 *
 * Returned by @ref texture_format::describe(). All fields are valid for every format;
 * @c channel_count is 0 for depth and depth-stencil formats (they have no color channels).
 * @c block_width and @c block_height are 1 for all uncompressed formats and are reserved
 * for future block-compressed format support.
 */
struct TextureFormatInfo {
  FormatClass format_class;          /**< Coarse role classification (color, depth, depth-stencil). */
  NumericKind numeric_kind;          /**< How texel data is interpreted numerically. */
  std::uint8_t channel_count;        /**< Number of color channels (0 for depth/stencil formats). */
  std::uint8_t bytes_per_block;      /**< Bytes occupied by one texel (or one compressed block). */
  std::uint8_t block_width  = 1;     /**< Block width in texels; 1 for uncompressed formats. */
  std::uint8_t block_height = 1;     /**< Block height in texels; 1 for uncompressed formats. */
  bool is_srgb              = false; /**< True if the format uses an sRGB transfer function. */
};

/**
 * @brief Compile-time format metadata and predicate functions.
 *
 * All functions delegate to @ref describe(), which performs an O(1) table lookup.
 * Every function is @c [[nodiscard]] and @c constexpr, making them usable in
 * static assertions and template parameters.
 */
namespace texture_format {

using enum TextureFormat;

namespace detail {

using TF = std::pair<TextureFormat, TextureFormatInfo>;

/** @brief Compile-time table mapping every @ref TextureFormat to its @ref TextureFormatInfo. */
inline constexpr util::EnumMap kTextureFormatInfo{{
    // Unsigned Normalized
    TF{TextureFormat::R8,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::UNorm,
        .channel_count   = 1,
        .bytes_per_block = 1}},
    TF{TextureFormat::RG8,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::UNorm,
        .channel_count   = 2,
        .bytes_per_block = 2}},
    TF{TextureFormat::RGBA8,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::UNorm,
        .channel_count   = 4,
        .bytes_per_block = 4}},
    TF{TextureFormat::RGBA8sRGB,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::UNorm,
        .channel_count   = 4,
        .bytes_per_block = 4,
        .is_srgb         = true}},

    // Floating point
    TF{TextureFormat::R16F,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::Float,
        .channel_count   = 1,
        .bytes_per_block = 2}},
    TF{TextureFormat::RG16F,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::Float,
        .channel_count   = 2,
        .bytes_per_block = 4}},
    TF{TextureFormat::RGBA16F,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::Float,
        .channel_count   = 4,
        .bytes_per_block = 8}},
    TF{TextureFormat::R32F,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::Float,
        .channel_count   = 1,
        .bytes_per_block = 4}},
    TF{TextureFormat::RG32F,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::Float,
        .channel_count   = 2,
        .bytes_per_block = 8}},
    TF{TextureFormat::RGBA32F,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::Float,
        .channel_count   = 4,
        .bytes_per_block = 16}},

    // Unsigned Integer
    TF{TextureFormat::R8UI,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::UInt,
        .channel_count   = 1,
        .bytes_per_block = 1}},
    TF{TextureFormat::R16UI,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::UInt,
        .channel_count   = 1,
        .bytes_per_block = 2}},
    TF{TextureFormat::R32UI,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::UInt,
        .channel_count   = 1,
        .bytes_per_block = 4}},

    // Signed Integer
    TF{TextureFormat::R8I,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::Int,
        .channel_count   = 1,
        .bytes_per_block = 1}},
    TF{TextureFormat::R16I,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::Int,
        .channel_count   = 1,
        .bytes_per_block = 2}},
    TF{TextureFormat::R32I,
       {.format_class    = FormatClass::Color,
        .numeric_kind    = NumericKind::Int,
        .channel_count   = 1,
        .bytes_per_block = 4}},

    // Depth & stencil
    TF{TextureFormat::Depth16,
       {.format_class    = FormatClass::Depth,
        .numeric_kind    = NumericKind::UNorm,
        .channel_count   = 0,
        .bytes_per_block = 2}},
    TF{TextureFormat::Depth32F,
       {.format_class    = FormatClass::Depth,
        .numeric_kind    = NumericKind::Float,
        .channel_count   = 0,
        .bytes_per_block = 4}},
    TF{TextureFormat::Depth24Stencil8,
       {.format_class    = FormatClass::DepthStencil,
        .numeric_kind    = NumericKind::UNorm,
        .channel_count   = 0,
        .bytes_per_block = 4}},
    TF{TextureFormat::Depth32FStencil8,
       {.format_class    = FormatClass::DepthStencil,
        .numeric_kind    = NumericKind::Float,
        .channel_count   = 0,
        .bytes_per_block = 8}},
}};

}  // namespace detail

/**
 * @brief Return the full @ref TextureFormatInfo for @p fmt.
 * @param fmt Format to query.
 * @return A copy of the corresponding @ref TextureFormatInfo entry.
 */
[[nodiscard]] constexpr TextureFormatInfo describe(TextureFormat fmt) noexcept {
  return detail::kTextureFormatInfo[fmt];
}

/**
 * @brief Return the @ref FormatClass of @p fmt.
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr FormatClass format_class(TextureFormat fmt) noexcept { return describe(fmt).format_class; }

/**
 * @brief Return the @ref NumericKind of @p fmt.
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr NumericKind numeric_kind(TextureFormat fmt) noexcept { return describe(fmt).numeric_kind; }

/**
 * @brief Return the number of color channels in @p fmt.
 *
 * Returns 0 for depth and depth-stencil formats, which carry no color data.
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr std::uint8_t color_channel_count(TextureFormat fmt) noexcept {
  return describe(fmt).channel_count;
}

/**
 * @brief Return the byte size of one texel (or one compressed block) for @p fmt.
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr std::uint8_t bytes_per_block(TextureFormat fmt) noexcept {
  return describe(fmt).bytes_per_block;
}

/**
 * @brief Return the block dimensions @c {width, height} in texels for @p fmt.
 *
 * Both components are 1 for all currently defined uncompressed formats.
 * Reserved for block-compressed format support.
 * @param fmt Format to query.
 * @return A pair @c {block_width, block_height}.
 */
[[nodiscard]] constexpr std::pair<std::uint8_t, std::uint8_t> block_extent(TextureFormat fmt) noexcept {
  const auto desc = describe(fmt);
  return {desc.block_width, desc.block_height};
}

/**
 * @brief Return @c true if @p fmt is a color format (@ref FormatClass::Color).
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr bool is_color_format(TextureFormat fmt) noexcept {
  return format_class(fmt) == FormatClass::Color;
}

/**
 * @brief Return @c true if @p fmt has a depth aspect.
 *
 * True for both depth-only (@ref FormatClass::Depth) and
 * combined depth-stencil (@ref FormatClass::DepthStencil) formats.
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr bool has_depth_aspect(TextureFormat fmt) noexcept {
  const auto cls = format_class(fmt);
  return cls == FormatClass::Depth || cls == FormatClass::DepthStencil;
}

/**
 * @brief Return @c true if @p fmt has a stencil aspect (@ref FormatClass::DepthStencil).
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr bool has_stencil_aspect(TextureFormat fmt) noexcept {
  return format_class(fmt) == FormatClass::DepthStencil;
}

/**
 * @brief Return @c true if @p fmt is depth-only (no stencil component).
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr bool is_depth_only(TextureFormat fmt) noexcept {
  return format_class(fmt) == FormatClass::Depth;
}

/**
 * @brief Return @c true if @p fmt is a combined depth-stencil format.
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr bool is_depth_stencil(TextureFormat fmt) noexcept {
  return format_class(fmt) == FormatClass::DepthStencil;
}

/**
 * @brief Return @c true if @p fmt uses an sRGB transfer function.
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr bool is_srgb_format(TextureFormat fmt) noexcept { return describe(fmt).is_srgb; }

/**
 * @brief Return @c true if @p fmt is an unsigned integer color format (@ref NumericKind::UInt).
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr bool is_unsigned_integer_format(TextureFormat fmt) noexcept {
  return is_color_format(fmt) && numeric_kind(fmt) == NumericKind::UInt;
}

/**
 * @brief Return @c true if @p fmt is a signed integer color format (@ref NumericKind::Int).
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr bool is_signed_integer_format(TextureFormat fmt) noexcept {
  return is_color_format(fmt) && numeric_kind(fmt) == NumericKind::Int;
}

/**
 * @brief Return @c true if @p fmt is any integer color format (signed or unsigned).
 * @param fmt Format to query.
 * @see is_unsigned_integer_format(), is_signed_integer_format()
 */
[[nodiscard]] constexpr bool is_integer_format(TextureFormat fmt) noexcept {
  return is_unsigned_integer_format(fmt) || is_signed_integer_format(fmt);
}

/**
 * @brief Return @c true if @p fmt is a floating-point color format (@ref NumericKind::Float).
 *
 * @note Depth formats such as @c Depth32F also store floats internally but return @c false
 *       here, as they are not color formats. Use @ref numeric_kind() directly if you need
 *       the raw representation of depth formats.
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr bool is_float_format(TextureFormat fmt) noexcept {
  return is_color_format(fmt) && numeric_kind(fmt) == NumericKind::Float;
}

/**
 * @brief Return @c true if @p fmt is an unsigned normalized color format (@ref NumericKind::UNorm).
 *
 * @note Depth16 and Depth24Stencil8 are also UNorm internally but return @c false here.
 *       Use @ref numeric_kind() directly for depth format queries.
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr bool is_normalized_format(TextureFormat fmt) noexcept {
  return is_color_format(fmt) && numeric_kind(fmt) == NumericKind::UNorm;
}

}  // namespace texture_format

}  // namespace kEn
