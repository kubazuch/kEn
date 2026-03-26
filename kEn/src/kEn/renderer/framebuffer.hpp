#pragma once

#include <vector>

#include <mEn/vec4.hpp>

#include <kEn/core/core.hpp>
#include <kEn/renderer/texture_format.hpp>

namespace kEn {

struct FramebufferTextureSpec {
  FramebufferTextureSpec() = default;
  FramebufferTextureSpec(TextureFormat format)  // NOLINT(google-explicit-constructor)
      : texture_format(format) {}

  TextureFormat texture_format;
};

struct FramebufferAttachmentSpec {
  FramebufferAttachmentSpec() = default;
  FramebufferAttachmentSpec(std::initializer_list<FramebufferTextureSpec> attachments) : attachments(attachments) {}

  std::vector<FramebufferTextureSpec> attachments;
};

struct FramebufferSpec {
  uint32_t width{}, height{};
  FramebufferAttachmentSpec attachments;
};

class Framebuffer {
 public:
  virtual ~Framebuffer() = default;

  virtual void bind()   = 0;
  virtual void unbind() = 0;

  virtual void resize(uint32_t width, uint32_t height)                                                = 0;
  virtual int read_pixel(uint32_t attachment_id, int x, int y)                                        = 0;
  virtual void read_pixels(uint32_t attachment_id, int x, int y, int width, int height, void* buffer) = 0;

  virtual void bind_attachment_as_texture(uint32_t attachment_id, uint32_t slot) const = 0;
  virtual void bind_depth_as_texture(uint32_t slot) const                              = 0;

  virtual void clear_attachment(uint32_t attachment_id, int value)       = 0;
  virtual void clear_attachment(uint32_t attachment_id, float value)     = 0;
  virtual void clear_attachment(uint32_t attachment_id, mEn::Vec4 value) = 0;

  virtual uint32_t color_attachment_renderer_id(uint32_t id) const = 0;

  virtual const FramebufferSpec& spec() const = 0;
};

}  // namespace kEn
