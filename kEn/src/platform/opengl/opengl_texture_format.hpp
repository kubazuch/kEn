#pragma once

#include <glad/gl.h>

#include <kEn/renderer/texture_format.hpp>
#include <kEn/util/enum_map.hpp>

namespace kEn {

struct GlTextureFormatInfo {
  GLint internal_format;
  GLenum pixel_format;
  GLenum pixel_type;
  GLenum attachment;
};

namespace texture_format {

using GF = std::pair<TextureFormat, GlTextureFormatInfo>;

inline constexpr util::EnumMap kGlTextureFormatInfo{{
    GF{TextureFormat::R8,
       {.internal_format = GL_R8,
        .pixel_format    = GL_RED,
        .pixel_type      = GL_UNSIGNED_BYTE,
        .attachment      = GL_COLOR_ATTACHMENT0}},
    GF{TextureFormat::RG8,
       {.internal_format = GL_RG8,
        .pixel_format    = GL_RG,
        .pixel_type      = GL_UNSIGNED_BYTE,
        .attachment      = GL_COLOR_ATTACHMENT0}},
    GF{TextureFormat::RGB8,
       {.internal_format = GL_RGB8,
        .pixel_format    = GL_RGB,
        .pixel_type      = GL_UNSIGNED_BYTE,
        .attachment      = GL_COLOR_ATTACHMENT0}},
    GF{TextureFormat::RGBA8,
       {.internal_format = GL_RGBA8,
        .pixel_format    = GL_RGBA,
        .pixel_type      = GL_UNSIGNED_BYTE,
        .attachment      = GL_COLOR_ATTACHMENT0}},
    GF{TextureFormat::RGB8sRGB,
       {.internal_format = GL_SRGB8,
        .pixel_format    = GL_RGB,
        .pixel_type      = GL_UNSIGNED_BYTE,
        .attachment      = GL_COLOR_ATTACHMENT0}},
    GF{TextureFormat::RGBA8sRGB,
       {.internal_format = GL_SRGB8_ALPHA8,
        .pixel_format    = GL_RGBA,
        .pixel_type      = GL_UNSIGNED_BYTE,
        .attachment      = GL_COLOR_ATTACHMENT0}},

    GF{TextureFormat::R32F,
       {.internal_format = GL_R32F,
        .pixel_format    = GL_RED,
        .pixel_type      = GL_FLOAT,
        .attachment      = GL_COLOR_ATTACHMENT0}},
    GF{TextureFormat::RG32F,
       {.internal_format = GL_RG32F,
        .pixel_format    = GL_RG,
        .pixel_type      = GL_FLOAT,
        .attachment      = GL_COLOR_ATTACHMENT0}},
    GF{TextureFormat::RGB32F,
       {.internal_format = GL_RGB32F,
        .pixel_format    = GL_RGB,
        .pixel_type      = GL_FLOAT,
        .attachment      = GL_COLOR_ATTACHMENT0}},
    GF{TextureFormat::RGBA32F,
       {.internal_format = GL_RGBA32F,
        .pixel_format    = GL_RGBA,
        .pixel_type      = GL_FLOAT,
        .attachment      = GL_COLOR_ATTACHMENT0}},

    GF{TextureFormat::R32UI,
       {.internal_format = GL_R32UI,
        .pixel_format    = GL_RED_INTEGER,
        .pixel_type      = GL_UNSIGNED_INT,
        .attachment      = GL_COLOR_ATTACHMENT0}},

    GF{TextureFormat::Depth24,
       {.internal_format = GL_DEPTH_COMPONENT24,
        .pixel_format    = GL_DEPTH_COMPONENT,
        .pixel_type      = GL_UNSIGNED_INT,
        .attachment      = GL_DEPTH_ATTACHMENT}},
    GF{TextureFormat::Stencil8,
       {.internal_format = GL_STENCIL_INDEX8,
        .pixel_format    = GL_STENCIL_INDEX,
        .pixel_type      = GL_UNSIGNED_BYTE,
        .attachment      = GL_STENCIL_ATTACHMENT}},
    GF{TextureFormat::Depth24Stencil8,
       {.internal_format = GL_DEPTH24_STENCIL8,
        .pixel_format    = GL_DEPTH_STENCIL,
        .pixel_type      = GL_UNSIGNED_INT_24_8,
        .attachment      = GL_DEPTH_STENCIL_ATTACHMENT}},
}};

[[nodiscard]] constexpr GlTextureFormatInfo opengl_describe(TextureFormat fmt) noexcept {
  return kGlTextureFormatInfo[fmt];
}

[[nodiscard]] constexpr GLint internal_format(TextureFormat fmt) noexcept {
  return opengl_describe(fmt).internal_format;
}
[[nodiscard]] constexpr GLenum pixel_format(TextureFormat fmt) noexcept { return opengl_describe(fmt).pixel_format; }
[[nodiscard]] constexpr GLenum pixel_type(TextureFormat fmt) noexcept { return opengl_describe(fmt).pixel_type; }
[[nodiscard]] constexpr GLenum attachment(TextureFormat fmt) noexcept { return opengl_describe(fmt).attachment; }

}  // namespace texture_format

}  // namespace kEn