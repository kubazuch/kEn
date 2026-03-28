#pragma once

#include <cstdint>
#include <span>
#include <vector>

#include <mEn/vec4.hpp>

#include <kEn/core/core.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture_format.hpp>

namespace kEn {

struct FramebufferTextureSpec {
  FramebufferTextureSpec() = default;

  FramebufferTextureSpec(TextureFormat format)  // NOLINT(google-explicit-constructor)
      : texture_format(format) {}

  FramebufferTextureSpec(TextureFormat format, bool shader_readable, bool unordered_access, bool cpu_readback)
      : texture_format(format),
        shader_readable(shader_readable),
        unordered_access(unordered_access),
        cpu_readback(cpu_readback) {}

  TextureFormat texture_format{};
  bool shader_readable  = true;
  bool unordered_access = false;
  bool cpu_readback     = false;
};

struct FramebufferAttachmentSpec {
  std::vector<FramebufferTextureSpec> color_attachments;
  std::optional<FramebufferTextureSpec> depth_attachment;
};

struct FramebufferSpec {
  std::uint32_t width      = 0;
  std::uint32_t height     = 0;
  std::uint32_t samples    = 1;
  std::uint32_t mip_levels = 1;
  std::uint32_t array_size = 1;

  FramebufferAttachmentSpec attachments;
};

class Framebuffer {
 public:
  virtual ~Framebuffer() = default;

  virtual void bind_for_rendering() = 0;

  virtual void resize(std::uint32_t width, std::uint32_t height) = 0;

  [[nodiscard]] virtual const FramebufferSpec& spec() const = 0;

  [[nodiscard]] virtual std::size_t color_attachment_count() const                               = 0;
  [[nodiscard]] virtual TextureFormat color_attachment_format(std::uint32_t attachment_id) const = 0;
  [[nodiscard]] virtual bool has_depth_attachment() const                                        = 0;
  [[nodiscard]] virtual std::optional<TextureFormat> depth_attachment_format() const             = 0;

  virtual void read_pixels(std::uint32_t attachment_id, int x, int y, int width, int height,
                           std::span<std::byte> out_buffer, std::size_t out_row_pitch) const = 0;
  void read_pixels(std::uint32_t attachment_id, int x, int y, int width, int height,
                   std::span<std::byte> out_buffer) const {
    read_pixels(attachment_id, x, y, width, height, out_buffer, 0);
  }

  template <typename T>
  [[nodiscard]] T read_pixel(std::uint32_t attachment_id, int x, int y) const {
    static_assert(std::is_trivially_copyable_v<T>, "read_pixel<T>() requires a trivially copyable type");

    T value{};
    read_pixels(attachment_id, x, y, 1, 1, std::as_writable_bytes(std::span<T>(&value, 1)));

    return value;
  }

  [[nodiscard]] virtual std::uintptr_t native_color_attachment_handle(std::uint32_t attachment_id) const = 0;

  virtual void bind_color_attachment_as_texture(std::uint32_t attachment_id, ShaderStage stage,
                                                std::uint32_t slot) const = 0;

  virtual void bind_depth_as_texture(ShaderStage stage, std::uint32_t slot) const = 0;

  virtual void clear_color_attachment(std::uint32_t attachment_id, std::int32_t value)     = 0;
  virtual void clear_color_attachment(std::uint32_t attachment_id, std::uint32_t value)    = 0;
  virtual void clear_color_attachment(std::uint32_t attachment_id, float value)            = 0;
  virtual void clear_color_attachment(std::uint32_t attachment_id, const mEn::Vec4& value) = 0;

  virtual void clear_depth(float depth) = 0;
  void clear_depth() { clear_depth(1.0F); }

  virtual void clear_stencil(std::uint8_t stencil) = 0;
  void clear_stencil() { clear_stencil(0); }

  virtual void clear_depth_stencil(float depth, std::uint8_t stencil) = 0;
  void clear_depth_stencil(float depth = 1.0F) { clear_depth_stencil(depth, 0); }
};

}  // namespace kEn
