#include "opengl_framebuffer.hpp"

#include <glad/gl.h>

#include <cstddef>
#include <cstdint>
#include <utility>

#include <mEn/features/type_ptr.hpp>
#include <mEn/fwd.hpp>

#include <kEn/core/assert.hpp>
#include <kEn/core/log.hpp>
#include <kEn/renderer/framebuffer.hpp>
#include <kEn/renderer/texture_format.hpp>

#include "opengl_texture_format.hpp"

namespace kEn {

static constexpr uint32_t kMaxFramebufferSize = 8192;

namespace {

GLenum texture_target(bool multisampled) { return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D; }

void create_textures(bool multisampled, std::uint32_t* outId, std::size_t count) {
  glCreateTextures(texture_target(multisampled), static_cast<GLsizei>(count), outId);
}

void bind_texture(bool multisampled, std::uint32_t id) { glBindTexture(texture_target(multisampled), id); }

void attach_color_texture(GLuint fbo, std::uint32_t id, int samples, TextureFormat fmt, std::uint32_t width,
                          std::uint32_t height, std::size_t index) {
  const bool multisampled = samples > 1;
  const auto gl           = texture_format::opengl_describe(fmt);

  if (multisampled) {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, gl.internal_format, static_cast<GLsizei>(width),
                            static_cast<GLsizei>(height), GL_FALSE);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(gl.internal_format), static_cast<GLsizei>(width),
                 static_cast<GLsizei>(height), 0, gl.pixel_format, gl.pixel_type, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
  glNamedFramebufferTexture(fbo, static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + index), id, 0);
}

void attach_depth_texture(GLuint fbo, uint32_t id, int samples, TextureFormat fmt, uint32_t width, uint32_t height) {
  const bool multisampled = samples > 1;
  const auto gl           = texture_format::opengl_describe(fmt);

  if (multisampled) {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, gl.internal_format, static_cast<GLsizei>(width),
                            static_cast<GLsizei>(height), GL_FALSE);
  } else {
    glTexStorage2D(GL_TEXTURE_2D, 1, gl.internal_format, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
  const GLenum attachment = texture_format::has_stencil_aspect(fmt) ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
  glNamedFramebufferTexture(fbo, attachment, id, 0);
}

}  // namespace

OpenglFramebuffer::OpenglFramebuffer(FramebufferSpec spec) : spec_(std::move(spec)) {
  KEN_CORE_ASSERT(spec_.mip_levels == 1, "OpenglFramebuffer currently supports only mip_levels == 1");
  KEN_CORE_ASSERT(spec_.array_size == 1, "OpenglFramebuffer currently supports only array_size == 1");
  KEN_CORE_ASSERT(spec_.samples >= 1, "Framebuffer sample count must be at least 1");

  color_attachment_specs_ = spec_.attachments.color_attachments;
  depth_attachment_spec_  = spec_.attachments.depth_attachment;

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

  const bool multisampled = spec_.samples > 1;

  glCreateFramebuffers(1, &renderer_id_);

  if (!color_attachment_specs_.empty()) {
    color_attachments_.resize(color_attachment_specs_.size());
    create_textures(multisampled, color_attachments_.data(), color_attachments_.size());

    for (std::size_t i = 0; i < color_attachments_.size(); ++i) {
      bind_texture(multisampled, color_attachments_[i]);
      attach_color_texture(renderer_id_, color_attachments_[i], static_cast<int>(spec_.samples),
                           color_attachment_specs_[i].texture_format, spec_.width, spec_.height, i);
    }
  }

  if (depth_attachment_spec_.has_value()) {
    create_textures(multisampled, &depth_attachment_, 1);
    bind_texture(multisampled, depth_attachment_);
    attach_depth_texture(renderer_id_, depth_attachment_, static_cast<int>(spec_.samples),
                         depth_attachment_spec_->texture_format, spec_.width, spec_.height);
  }

  if (color_attachments_.size() > 1) {
    KEN_CORE_ASSERT(color_attachments_.size() <= 4);
    GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glNamedFramebufferDrawBuffers(renderer_id_, static_cast<GLsizei>(color_attachments_.size()), buffers);
  } else if (color_attachments_.empty()) {
    glNamedFramebufferDrawBuffer(renderer_id_, GL_NONE);
    glNamedFramebufferReadBuffer(renderer_id_, GL_NONE);
  }

  KEN_CORE_ASSERT(glCheckNamedFramebufferStatus(renderer_id_, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                  "Framebuffer is incomplete!");
}

void OpenglFramebuffer::bind_for_rendering() {
  glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);
  glViewport(0, 0, static_cast<GLsizei>(spec_.width), static_cast<GLsizei>(spec_.height));
}

void OpenglFramebuffer::resize(std::uint32_t width, std::uint32_t height) {
  if (width == 0 || height == 0 || width > kMaxFramebufferSize || height > kMaxFramebufferSize) {
    KEN_CORE_WARN("Attempted to rezize framebuffer to {0}x{1}", width, height);
    return;
  }

  spec_.width  = width;
  spec_.height = height;

  invalidate();
}

void OpenglFramebuffer::read_pixels(std::uint32_t attachment_id, int x, int y, int width, int height,
                                    std::span<std::byte> out_buffer, std::size_t out_row_pitch) const {
  KEN_CORE_ASSERT(attachment_id < color_attachments_.size());
  KEN_CORE_ASSERT(spec_.samples == 1, "OpenglFramebuffer::read_pixels does not support multisampled attachments");

  const auto gl           = texture_format::opengl_describe(color_attachment_specs_[attachment_id].texture_format);
  const GLuint texture_id = color_attachments_[attachment_id];

  if (out_row_pitch == 0) {
    glGetTextureSubImage(texture_id, 0, x, y, 0, width, height, 1, gl.pixel_format, gl.pixel_type,
                         static_cast<GLsizei>(out_buffer.size_bytes()), out_buffer.data());
  } else {
    for (int row = 0; row < height; ++row) {
      std::byte* row_ptr = out_buffer.data() + (static_cast<std::size_t>(row) * out_row_pitch);
      glGetTextureSubImage(texture_id, 0, x, y + row, 0, width, 1, 1, gl.pixel_format, gl.pixel_type,
                           static_cast<GLsizei>(out_row_pitch), row_ptr);
    }
  }
}

std::uintptr_t OpenglFramebuffer::native_color_attachment_handle(std::uint32_t attachment_id) const {
  KEN_CORE_ASSERT(attachment_id < color_attachments_.size());
  return static_cast<std::uintptr_t>(color_attachments_[attachment_id]);
}

void OpenglFramebuffer::bind_color_attachment_as_texture(std::uint32_t attachment_id, ShaderStage stage,
                                                         std::uint32_t slot) const {
  static_cast<void>(stage);

  KEN_CORE_ASSERT(attachment_id < color_attachments_.size());
  KEN_CORE_ASSERT(color_attachment_specs_[attachment_id].shader_readable,
                  "Color attachment was not created as shader-readable");

  glBindTextureUnit(slot, color_attachments_[attachment_id]);
}

void OpenglFramebuffer::bind_depth_as_texture(ShaderStage stage, std::uint32_t slot) const {
  static_cast<void>(stage);

  KEN_CORE_ASSERT(depth_attachment_ != 0);
  KEN_CORE_ASSERT(depth_attachment_spec_.has_value());
  KEN_CORE_ASSERT(depth_attachment_spec_->shader_readable, "Depth attachment was not created as shader-readable");

  glBindTextureUnit(slot, depth_attachment_);
}

void OpenglFramebuffer::clear_color_attachment(std::uint32_t attachment_id, std::int32_t value) {
  KEN_CORE_ASSERT(attachment_id < color_attachments_.size());

  const GLint clear_value[4] = {value, value, value, value};

  glClearNamedFramebufferiv(renderer_id_, GL_COLOR, static_cast<GLint>(attachment_id), clear_value);
}

void OpenglFramebuffer::clear_color_attachment(std::uint32_t attachment_id, std::uint32_t value) {
  KEN_CORE_ASSERT(attachment_id < color_attachments_.size());

  const GLuint clear_value[4] = {value, value, value, value};
  glClearNamedFramebufferuiv(renderer_id_, GL_COLOR, static_cast<GLint>(attachment_id), clear_value);
}

void OpenglFramebuffer::clear_color_attachment(std::uint32_t attachment_id, float value) {
  KEN_CORE_ASSERT(attachment_id < color_attachments_.size());

  const GLfloat clear_value[4] = {value, value, value, value};
  glClearNamedFramebufferfv(renderer_id_, GL_COLOR, static_cast<GLint>(attachment_id), clear_value);
}

void OpenglFramebuffer::clear_color_attachment(std::uint32_t attachment_id, const mEn::Vec4& value) {
  KEN_CORE_ASSERT(attachment_id < color_attachments_.size());

  glClearNamedFramebufferfv(renderer_id_, GL_COLOR, static_cast<GLint>(attachment_id), mEn::value_ptr(value));
}

void OpenglFramebuffer::clear_depth(float depth) {
  KEN_CORE_ASSERT(depth_attachment_ != 0);

  glClearNamedFramebufferfv(renderer_id_, GL_DEPTH, 0, &depth);
}

void OpenglFramebuffer::clear_stencil(std::uint8_t stencil) {
  KEN_CORE_ASSERT(depth_attachment_ != 0);

  const auto stencil_value = static_cast<GLint>(stencil);
  glClearNamedFramebufferiv(renderer_id_, GL_STENCIL, 0, &stencil_value);
}

void OpenglFramebuffer::clear_depth_stencil(float depth, std::uint8_t stencil) {
  KEN_CORE_ASSERT(depth_attachment_ != 0);

  glClearNamedFramebufferfi(renderer_id_, GL_DEPTH_STENCIL, 0, depth, static_cast<GLint>(stencil));
}

}  // namespace kEn
