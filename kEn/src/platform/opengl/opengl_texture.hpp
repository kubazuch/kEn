#pragma once

#include <glad/gl.h>

#include <filesystem>
#include <kEn/renderer/texture.hpp>

namespace kEn {

class OpenglTexture2D : public Texture2D {
 public:
  explicit OpenglTexture2D(const TextureSpec& specification);
  explicit OpenglTexture2D(const std::filesystem::path& path, const TextureSpec& specification = TextureSpec());
  virtual ~OpenglTexture2D() override;

  const TextureSpec& get_specification() const override { return spec_; }
  uint32_t width() const override { return spec_.width.value(); }
  uint32_t height() const override { return spec_.height.value(); }
  uint32_t renderer_id() const override { return renderer_id_; }
  const std::filesystem::path& path() const override { return path_; }
  void set_data(void* data, uint32_t size) override;
  void bind(uint32_t slot) const override;
  bool is_loaded() const override { return is_loaded_; }
  bool operator==(const Texture& other) const override { return renderer_id_ == other.renderer_id(); }

  void imgui() override;

 private:
  TextureSpec spec_;
  std::filesystem::path path_;
  bool is_loaded_ = false;
  uint32_t renderer_id_;
  GLenum internal_format_, data_format_;
};

}  // namespace kEn
