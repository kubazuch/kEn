#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <unordered_map>
#include <utility>

#include <kEn/core/core.hpp>
#include <kEn/renderer/texture_format.hpp>
#include <kEn/util/enum_map.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

struct TextureSpec {
  enum class filter : uint8_t { Linear, Nearest };

  enum class wrap : uint8_t { Repeat, Clamp, MirroredRepeat };

  std::optional<uint32_t> width;
  std::optional<uint32_t> height;
  std::optional<TextureFormat> format;

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

/**
 * @brief Strongly-typed texture semantic category.
 */
enum class TextureType : std::uint8_t { AmbientOcclusion, Diffuse, Height, Normal, Specular, Count };

namespace texture_type {

/** @brief C++20 convenience: brings enumerators into `kEn::texture_type` namespace. */
using enum TextureType;

inline constexpr util::EnumMap kNames{{
    std::pair{AmbientOcclusion, "ambient_occlusion"},
    std::pair{Diffuse, "diffuse"},
    std::pair{Height, "height"},
    std::pair{Normal, "normal"},
    std::pair{Specular, "specular"},
}};

/**
 * @brief Get a stable, human-readable name for a texture type.
 * @param type Texture type enum value.
 * @return A string view describing the type (e.g. @c "diffuse").
 */
[[nodiscard]] constexpr std::string_view name_of(TextureType type) noexcept { return kNames[type]; }

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
