#pragma once

#include <glad/gl.h>

#include <cstdint>
#include <filesystem>

#include <kEn/renderer/texture.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief OpenGL 2D texture backed by immutable @c GL_TEXTURE_2D storage.
 *
 * Supports all @ref TextureFormat values by mapping them to the appropriate
 * internal format, pixel format, and pixel type via @c opengl_texture_format.
 * All GL calls use DSA (direct state access) to avoid implicit bind side-effects.
 *
 * When @ref set_data() is called for mip level 0 and the texture has more than
 * one mip level, the remaining levels are regenerated automatically via
 * @c glGenerateTextureMipmap.
 *
 * @note Currently restricted to Tex2D kind with a single layer.
 */
class OpenglTexture2D : public Texture {
 public:
  /**
   * @brief Allocates immutable GPU storage for the given descriptor and applies sampler state.
   *
   * @param desc    Texture shape and format; kind must be Tex2D with depth == 1 and layers == 1.
   * @param sampler Sampler parameters applied once at construction.
   */
  explicit OpenglTexture2D(TextureDesc desc, SamplerDesc sampler = {});
  // TODO(remove): texture should represent gpu data, not an asset!
  /**
   * @brief Loads an image file via stb_image, allocates storage, and uploads the pixel data.
   *
   * The image is flipped vertically on load so that the first row in memory corresponds to
   * the bottom of the image (OpenGL convention).  Only byte-per-channel color formats are
   * accepted (e.g. RGBA8, RGB8).
   *
   * @param path       Path to the image file (any format supported by stb_image).
   * @param sampler    Sampler parameters applied once at construction.
   * @param format     Desired pixel format; must be a non-integer byte-per-channel color format.
   * @param mip_levels Mip level count; use @ref kFullMipChain to generate a full chain.
   */
  explicit OpenglTexture2D(const std::filesystem::path& path, SamplerDesc sampler = {},
                           TextureFormat format = TextureFormat::RGBA8, std::uint32_t mip_levels = kFullMipChain);
  ~OpenglTexture2D() override;

  [[nodiscard]] const TextureDesc& desc() const override { return desc_; }
  void set_data(std::span<const std::byte> data, std::uint32_t mip_level, std::uint32_t layer) override;

  /** @brief Returns the sampler descriptor this texture was created with. */
  [[nodiscard]] const SamplerDesc& sampler_desc() const noexcept { return sampler_desc_; }
  /** @brief Returns the raw OpenGL texture name. */
  [[nodiscard]] GLuint renderer_id() const noexcept { return renderer_id_; }
  /** @brief Returns the OpenGL texture target (always @c GL_TEXTURE_2D). */
  [[nodiscard]] GLenum target() const noexcept { return target_; }

  [[nodiscard]] std::uintptr_t native_handle() const noexcept override {
    return static_cast<std::uintptr_t>(renderer_id_);
  }

  [[nodiscard]] ImTextureID imgui_id() const noexcept override { return static_cast<ImTextureID>(renderer_id_); }

 private:
  /** @brief Creates the GL texture object and allocates immutable storage via @c glTextureStorage2D. */
  void allocate_storage();
  /** @brief Applies filtering, wrap, and comparison state from @ref sampler_desc_ to the GL texture object. */
  void apply_sampler_state();

  TextureDesc desc_;
  SamplerDesc sampler_desc_;
  GLuint renderer_id_ = 0;
  GLenum target_      = GL_TEXTURE_2D;

  GLenum internal_format_ = GL_NONE;
  GLenum upload_format_   = GL_NONE;
  GLenum upload_type_     = GL_NONE;
};

}  // namespace kEn
