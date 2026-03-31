#pragma once

#include <cstdint>
#include <span>
#include <vector>

#include <mEn/vec4.hpp>

#include <kEn/core/core.hpp>
#include <kEn/renderer/attachment_handle.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture_format.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief Describes a single texture attachment within a framebuffer.
 *
 * Specifies the pixel format and optional usage flags that control how the
 * underlying texture is allocated and which pipeline operations are permitted
 * on it at runtime.
 */
struct FramebufferTextureSpec {
  FramebufferTextureSpec() = default;

  /** @brief Construct with the given @p format; all usage flags take their defaults. */
  FramebufferTextureSpec(TextureFormat format)  // NOLINT(google-explicit-constructor)
      : texture_format(format) {}

  /**
   * @brief Construct with explicit format and usage flags.
   * @param format           Pixel format of the texture.
   * @param shader_readable  Allow binding this attachment as a shader-readable texture.
   * @param unordered_access Allow unordered-access (UAV/image) reads and writes from shaders.
   * @param cpu_readback     Allow reading the attachment contents back to CPU memory.
   */
  FramebufferTextureSpec(TextureFormat format, bool shader_readable, bool unordered_access, bool cpu_readback)
      : texture_format(format),
        shader_readable(shader_readable),
        unordered_access(unordered_access),
        cpu_readback(cpu_readback) {}

  TextureFormat texture_format{}; /**< Pixel/texel format of the attachment. */
  bool shader_readable  = true;   /**< Whether the attachment can be sampled in shaders. */
  bool unordered_access = false;  /**< Whether unordered-access (image) operations are allowed. */
  bool cpu_readback     = false;  /**< Whether CPU readback via @ref Framebuffer::read_pixels is allowed. */
};

/**
 * @brief Groups all attachment specifications for a @ref Framebuffer.
 *
 * A framebuffer may have zero or more color attachments and at most one
 * depth/stencil attachment.  Both are described by @ref FramebufferTextureSpec
 * entries that carry the format and usage flags.
 */
struct FramebufferAttachmentSpec {
  std::vector<FramebufferTextureSpec> color_attachments;  /**< Ordered list of color attachment specs. */
  std::optional<FramebufferTextureSpec> depth_attachment; /**< Optional depth (or depth-stencil) attachment spec. */
};

/**
 * @brief Complete creation parameters for a @ref Framebuffer.
 *
 * Passed to the platform-specific factory at construction time.  All dimension
 * and sample fields must be filled in before use; the @ref attachments field
 * describes every texture that will be allocated.
 */
struct FramebufferSpec {
  std::uint32_t width      = 0; /**< Framebuffer width in pixels. */
  std::uint32_t height     = 0; /**< Framebuffer height in pixels. */
  std::uint32_t samples    = 1; /**< MSAA sample count; 1 means no multisampling. */
  std::uint32_t mip_levels = 1; /**< Number of mip levels (currently only 1 is supported). */
  std::uint32_t array_size = 1; /**< Texture array layers (currently only 1 is supported). */

  FramebufferAttachmentSpec attachments; /**< Color and optional depth attachment specifications. */
};

/**
 * @brief Abstract off-screen render target.
 *
 * Owns one or more texture attachments (color and/or depth/stencil) and
 * exposes a graphics-API-agnostic interface for binding, clearing, resizing,
 * and reading back pixel data.  Concrete implementations (e.g.
 * @ref OpenglFramebuffer) are created by the renderer factory.
 *
 * ### Typical usage
 * @code
 * FramebufferSpec spec;
 * spec.width  = 1280;
 * spec.height = 720;
 * spec.attachments.color_attachments = {TextureFormat::RGBA8};
 * spec.attachments.depth_attachment  = TextureFormat::Depth24Stencil8;
 *
 * auto fb = device().create_framebuffer(spec);   // via the active Device
 *
 * device().context().set_render_target(*fb);              // bind as render target
 * fb->clear_color_attachment(0, mEn::Vec4{0, 0, 0, 1});
 * fb->clear_depth();
 * // ... draw calls via device().context() ...
 * device().context().bind_default_framebuffer();          // restore default render target
 * auto id = fb->read_pixel<std::int32_t>(0, mouse_x, mouse_y);
 * @endcode
 */
class Framebuffer {
 public:
  Framebuffer()          = default;
  virtual ~Framebuffer() = default;

  /** @brief Return the platform-native framebuffer handle. */
  [[nodiscard]] virtual std::uintptr_t native_handle() const noexcept = 0;

  /** @brief Return the handle for the depth attachment texture, or @c std::nullopt if absent. */
  [[nodiscard]] virtual std::optional<AttachmentHandle> depth_attachment() const noexcept = 0;

  /**
   * @brief Resize the framebuffer and recreate its attachments.
   *
   * Destroys all existing GPU resources and reallocates them at the new
   * dimensions.  Has no effect and logs a warning if either dimension is 0 or
   * exceeds the platform maximum.
   * @param width   New width in pixels.
   * @param height  New height in pixels.
   */
  virtual void resize(std::uint32_t width, std::uint32_t height) = 0;

  /** @brief Return the creation spec this framebuffer was built from. */
  [[nodiscard]] virtual const FramebufferSpec& spec() const = 0;

  /** @brief Return the number of color attachments. */
  [[nodiscard]] virtual std::size_t color_attachment_count() const = 0;

  /**
   * @brief Return the @ref TextureFormat of color attachment @p attachment_id.
   * @param attachment_id  Zero-based index; must be less than @ref color_attachment_count().
   */
  [[nodiscard]] virtual TextureFormat color_attachment_format(std::uint32_t attachment_id) const = 0;

  /** @brief Return @c true if a depth (or depth-stencil) attachment was specified. */
  [[nodiscard]] virtual bool has_depth_attachment() const = 0;

  /**
   * @brief Return the @ref TextureFormat of the depth attachment, or @c std::nullopt if absent.
   * @see has_depth_attachment()
   */
  [[nodiscard]] virtual std::optional<TextureFormat> depth_attachment_format() const = 0;

  /**
   * @brief Copy a rectangular region of a color attachment into a caller-supplied buffer.
   *
   * Rows in @p out_buffer are laid out with a stride of @p out_row_pitch bytes.
   * When @p out_row_pitch is 0 the rows are assumed to be tightly packed (stride
   * equals @c width * bytes_per_pixel).
   *
   * @param attachment_id  Zero-based color attachment index.
   * @param x              Left edge of the region (texel coordinates).
   * @param y              Bottom edge of the region (texel coordinates, origin at bottom-left).
   * @param width          Width of the region in texels.
   * @param height         Height of the region in texels.
   * @param out_buffer     Destination byte span; must be large enough to hold the requested data.
   * @param out_row_pitch  Output row stride in bytes, or 0 for tightly-packed rows.
   *
   * @note Not supported on multisampled attachments.
   */
  virtual void read_pixels(std::uint32_t attachment_id, int x, int y, int width, int height,
                           std::span<std::byte> out_buffer, std::size_t out_row_pitch) const = 0;

  /**
   * @brief Convenience overload that reads tightly-packed rows (@p out_row_pitch = 0).
   * @see read_pixels(std::uint32_t, int, int, int, int, std::span<std::byte>, std::size_t)
   */
  void read_pixels(std::uint32_t attachment_id, int x, int y, int width, int height,
                   std::span<std::byte> out_buffer) const {
    read_pixels(attachment_id, x, y, width, height, out_buffer, 0);
  }

  /**
   * @brief Read a single texel from a color attachment and return it as @p T.
   *
   * @tparam T  Trivially copyable type whose size matches the attachment's bytes-per-pixel.
   * @param attachment_id  Zero-based color attachment index.
   * @param x              Texel column (origin at left).
   * @param y              Texel row (origin at bottom-left).
   * @return The texel value reinterpreted as @p T.
   */
  template <typename T>
  [[nodiscard]] T read_pixel(std::uint32_t attachment_id, int x, int y) const {
    static_assert(std::is_trivially_copyable_v<T>, "read_pixel<T>() requires a trivially copyable type");

    T value{};
    read_pixels(attachment_id, x, y, 1, 1, std::as_writable_bytes(std::span<T>(&value, 1)));

    return value;
  }

  /**
   * @brief Return a handle for a color attachment texture.
   *
   * Use @ref AttachmentHandle::imgui_id() to pass the result to ImGui::Image(), or
   * pass it directly to @ref RenderContext::bind_attachment to sample it in a shader.
   * @param attachment_id  Zero-based color attachment index.
   */
  [[nodiscard]] virtual AttachmentHandle color_attachment(std::uint32_t attachment_id) const = 0;

  /**
   * @brief Clear a color attachment to a uniform signed integer value.
   * @param attachment_id  Zero-based color attachment index.
   * @param value          Fill value broadcast to all channels.
   */
  virtual void clear_color_attachment(std::uint32_t attachment_id, std::int32_t value) = 0;

  /**
   * @brief Clear a color attachment to a uniform unsigned integer value.
   * @param attachment_id  Zero-based color attachment index.
   * @param value          Fill value broadcast to all channels.
   */
  virtual void clear_color_attachment(std::uint32_t attachment_id, std::uint32_t value) = 0;

  /**
   * @brief Clear a color attachment to a uniform float value.
   * @param attachment_id  Zero-based color attachment index.
   * @param value          Fill value broadcast to all channels.
   */
  virtual void clear_color_attachment(std::uint32_t attachment_id, float value) = 0;

  /**
   * @brief Clear a color attachment to a four-component RGBA color.
   * @param attachment_id  Zero-based color attachment index.
   * @param value          RGBA fill color.
   */
  virtual void clear_color_attachment(std::uint32_t attachment_id, const mEn::Vec4& value) = 0;

  /**
   * @brief Clear the depth buffer to @p depth.
   * @param depth  Value to write into every depth sample; must be in [0, 1].
   */
  virtual void clear_depth(float depth) = 0;

  /** @brief Clear the depth buffer to 1.0 (far plane). */
  void clear_depth() { clear_depth(1.0F); }

  /**
   * @brief Clear the stencil buffer to @p stencil.
   * @param stencil  8-bit stencil clear value.
   */
  virtual void clear_stencil(std::uint8_t stencil) = 0;

  /** @brief Clear the stencil buffer to 0. */
  void clear_stencil() { clear_stencil(0); }

  /**
   * @brief Clear the combined depth-stencil attachment in a single call.
   * @param depth    Depth clear value in [0, 1].
   * @param stencil  8-bit stencil clear value.
   */
  virtual void clear_depth_stencil(float depth, std::uint8_t stencil) = 0;

  /**
   * @brief Clear the depth component to @p depth and the stencil to 0.
   * @param depth  Depth clear value in [0, 1]; defaults to 1.0 (far plane).
   */
  void clear_depth_stencil(float depth = 1.0F) { clear_depth_stencil(depth, 0); }

  DELETE_COPY_MOVE(Framebuffer);
};

}  // namespace kEn
