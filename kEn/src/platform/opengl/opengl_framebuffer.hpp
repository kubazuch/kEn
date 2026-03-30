#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

#include <kEn/core/assert.hpp>
#include <kEn/renderer/framebuffer.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief OpenGL implementation of @ref Framebuffer.
 *
 * Manages an OpenGL framebuffer object (FBO) together with its color and
 * depth/stencil texture attachments.  The FBO and all associated textures are
 * created by @ref invalidate(), which is called automatically on construction
 * and after every @ref resize().
 *
 * @note Currently supports only `mip_levels == 1` and `array_size == 1`.
 *       Up to four simultaneous color attachments are supported.
 */
class OpenglFramebuffer : public Framebuffer {
 public:
  /**
   * @brief Construct and allocate GPU resources for the given @p spec.
   *
   * Calls @ref invalidate() internally to create the FBO and all attachment
   * textures.
   * @param spec  Complete framebuffer specification.
   */
  explicit OpenglFramebuffer(FramebufferSpec spec);

  /** @brief Destroy the FBO and all attachment textures. */
  ~OpenglFramebuffer() override;

  /**
   * @brief (Re)create the OpenGL FBO and attachment textures from the current spec.
   *
   * If an FBO already exists its GPU resources are deleted before new ones are
   * allocated.  Called automatically by the constructor and by @ref resize().
   * May also be called manually when attachment specifications change without a
   * dimension change.
   */
  void invalidate();

  void resize(std::uint32_t width, std::uint32_t height) override;

  [[nodiscard]] std::uintptr_t native_handle() const noexcept override {
    return static_cast<std::uintptr_t>(renderer_id_);
  }

  [[nodiscard]] std::uintptr_t depth_attachment() const noexcept override {
    return static_cast<std::uintptr_t>(depth_attachment_);
  }

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

  [[nodiscard]] std::uintptr_t color_attachment(std::uint32_t attachment_id) const override;

  void clear_color_attachment(std::uint32_t attachment_id, std::int32_t value) override;
  void clear_color_attachment(std::uint32_t attachment_id, std::uint32_t value) override;
  void clear_color_attachment(std::uint32_t attachment_id, float value) override;
  void clear_color_attachment(std::uint32_t attachment_id, const mEn::Vec4& value) override;

  void clear_depth(float depth) override;
  void clear_stencil(std::uint8_t stencil) override;
  void clear_depth_stencil(float depth, std::uint8_t stencil) override;

 private:
  uint32_t renderer_id_ = 0; /**< OpenGL framebuffer object name. */
  FramebufferSpec spec_;     /**< Creation spec; width/height updated on resize. */

  std::vector<FramebufferTextureSpec> color_attachment_specs_;  /**< Per-attachment format and usage flags. */
  std::optional<FramebufferTextureSpec> depth_attachment_spec_; /**< Depth/stencil spec, absent if none requested. */

  std::vector<uint32_t> color_attachments_; /**< OpenGL texture names for each color attachment. */
  uint32_t depth_attachment_ = 0;           /**< OpenGL texture name for the depth attachment (0 if absent). */
};

}  // namespace kEn
