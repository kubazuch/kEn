#include <glad/gl.h>

#include <kEn/core/assert.hpp>
#include <kenpch.hpp>
#include <platform/opengl/opengl_framebuffer.hpp>

#include "kEn/renderer/framebuffer.hpp"

namespace kEn {

static constexpr uint32_t kMaxFramebufferSize = 8192;

namespace utils {
static GLenum texture_target(bool multisampled) {
  return static_cast<GLenum>(multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D);
}

static void create_textures(bool multisampled, uint32_t* outId, uint32_t count) {
  glCreateTextures(texture_target(multisampled), static_cast<GLsizei>(count), outId);
}

static void bind_texture(bool multisampled, uint32_t id) { glBindTexture(texture_target(multisampled), id); }

static void attach_color_texture(uint32_t id, int samples, GLint internalFormat, GLenum format, uint32_t width,
                                 uint32_t height, size_t index) {
  bool multisampled = samples > 1;
  if (multisampled) {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, static_cast<GLsizei>(width),
                            static_cast<GLsizei>(height), GL_FALSE);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, format,
                 GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + index),
                         texture_target(multisampled), id, 0);
}

static void attach_depth_texture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width,
                                 uint32_t height) {
  bool multisampled = samples > 1;
  if (multisampled) {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, static_cast<GLsizei>(width),
                            static_cast<GLsizei>(height), GL_FALSE);
  } else {
    glTexStorage2D(GL_TEXTURE_2D, 1, format, static_cast<GLsizei>(width), static_cast<GLsizei>(height));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, texture_target(multisampled), id, 0);
}

static bool is_depth_format(FramebufferTextureFormat format) {
  switch (format) {
    case FramebufferTextureFormat::DEPTH24STENCIL8:
      return true;
    default:
      return false;
  }
}

static GLenum texture_format_to_GL(FramebufferTextureFormat format) {
  switch (format) {
    case FramebufferTextureFormat::RGBA8:
      return GL_RGBA;
    case FramebufferTextureFormat::RED_INT:
      return GL_RED_INTEGER;
    default:
      KEN_CORE_ASSERT(false);
      return 0;
  }
}
}  // namespace utils

OpenglFramebuffer::OpenglFramebuffer(const FramebufferSpec& spec) : spec_(spec) {
  for (auto spec : spec_.attachments.attachments) {
    if (!utils::is_depth_format(spec.texture_format)) {
      color_attachment_specs_.emplace_back(spec);
    } else {
      depth_attachment_spec_ = spec;
    }
  }

  invalidate();
}

OpenglFramebuffer::~OpenglFramebuffer() {
  glDeleteFramebuffers(1, &renderer_id_);
  glDeleteTextures(static_cast<GLsizei>(color_attachments_.size()), color_attachments_.data());
  glDeleteTextures(1, &depth_attachment_);
}

void OpenglFramebuffer::invalidate() {
  if (renderer_id_) {
    glDeleteFramebuffers(1, &renderer_id_);
    glDeleteTextures(static_cast<GLsizei>(color_attachments_.size()), color_attachments_.data());
    glDeleteTextures(1, &depth_attachment_);

    color_attachments_.clear();
    depth_attachment_ = 0;
  }

  glCreateFramebuffers(1, &renderer_id_);
  glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);

  if (!color_attachment_specs_.empty()) {
    color_attachments_.resize(color_attachment_specs_.size());
    utils::create_textures(false, color_attachments_.data(), color_attachments_.size());

    for (size_t i = 0; i < color_attachments_.size(); i++) {
      utils::bind_texture(false, color_attachments_[i]);
      switch (color_attachment_specs_[i].texture_format) {
        case FramebufferTextureFormat::RGBA8:
          utils::attach_color_texture(color_attachments_[i], 1, GL_RGBA8, GL_RGBA, spec_.width, spec_.height, i);
          break;
        case FramebufferTextureFormat::RED_INT:
          utils::attach_color_texture(color_attachments_[i], 1, GL_R32I, GL_RED_INTEGER, spec_.width, spec_.height, i);
          break;
      }
    }
  }

  if (depth_attachment_spec_.texture_format != FramebufferTextureFormat::None) {
    utils::create_textures(false, &depth_attachment_, 1);
    utils::bind_texture(false, depth_attachment_);
    switch (depth_attachment_spec_.texture_format) {
      case FramebufferTextureFormat::DEPTH24STENCIL8:
        utils::attach_depth_texture(depth_attachment_, 1, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, spec_.width,
                                    spec_.height);
        break;
    }
  }

  if (color_attachments_.size() > 1) {
    KEN_CORE_ASSERT(color_attachments_.size() <= 4);
    GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(static_cast<GLsizei>(color_attachments_.size()), buffers);
  } else if (color_attachments_.empty()) {
    glDrawBuffer(GL_NONE);
  }

  KEN_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenglFramebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);
  glViewport(0, 0, static_cast<GLsizei>(spec_.width), static_cast<GLsizei>(spec_.height));
}

void OpenglFramebuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenglFramebuffer::resize(uint32_t width, uint32_t height) {
  if (width == 0 || height == 0 || width > kMaxFramebufferSize || height > kMaxFramebufferSize) {
    KEN_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
    return;
  }
  spec_.width  = width;
  spec_.height = height;

  invalidate();
}

int OpenglFramebuffer::read_pixel(uint32_t attachment_id, int x, int y) {
  KEN_CORE_ASSERT(attachment_id < color_attachments_.size());

  glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_id);
  int pixel_data = 0;
  glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);
  return pixel_data;
}

void OpenglFramebuffer::clear_attachment(uint32_t attachment_id, int value) {
  KEN_CORE_ASSERT(attachment_id < color_attachments_.size());

  auto& spec = color_attachment_specs_[attachment_id];
  glClearTexImage(color_attachments_[attachment_id], 0, utils::texture_format_to_GL(spec.texture_format), GL_INT,
                  &value);
}

}  // namespace kEn
