#pragma once

#include <glad/gl.h>

#include <cstdint>
#include <filesystem>

#include <kEn/renderer/texture.hpp>

namespace kEn {

class OpenglTexture2D : public Texture {
 public:
  explicit OpenglTexture2D(TextureDesc desc, SamplerDesc sampler = {});
  // TODO(remove): texture should represent gpu data, not an asset!
  explicit OpenglTexture2D(const std::filesystem::path& path, SamplerDesc sampler = {},
                           TextureFormat format = TextureFormat::RGBA8, std::uint32_t mip_levels = kFullMipChain);
  ~OpenglTexture2D() override;

  [[nodiscard]] const TextureDesc& desc() const override { return desc_; }
  void set_data(std::span<const std::byte> data, std::uint32_t mip_level, std::uint32_t layer) override;

  [[nodiscard]] const SamplerDesc& sampler_desc() const noexcept { return sampler_desc_; }
  [[nodiscard]] GLuint renderer_id() const noexcept { return renderer_id_; }
  [[nodiscard]] GLenum target() const noexcept { return target_; }

  void bind(std::uint32_t slot) const noexcept override;
  [[nodiscard]] ImTextureID imgui_id() const noexcept override { return static_cast<ImTextureID>(renderer_id_); }
  void bind() const noexcept { bind(0); }

 private:
  void allocate_storage();
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
