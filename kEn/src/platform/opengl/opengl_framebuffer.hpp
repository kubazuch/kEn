#pragma once

#include <kEn/core/assert.hpp>
#include <kEn/renderer/framebuffer.hpp>

namespace kEn {

class OpenglFramebuffer : public Framebuffer {
 public:
  explicit OpenglFramebuffer(const FramebufferSpec& spec);
  virtual ~OpenglFramebuffer();

  void invalidate();

  void bind() override;
  void unbind() override;

  void resize(uint32_t width, uint32_t height) override;
  int read_pixel(uint32_t attachment_id, int x, int y) override;

  void bind_attachment_as_texture(uint32_t attachment_id, uint32_t slot) const override;
  void bind_depth_as_texture(uint32_t slot) const override;

  void clear_attachment(uint32_t attachment_id, int value) override;
  void clear_attachment(uint32_t attachment_id, mEn::Vec4 value) override;

  uint32_t get_color_attachment_renderer_id(uint32_t id) const override {
    KEN_CORE_ASSERT(id < color_attachments_.size());
    return color_attachments_[id];
  }

  const FramebufferSpec& get_spec() const override { return spec_; }

 private:
  uint32_t renderer_id_ = 0;
  FramebufferSpec spec_;

  std::vector<FramebufferTextureSpec> color_attachment_specs_;
  FramebufferTextureSpec depth_attachment_spec_ = FramebufferTextureFormat::None;

  std::vector<uint32_t> color_attachments_;
  uint32_t depth_attachment_ = 0;
};

}  // namespace kEn
