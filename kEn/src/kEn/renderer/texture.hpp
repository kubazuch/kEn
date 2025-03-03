#pragma once

#include <filesystem>
#include <kEn/core/core.hpp>
#include <optional>

#define TEXTURE_TYPES(X)                                \
  X(ambient_occlusion, aiTextureType_AMBIENT_OCCLUSION) \
  X(diffuse, aiTextureType_DIFFUSE)                     \
  X(height, aiTextureType_HEIGHT)                       \
  X(normal, aiTextureType_NORMALS)                      \
  X(specular, aiTextureType_SPECULAR)

namespace kEn {

class Texture;

enum class ImageFormat { None = 0, R8, RGB8, RGBA8, RGBA32F };

struct TextureSpec {
  enum class filter { Linear, Nearest };

  enum class wrap { Repeat, Clamp, MirroredRepeat };

  std::optional<uint32_t> width;
  std::optional<uint32_t> height;
  std::optional<ImageFormat> format;

  uint32_t mipmap_levels = 1;

  filter min_filter = filter::Linear;
  filter mag_filter = filter::Linear;
  wrap x_wrap       = wrap::Repeat;
  wrap y_wrap       = wrap::Repeat;

  TextureSpec& set_mipmap_levels(uint32_t l) {
    mipmap_levels = l;
    return *this;
  }
  TextureSpec& set_min_filter(filter f) {
    min_filter = f;
    return *this;
  }
  TextureSpec& set_mag_filter(filter f) {
    mag_filter = f;
    return *this;
  }
  TextureSpec& set_x_wrap(wrap f) {
    x_wrap = f;
    return *this;
  }
  TextureSpec& set_y_wrap(wrap f) {
    y_wrap = f;
    return *this;
  }
};

using texture_type_t = uint8_t;

namespace texture_type {
#define ENUM_ENTRY(name, x) name,
#define CASE_ENTRY(name, x) \
  case name:                \
    return #name;

enum : texture_type_t { TEXTURE_TYPES(ENUM_ENTRY) Last };

inline const char* name_of(const texture_type_t type) {
  switch (type) {
    TEXTURE_TYPES(CASE_ENTRY)
    default:
      return "INVALID";
  }
}
}  // namespace texture_type

class Texture {
 public:
  virtual ~Texture() = default;

  virtual const TextureSpec& get_specification() const = 0;

  virtual uint32_t width() const       = 0;
  virtual uint32_t height() const      = 0;
  virtual uint32_t renderer_id() const = 0;

  virtual const std::filesystem::path& path() const   = 0;
  virtual void set_data(void* data, uint32_t size)    = 0;
  virtual void bind(uint32_t slot) const              = 0;
  virtual bool is_loaded() const                      = 0;
  virtual bool operator==(const Texture& other) const = 0;
};

class Texture2D : public Texture {
 public:
  static std::shared_ptr<Texture2D> create(const TextureSpec& spec);
  static std::shared_ptr<Texture2D> create(const std::filesystem::path& name, const TextureSpec& spec = TextureSpec());

  static const std::filesystem::path kTexturePath;

  virtual void imgui() = 0;

 private:
  static std::unordered_map<std::filesystem::path, std::shared_ptr<Texture2D>> loaded_resources_;
};

}  // namespace kEn
