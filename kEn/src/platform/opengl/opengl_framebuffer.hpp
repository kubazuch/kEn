#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

#include <kEn/core/assert.hpp>
#include <kEn/renderer/framebuffer.hpp>

namespace kEn {

class OpenglFramebuffer : public Framebuffer {
 public:
  explicit OpenglFramebuffer(FramebufferSpec spec);
  ~OpenglFramebuffer() override;

  void invalidate();

  void bind_for_rendering() override;
  void resize(std::uint32_t width, std::uint32_t height) override;

  [[nodiscard]] const FramebufferSpec& spec() const override { return spec_; }

  [[nodiscard]] std::size_t color_attachment_count() const override { return color_attachments_.size(); }

  [[nodiscard]] TextureFormat color_attachment_format(std::uint32_t attachment_id) const override {
    KEN_CORE_ASSERT(attachment_id < color_attachment_specs_.size());
    return color_attachment_specs_[attachment_id].texture_format;
  }

  [[nodiscard]] bool has_depth_attachment() const override { return depth_attachment_spec_.has_value(); }

  [[nodiscard]] std::optional<TextureFormat> depth_attachment_format() const override {
    if (!depth_attachment_spec_.has_value()) {
      return std::nullopt;
    }
    return depth_attachment_spec_->texture_format;
  }

  void read_pixels(std::uint32_t attachment_id, int x, int y, int width, int height, std::span<std::byte> out_buffer,
                   std::size_t out_row_pitch) const override;

  [[nodiscard]] std::uintptr_t native_color_attachment_handle(std::uint32_t attachment_id) const override;

  void bind_color_attachment_as_texture(std::uint32_t attachment_id, ShaderStage stage,
                                        std::uint32_t slot) const override;
  void bind_depth_as_texture(ShaderStage stage, std::uint32_t slot) const override;

  void clear_color_attachment(std::uint32_t attachment_id, std::int32_t value) override;
  void clear_color_attachment(std::uint32_t attachment_id, std::uint32_t value) override;
  void clear_color_attachment(std::uint32_t attachment_id, float value) override;
  void clear_color_attachment(std::uint32_t attachment_id, const mEn::Vec4& value) override;

  void clear_depth(float depth) override;
  void clear_stencil(std::uint8_t stencil) override;
  void clear_depth_stencil(float depth, std::uint8_t stencil) override;

 private:
  uint32_t renderer_id_ = 0;
  FramebufferSpec spec_;

  std::vector<FramebufferTextureSpec> color_attachment_specs_;
  std::optional<FramebufferTextureSpec> depth_attachment_spec_;

  std::vector<uint32_t> color_attachments_;
  uint32_t depth_attachment_ = 0;
};

}  // namespace kEn
