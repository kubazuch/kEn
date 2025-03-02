#pragma once

#include <kEn/core/core.hpp>

namespace kEn {

enum class framebuffer_texture_format { none = 0, RGBA8, RED_INT, DEPTH24STENCIL8, depth = DEPTH24STENCIL8 };

struct framebuffer_texture_spec {
  framebuffer_texture_spec() = default;
  framebuffer_texture_spec(framebuffer_texture_format format) : texture_format(format) {}

  framebuffer_texture_format texture_format = framebuffer_texture_format::none;
};

struct framebuffer_attachment_spec {
  framebuffer_attachment_spec() = default;
  framebuffer_attachment_spec(std::initializer_list<framebuffer_texture_spec> attachments) : attachments(attachments) {}

  std::vector<framebuffer_texture_spec> attachments;
};

struct framebuffer_spec {
  uint32_t width, height;
  framebuffer_attachment_spec attachments;
};

class framebuffer {
 public:
  virtual ~framebuffer() = default;

  virtual void bind()   = 0;
  virtual void unbind() = 0;

  virtual void resize(uint32_t width, uint32_t height)         = 0;
  virtual int read_pixel(uint32_t attachment_id, int x, int y) = 0;

  virtual void clear_attachment(uint32_t attachment_id, int value) = 0;

  virtual uint32_t get_color_attachment_renderer_id(uint32_t id = 0) const = 0;

  virtual const framebuffer_spec& get_spec() const = 0;

  static std::shared_ptr<framebuffer> create(const framebuffer_spec& spec);
};

}  // namespace kEn
