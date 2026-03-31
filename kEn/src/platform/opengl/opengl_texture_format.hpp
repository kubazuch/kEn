#pragma once

#include <glad/gl.h>

#include <utility>

#include <kEn/renderer/texture_format.hpp>
#include <kEn/util/enum_map.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief OpenGL-specific format parameters derived from a @ref TextureFormat.
 *
 * Bundles the three values required by @c glTextureStorage2D and @c glTextureSubImage2D.
 */
struct GlTextureFormatInfo {
  GLenum internal_format; /**< Sized internal format passed to @c glTextureStorage2D. */
  GLenum pixel_format;    /**< Base format (e.g. @c GL_RED, @c GL_RGBA) for @c glTextureSubImage2D. */
  GLenum pixel_type;      /**< Pixel data type for @c glTextureSubImage2D (matches the CPU-side representation). */
};

/**
 * @brief OpenGL format metadata and accessor functions.
 *
 * Extends @ref kEn::texture_format with GL-specific lookups. All functions delegate to
 * @ref opengl_describe(), which performs an O(1) @ref util::EnumMap lookup.
 */
namespace texture_format {

namespace detail {

using GF = std::pair<TextureFormat, GlTextureFormatInfo>;

/** @brief Compile-time table mapping every @ref TextureFormat to its @ref GlTextureFormatInfo. */
inline constexpr util::EnumMap kGlTextureFormatInfo{{
    // Unsigned Normalized
    GF{TextureFormat::R8, {.internal_format = GL_R8, .pixel_format = GL_RED, .pixel_type = GL_UNSIGNED_BYTE}},
    GF{TextureFormat::RG8, {.internal_format = GL_RG8, .pixel_format = GL_RG, .pixel_type = GL_UNSIGNED_BYTE}},
    GF{TextureFormat::RGBA8, {.internal_format = GL_RGBA8, .pixel_format = GL_RGBA, .pixel_type = GL_UNSIGNED_BYTE}},
    GF{TextureFormat::RGBA8sRGB,
       {.internal_format = GL_SRGB8_ALPHA8, .pixel_format = GL_RGBA, .pixel_type = GL_UNSIGNED_BYTE}},

    // Floating point
    GF{TextureFormat::R16F, {.internal_format = GL_R16F, .pixel_format = GL_RED, .pixel_type = GL_FLOAT}},
    GF{TextureFormat::RG16F, {.internal_format = GL_RG16F, .pixel_format = GL_RG, .pixel_type = GL_FLOAT}},
    GF{TextureFormat::RGBA16F, {.internal_format = GL_RGBA16F, .pixel_format = GL_RGBA, .pixel_type = GL_FLOAT}},
    GF{TextureFormat::R32F, {.internal_format = GL_R32F, .pixel_format = GL_RED, .pixel_type = GL_FLOAT}},
    GF{TextureFormat::RG32F, {.internal_format = GL_RG32F, .pixel_format = GL_RG, .pixel_type = GL_FLOAT}},
    GF{TextureFormat::RGBA32F, {.internal_format = GL_RGBA32F, .pixel_format = GL_RGBA, .pixel_type = GL_FLOAT}},

    // Unsigned Integer
    GF{TextureFormat::R8UI,
       {.internal_format = GL_R8UI, .pixel_format = GL_RED_INTEGER, .pixel_type = GL_UNSIGNED_BYTE}},
    GF{TextureFormat::R16UI,
       {.internal_format = GL_R16UI, .pixel_format = GL_RED_INTEGER, .pixel_type = GL_UNSIGNED_SHORT}},
    GF{TextureFormat::R32UI,
       {.internal_format = GL_R32UI, .pixel_format = GL_RED_INTEGER, .pixel_type = GL_UNSIGNED_INT}},

    // Signed Integer
    GF{TextureFormat::R8I, {.internal_format = GL_R8I, .pixel_format = GL_RED_INTEGER, .pixel_type = GL_BYTE}},
    GF{TextureFormat::R16I, {.internal_format = GL_R16I, .pixel_format = GL_RED_INTEGER, .pixel_type = GL_SHORT}},
    GF{TextureFormat::R32I, {.internal_format = GL_R32I, .pixel_format = GL_RED_INTEGER, .pixel_type = GL_INT}},

    // Depth & stencil
    GF{TextureFormat::Depth16,
       {.internal_format = GL_DEPTH_COMPONENT16, .pixel_format = GL_DEPTH_COMPONENT, .pixel_type = GL_UNSIGNED_SHORT}},
    GF{TextureFormat::Depth32F,
       {.internal_format = GL_DEPTH_COMPONENT32F, .pixel_format = GL_DEPTH_COMPONENT, .pixel_type = GL_FLOAT}},
    GF{TextureFormat::Depth24Stencil8,
       {.internal_format = GL_DEPTH24_STENCIL8, .pixel_format = GL_DEPTH_STENCIL, .pixel_type = GL_UNSIGNED_INT_24_8}},
    GF{TextureFormat::Depth32FStencil8,
       {.internal_format = GL_DEPTH32F_STENCIL8,
        .pixel_format    = GL_DEPTH_STENCIL,
        .pixel_type      = GL_FLOAT_32_UNSIGNED_INT_24_8_REV}},
}};

}  // namespace detail

/**
 * @brief Return the full @ref GlTextureFormatInfo for @p fmt.
 * @param fmt Format to query.
 * @return A copy of the corresponding @ref GlTextureFormatInfo entry.
 */
[[nodiscard]] constexpr GlTextureFormatInfo opengl_describe(TextureFormat fmt) noexcept {
  return detail::kGlTextureFormatInfo[fmt];
}

/**
 * @brief Return the sized OpenGL internal format for @p fmt.
 *
 * Suitable for the @c internalformat parameter of @c glTextureStorage2D.
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr GLenum internal_format(TextureFormat fmt) noexcept {
  return opengl_describe(fmt).internal_format;
}

/**
 * @brief Return the base pixel format for @p fmt.
 *
 * Suitable for the @c format parameter of @c glTextureSubImage2D.
 * Integer formats use @c GL_RED_INTEGER / @c GL_RG_INTEGER etc.; color formats use
 * @c GL_RED / @c GL_RG / @c GL_RGBA; depth-stencil formats use @c GL_DEPTH_STENCIL.
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr GLenum pixel_format(TextureFormat fmt) noexcept { return opengl_describe(fmt).pixel_format; }

/**
 * @brief Return the pixel data type for @p fmt.
 *
 * Suitable for the @c type parameter of @c glTextureSubImage2D.
 * Matches the canonical CPU-side representation: @c GL_UNSIGNED_BYTE for 8-bit UNorm,
 * @c GL_FLOAT for all float formats (including *16F -- the driver converts),
 * @c GL_UNSIGNED_INT_24_8 for Depth24Stencil8, etc.
 * @param fmt Format to query.
 */
[[nodiscard]] constexpr GLenum pixel_type(TextureFormat fmt) noexcept { return opengl_describe(fmt).pixel_type; }

}  // namespace texture_format

}  // namespace kEn
