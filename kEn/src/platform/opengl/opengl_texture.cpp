#include "opengl_texture.hpp"

#include <imgui/imgui.h>
#include <stb_image.h>

#include <mEn/functions.hpp>

#include <kEn/core/assert.hpp>
#include <kEn/renderer/texture.hpp>

namespace kEn {

namespace {

GLenum ken_image_format_to_gl_data(ImageFormat format) {
  switch (format) {
    case ImageFormat::RGB8:
      return GL_RGB;
    case ImageFormat::RGBA8:
      return GL_RGBA;
  }

  KEN_CORE_ASSERT(false);
  return 0;
}

GLenum ken_image_format_to_gl_internal(ImageFormat format) {
  switch (format) {
    case ImageFormat::RGB8:
      return GL_RGB8;
    case ImageFormat::RGBA8:
      return GL_RGBA8;
  }

  KEN_CORE_ASSERT(false);
  return 0;
}

GLint ken_filter_to_gl(TextureSpec::filter filter, bool mipmap = false) {
  switch (filter) {
    case TextureSpec::filter::Nearest:
      return static_cast<GLint>(mipmap ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST);
    case TextureSpec::filter::Linear:
    default:
      return static_cast<GLint>(mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
  }
}

GLint ken_wrap_to_gl(TextureSpec::wrap wrap) {
  switch (wrap) {
    case TextureSpec::wrap::Clamp:
      return GL_CLAMP_TO_EDGE;
    case TextureSpec::wrap::MirroredRepeat:
      return GL_MIRRORED_REPEAT;
    case TextureSpec::wrap::Repeat:
    default:
      return GL_REPEAT;
  }
}

}  // namespace

OpenglTexture2D::OpenglTexture2D(const TextureSpec& specification)
    : spec_(specification),
      renderer_id_(0),
      internal_format_(ken_image_format_to_gl_internal(specification.format.value())),
      data_format_(ken_image_format_to_gl_data(specification.format.value())) {
  glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
  glTextureStorage2D(renderer_id_, 1, internal_format_, static_cast<GLsizei>(spec_.width.value()),
                     static_cast<GLsizei>(spec_.height.value()));

  glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, ken_filter_to_gl(specification.min_filter));
  glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, ken_filter_to_gl(specification.mag_filter));
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, ken_wrap_to_gl(specification.x_wrap));
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, ken_wrap_to_gl(specification.y_wrap));
}

OpenglTexture2D::OpenglTexture2D(const std::filesystem::path& path, const TextureSpec& spec)
    : spec_(spec), path_(path), renderer_id_(0) {
  int width    = 0;
  int height   = 0;
  int channels = 0;
  stbi_set_flip_vertically_on_load(1);
  stbi_uc* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

  if (data) {
    is_loaded_   = true;
    spec_.width  = width;
    spec_.height = height;

    GLenum internal_f = 0;
    GLenum data_f     = 0;
    if (channels == 4) {
      internal_f   = GL_RGBA8;
      data_f       = GL_RGBA;
      spec_.format = ImageFormat::RGBA8;
    } else if (channels == 3) {
      internal_f   = GL_RGB8;
      data_f       = GL_RGB;
      spec_.format = ImageFormat::RGB8;
    } else if (channels == 1) {
      internal_f   = GL_R8;
      data_f       = GL_RED;
      spec_.format = ImageFormat::R8;
    }

    KEN_CORE_INFO("Loaded texture with {} channels", channels);

    internal_format_ = internal_f;
    data_format_     = data_f;

    glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
    glTextureStorage2D(renderer_id_, static_cast<GLsizei>(spec_.mipmap_levels), internal_format_,
                       static_cast<GLsizei>(spec_.width.value()), static_cast<GLsizei>(spec_.height.value()));

    glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, ken_filter_to_gl(spec.min_filter, spec.mipmap_levels > 1));
    glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, ken_filter_to_gl(spec.mag_filter));
    glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, ken_wrap_to_gl(spec.x_wrap));
    glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, ken_wrap_to_gl(spec.y_wrap));

    if (channels == 4) {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    } else if (channels == 2) {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    } else {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    glTextureSubImage2D(renderer_id_, 0, 0, 0, width, height, data_f, GL_UNSIGNED_BYTE, data);

    glGenerateTextureMipmap(renderer_id_);

    stbi_image_free(data);
  } else {
    KEN_CORE_CRITICAL("stb_image load error: {0}", stbi_failure_reason());
  }
}

OpenglTexture2D::~OpenglTexture2D() { glDeleteTextures(1, &renderer_id_); }

void OpenglTexture2D::set_data(void* data, uint32_t size) {
  uint32_t bpp = data_format_ == GL_RGBA ? 4 : 3;
  KEN_CORE_ASSERT(size == spec_.width.value() * spec_.height.value() * bpp, "Data must be entire texture!");
  if (bpp == 4) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  } else {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }
  glTextureSubImage2D(renderer_id_, 0, 0, 0, static_cast<GLsizei>(spec_.width.value()),
                      static_cast<GLsizei>(spec_.height.value()), data_format_, GL_UNSIGNED_BYTE, data);
}

void OpenglTexture2D::bind(uint32_t slot) const { glBindTextureUnit(slot, renderer_id_); }

void OpenglTexture2D::imgui() {
  float height = mEn::max(mEn::min(static_cast<float>(spec_.height.value()), 250.F), 100.F);
  float width  = static_cast<float>(spec_.width.value()) / static_cast<float>(spec_.height.value()) * height;

  ImGui::Image(static_cast<ImTextureID>(renderer_id_), ImVec2{width, height}, ImVec2{0.0F, 1.0F}, ImVec2{1.0F, 0.0F});
}

}  // namespace kEn
