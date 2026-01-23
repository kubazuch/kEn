#pragma once

#include <memory>
#include <vector>

#include <mEn/vec4.hpp>

#include <kEn/core/core.hpp>

namespace kEn {

enum class FramebufferTextureFormat { None = 0, RGBA8, RED_INT, RED32, DEPTH24STENCIL8, Depth = DEPTH24STENCIL8 };

struct FramebufferTextureSpec {
  FramebufferTextureSpec() = default;
  FramebufferTextureSpec(FramebufferTextureFormat format) : texture_format(format) {}

  FramebufferTextureFormat texture_format = FramebufferTextureFormat::None;
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

  virtual uint32_t get_color_attachment_renderer_id(uint32_t id) const = 0;

  virtual const FramebufferSpec& get_spec() const = 0;

  static std::shared_ptr<Framebuffer> create(const FramebufferSpec& spec);
};

}  // namespace kEn
