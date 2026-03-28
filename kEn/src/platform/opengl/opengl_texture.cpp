#include "opengl_texture.hpp"

#include <glad/gl.h>
#include <stb_image.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <span>

#include <kEn/core/assert.hpp>
#include <kEn/core/log.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/renderer/texture_format.hpp>

#include "opengl_texture_format.hpp"

namespace kEn {

namespace {

[[nodiscard]] GLint opengl_min_filter(SamplerDesc::Filter filter, SamplerDesc::MipFilter mip_filter) noexcept {
  const bool nearest = filter == SamplerDesc::Filter::Nearest;
  switch (mip_filter) {
    case SamplerDesc::MipFilter::None:
      return nearest ? GL_NEAREST : GL_LINEAR;
    case SamplerDesc::MipFilter::Nearest:
      return nearest ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_NEAREST;
    case SamplerDesc::MipFilter::Linear:
      return nearest ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_LINEAR;
  }

  KEN_UNREACHABLE();
}

[[nodiscard]] GLint opengl_mag_filter(SamplerDesc::Filter filter) noexcept {
  return filter == SamplerDesc::Filter::Nearest ? GL_NEAREST : GL_LINEAR;
}

[[nodiscard]] GLint opengl_wrap(SamplerDesc::Wrap wrap) noexcept {
  switch (wrap) {
    case SamplerDesc::Wrap::Clamp:
      return GL_CLAMP_TO_EDGE;
    case SamplerDesc::Wrap::MirroredRepeat:
      return GL_MIRRORED_REPEAT;
    case SamplerDesc::Wrap::Repeat:
    default:
      return GL_REPEAT;
  }
}

[[nodiscard]] GLint opengl_compare_op(SamplerDesc::CompareOp op) noexcept {
  switch (op) {
    case SamplerDesc::CompareOp::Never:
      return GL_NEVER;
    case SamplerDesc::CompareOp::Less:
      return GL_LESS;
    case SamplerDesc::CompareOp::Equal:
      return GL_EQUAL;
    case SamplerDesc::CompareOp::LessEqual:
      return GL_LEQUAL;
    case SamplerDesc::CompareOp::Greater:
      return GL_GREATER;
    case SamplerDesc::CompareOp::NotEqual:
      return GL_NOTEQUAL;
    case SamplerDesc::CompareOp::GreaterEqual:
      return GL_GEQUAL;
    case SamplerDesc::CompareOp::Always:
    default:
      return GL_ALWAYS;
  }
}

[[nodiscard]] constexpr bool is_byte_color_format(TextureFormat format) noexcept {
  return texture_format::is_color_format(format) && !texture_format::is_integer_format(format) &&
         texture_format::bytes_per_block(format) == texture_format::color_channel_count(format);
}

}  // namespace

OpenglTexture2D::OpenglTexture2D(TextureDesc desc, SamplerDesc sampler) : desc_(desc), sampler_desc_(sampler) {
  KEN_ASSERT(desc_.kind == TextureKind::Tex2D);
  KEN_ASSERT(desc_.width > 0);
  KEN_ASSERT(desc_.height > 0);
  KEN_ASSERT(desc_.depth == 1);
  KEN_ASSERT(desc_.layers == 1);
  KEN_ASSERT(desc_.has_valid_mip_request());

  allocate_storage();
  apply_sampler_state();
}

OpenglTexture2D::OpenglTexture2D(const std::filesystem::path& path, SamplerDesc sampler, TextureFormat format,
                                 std::uint32_t mip_levels)
    : sampler_desc_(sampler) {
  KEN_ASSERT(is_byte_color_format(format));

  int width    = 0;
  int height   = 0;
  int channels = 0;

  const int desired_channels = static_cast<int>(texture_format::color_channel_count(format));

  stbi_set_flip_vertically_on_load(1);
  stbi_uc* data = stbi_load(path.string().c_str(), &width, &height, &channels, desired_channels);

  if (data == nullptr) {
    KEN_CORE_CRITICAL("stb_image load error: {0}", stbi_failure_reason());
    return;
  }

  KEN_ASSERT(width > 0);
  KEN_ASSERT(height > 0);

  desc_ = TextureDesc::texture_2d(static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height), format,
                                  mip_levels);

  KEN_ASSERT(desc_.has_valid_mip_request());

  allocate_storage();
  apply_sampler_state();

  const std::size_t size_bytes = static_cast<std::size_t>(desc_.width) * static_cast<std::size_t>(desc_.height) *
                                 static_cast<std::size_t>(texture_format::bytes_per_block(desc_.format));

  set_data(std::span<const std::byte>{reinterpret_cast<const std::byte*>(data), size_bytes}, 0, 0);

  stbi_image_free(data);

  KEN_CORE_INFO("Loaded texture '{}' ({}x{}, source channels: {}, requested channels: {})", path.string(), width,
                height, channels, desired_channels);
}

OpenglTexture2D::~OpenglTexture2D() { glDeleteTextures(1, &renderer_id_); }

void OpenglTexture2D::set_data(std::span<const std::byte> data, std::uint32_t mip_level,
                               [[maybe_unused]] std::uint32_t layer) {
  KEN_ASSERT(renderer_id_ != 0);
  KEN_ASSERT(layer == 0);
  KEN_ASSERT(mip_level < desc_.resolved_mip_levels());

  const std::uint32_t mip_width  = std::max(1U, desc_.width >> mip_level);
  const std::uint32_t mip_height = std::max(1U, desc_.height >> mip_level);

  const std::size_t expected_size = static_cast<std::size_t>(mip_width) * static_cast<std::size_t>(mip_height) *
                                    static_cast<std::size_t>(texture_format::bytes_per_block(desc_.format));

  KEN_CORE_ASSERT(data.size_bytes() == expected_size, "Data must match the target mip size");

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTextureSubImage2D(renderer_id_, static_cast<GLint>(mip_level), 0, 0, static_cast<GLsizei>(mip_width),
                      static_cast<GLsizei>(mip_height), upload_format_, upload_type_, data.data());

  if (mip_level == 0 && desc_.resolved_mip_levels() > 1) {
    glGenerateTextureMipmap(renderer_id_);
  }
}

void OpenglTexture2D::bind(std::uint32_t slot) const noexcept { glBindTextureUnit(slot, renderer_id_); }

void OpenglTexture2D::allocate_storage() {
  KEN_ASSERT(renderer_id_ == 0);

  internal_format_ = texture_format::internal_format(desc_.format);
  upload_format_   = texture_format::pixel_format(desc_.format);
  upload_type_     = texture_format::pixel_type(desc_.format);

  glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
  glTextureStorage2D(renderer_id_, static_cast<GLsizei>(desc_.resolved_mip_levels()), internal_format_,
                     static_cast<GLsizei>(desc_.width), static_cast<GLsizei>(desc_.height));
}

void OpenglTexture2D::apply_sampler_state() {
  KEN_ASSERT(renderer_id_ != 0);

  const auto mip_filter = desc_.resolved_mip_levels() > 1 ? sampler_desc_.mip_filter : SamplerDesc::MipFilter::None;

  glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, opengl_min_filter(sampler_desc_.min_filter, mip_filter));
  glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, opengl_mag_filter(sampler_desc_.mag_filter));

  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, opengl_wrap(sampler_desc_.wrap_u));
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, opengl_wrap(sampler_desc_.wrap_v));

  glTextureParameteri(renderer_id_, GL_TEXTURE_COMPARE_MODE,
                      sampler_desc_.compare_enabled ? GL_COMPARE_REF_TO_TEXTURE : GL_NONE);
  glTextureParameteri(renderer_id_, GL_TEXTURE_COMPARE_FUNC, opengl_compare_op(sampler_desc_.compare_op));
}

}  // namespace kEn
