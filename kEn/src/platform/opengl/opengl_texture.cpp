#include "opengl_texture.hpp"

#include <glad/gl.h>
#include <stb_image.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <span>
#include <utility>  // IWYU pragma: keep

#include <kEn/core/assert.hpp>
#include <kEn/core/log.hpp>
#include <kEn/core/profile.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/renderer/texture_format.hpp>

#include "opengl_profile.hpp"
#include "opengl_state.hpp"
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

[[nodiscard, maybe_unused]] constexpr bool is_byte_color_format(TextureFormat format) noexcept {
  return texture_format::is_color_format(format) && !texture_format::is_integer_format(format) &&
         texture_format::bytes_per_block(format) == texture_format::color_channel_count(format);
}

[[nodiscard, maybe_unused]] constexpr bool region_inside(const TextureRegion& region, const TextureDesc& desc,
                                                         std::uint32_t mip_level) noexcept {
  const std::uint32_t width  = std::max(1U, desc.width >> mip_level);
  const std::uint32_t height = std::max(1U, desc.height >> mip_level);
  const std::uint32_t depth  = desc.kind == TextureKind::Tex3D ? std::max(1U, desc.depth >> mip_level) : 1U;
  return region.width > 0 && region.height > 0 && region.depth > 0 && region.x <= width &&
         region.width <= width - region.x && region.y <= height && region.height <= height - region.y &&
         region.z <= depth && region.depth <= depth - region.z;
}

[[nodiscard]] constexpr std::size_t region_bytes(const TextureRegion& region, TextureFormat format) noexcept {
  return static_cast<std::size_t>(region.width) * static_cast<std::size_t>(region.height) *
         static_cast<std::size_t>(region.depth) * static_cast<std::size_t>(texture_format::bytes_per_block(format));
}

[[nodiscard, maybe_unused]] constexpr std::size_t region_span_bytes(const TextureRegion& region,
                                                                    std::uint32_t row_length,
                                                                    TextureFormat format) noexcept {
  const auto rows = (static_cast<std::size_t>(region.depth) * static_cast<std::size_t>(region.height)) - 1;
  return ((rows * row_length) + region.width) * static_cast<std::size_t>(texture_format::bytes_per_block(format));
}

/** @brief Reads @p region of level @p mip_level, slices from @p z on, tightly packed into @p data. */
void read_texture_region(GLuint renderer_id, TextureFormat format, std::span<std::byte> data,
                         const TextureRegion& region, std::uint32_t mip_level, std::uint32_t z) {
  KEN_CORE_ASSERT(data.size_bytes() >= region_bytes(region, format), "Data must cover the region");

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glGetTextureSubImage(renderer_id, static_cast<GLint>(mip_level), static_cast<GLint>(region.x),
                       static_cast<GLint>(region.y), static_cast<GLint>(z), static_cast<GLsizei>(region.width),
                       static_cast<GLsizei>(region.height), static_cast<GLsizei>(region.depth),
                       texture_format::pixel_format(format), texture_format::pixel_type(format),
                       static_cast<GLsizei>(data.size_bytes()), data.data());
}

}  // namespace

OpenglTexture2D::OpenglTexture2D(TextureDesc desc, SamplerDesc sampler) : desc_(desc), sampler_desc_(sampler) {
  KEN_CORE_ASSERT(desc_.kind == TextureKind::Tex2D);
  KEN_CORE_ASSERT(desc_.width > 0);
  KEN_CORE_ASSERT(desc_.height > 0);
  KEN_CORE_ASSERT(desc_.depth == 1);
  KEN_CORE_ASSERT(desc_.layers == 1);
  KEN_CORE_ASSERT(desc_.has_valid_mip_request());

  allocate_storage();
  apply_sampler_state();
}

OpenglTexture2D::OpenglTexture2D(const std::filesystem::path& path, SamplerDesc sampler, TextureFormat format,
                                 std::uint32_t mip_levels)
    : sampler_desc_(sampler) {
  KEN_PROFILE_FUNCTION();
  KEN_CORE_ASSERT(is_byte_color_format(format));

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

  KEN_CORE_ASSERT(width > 0);
  KEN_CORE_ASSERT(height > 0);

  desc_ = TextureDesc::texture_2d(static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height), format,
                                  mip_levels);

  KEN_CORE_ASSERT(desc_.has_valid_mip_request());

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
  KEN_PROFILE_FUNCTION();
  KEN_PROFILE_GPU_SCOPE("OpenglTexture2D::set_data");
  KEN_CORE_ASSERT(renderer_id_ != 0);
  KEN_CORE_ASSERT(layer == 0);
  KEN_CORE_ASSERT(mip_level < desc_.resolved_mip_levels());

  const std::uint32_t mip_width  = std::max(1U, desc_.width >> mip_level);
  const std::uint32_t mip_height = std::max(1U, desc_.height >> mip_level);

  [[maybe_unused]] const std::size_t expected_size =
      static_cast<std::size_t>(mip_width) * static_cast<std::size_t>(mip_height) *
      static_cast<std::size_t>(texture_format::bytes_per_block(desc_.format));

  KEN_CORE_ASSERT(data.size_bytes() == expected_size, "Data must match the target mip size");

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  profile::count_upload(data.size_bytes());
  glTextureSubImage2D(renderer_id_, static_cast<GLint>(mip_level), 0, 0, static_cast<GLsizei>(mip_width),
                      static_cast<GLsizei>(mip_height), upload_format_, upload_type_, data.data());

  if (mip_level == 0 && desc_.resolved_mip_levels() > 1) {
    glGenerateTextureMipmap(renderer_id_);
  }
}

void OpenglTexture2D::set_region(std::span<const std::byte> data, const TextureRegion& region, std::uint32_t row_length,
                                 std::uint32_t mip_level, [[maybe_unused]] std::uint32_t layer) {
  KEN_PROFILE_FUNCTION();
  KEN_PROFILE_GPU_SCOPE("OpenglTexture2D::set_region");
  KEN_CORE_ASSERT(renderer_id_ != 0);
  KEN_CORE_ASSERT(layer == 0);
  KEN_CORE_ASSERT(mip_level < desc_.resolved_mip_levels());

  const std::uint32_t row = row_length == 0 ? region.width : row_length;

  KEN_CORE_ASSERT(region_inside(region, desc_, mip_level), "Region must lie inside the target mip");
  KEN_CORE_ASSERT(row >= region.width);
  KEN_CORE_ASSERT(data.size_bytes() >= region_span_bytes(region, row, desc_.format), "Data must cover the region");

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, static_cast<GLint>(row));

  profile::count_upload(region_bytes(region, desc_.format));
  glTextureSubImage2D(renderer_id_, static_cast<GLint>(mip_level), static_cast<GLint>(region.x),
                      static_cast<GLint>(region.y), static_cast<GLsizei>(region.width),
                      static_cast<GLsizei>(region.height), upload_format_, upload_type_, data.data());

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

  if (mip_level == 0 && desc_.resolved_mip_levels() > 1) {
    glGenerateTextureMipmap(renderer_id_);
  }
}

void OpenglTexture2D::read_region(std::span<std::byte> data, const TextureRegion& region, std::uint32_t mip_level,
                                  [[maybe_unused]] std::uint32_t layer) const {
  KEN_PROFILE_FUNCTION();
  KEN_PROFILE_GPU_SCOPE("OpenglTexture2D::read_region");
  KEN_CORE_ASSERT(renderer_id_ != 0);
  KEN_CORE_ASSERT(layer == 0);
  KEN_CORE_ASSERT(mip_level < desc_.resolved_mip_levels());
  KEN_CORE_ASSERT(region_inside(region, desc_, mip_level), "Region must lie inside the source mip");

  read_texture_region(renderer_id_, desc_.format, data, region, mip_level, 0);
}

void OpenglTexture2D::allocate_storage() {
  KEN_CORE_ASSERT(renderer_id_ == 0);

  internal_format_ = texture_format::internal_format(desc_.format);
  upload_format_   = texture_format::pixel_format(desc_.format);
  upload_type_     = texture_format::pixel_type(desc_.format);

  glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
  glTextureStorage2D(renderer_id_, static_cast<GLsizei>(desc_.resolved_mip_levels()), internal_format_,
                     static_cast<GLsizei>(desc_.width), static_cast<GLsizei>(desc_.height));
}

void OpenglTexture2D::apply_sampler_state() {
  KEN_CORE_ASSERT(renderer_id_ != 0);

  const auto mip_filter = desc_.resolved_mip_levels() > 1 ? sampler_desc_.mip_filter : SamplerDesc::MipFilter::None;

  glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, opengl_min_filter(sampler_desc_.min_filter, mip_filter));
  glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, opengl_mag_filter(sampler_desc_.mag_filter));

  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, opengl_wrap(sampler_desc_.wrap_u));
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, opengl_wrap(sampler_desc_.wrap_v));

  glTextureParameteri(renderer_id_, GL_TEXTURE_COMPARE_MODE,
                      sampler_desc_.compare_enabled ? GL_COMPARE_REF_TO_TEXTURE : GL_NONE);
  glTextureParameteri(renderer_id_, GL_TEXTURE_COMPARE_FUNC,
                      static_cast<GLint>(render_state::opengl_compare_op(sampler_desc_.compare_op)));
}

OpenglTextureCube::OpenglTextureCube(TextureDesc desc, SamplerDesc sampler) : desc_(desc), sampler_desc_(sampler) {
  KEN_CORE_ASSERT(desc_.kind == TextureKind::Cube);
  KEN_CORE_ASSERT(desc_.width > 0);
  KEN_CORE_ASSERT(desc_.height == desc_.width);
  KEN_CORE_ASSERT(desc_.depth == 1);
  KEN_CORE_ASSERT(desc_.has_valid_mip_request());

  allocate_storage();
  apply_sampler_state();
}

OpenglTextureCube::OpenglTextureCube(const std::array<std::filesystem::path, 6>& faces, SamplerDesc sampler,
                                     TextureFormat format, std::uint32_t mip_levels)
    : sampler_desc_(sampler) {
  KEN_CORE_ASSERT(is_byte_color_format(format));

  const int desired_channels = static_cast<int>(texture_format::color_channel_count(format));

  stbi_set_flip_vertically_on_load(0);

  for (std::uint32_t face = 0; face < faces.size(); ++face) {
    int width     = 0;
    int height    = 0;
    int channels  = 0;
    stbi_uc* data = stbi_load(faces[face].string().c_str(), &width, &height, &channels, desired_channels);

    if (data == nullptr) {
      KEN_CORE_CRITICAL("stb_image cube load error ('{0}'): {1}", faces[face].string(), stbi_failure_reason());
      continue;
    }

    KEN_CORE_ASSERT(width > 0);
    KEN_CORE_ASSERT(height > 0);

    if (renderer_id_ == 0) {
      desc_ = TextureDesc::texture_cube(static_cast<std::uint32_t>(width), format, mip_levels);
      KEN_CORE_ASSERT(desc_.has_valid_mip_request());
      allocate_storage();
      apply_sampler_state();
    }

    KEN_CORE_ASSERT(std::cmp_equal(width, desc_.width) && std::cmp_equal(height, desc_.height),
                    "All cube faces must share one square size");

    const std::size_t size_bytes = static_cast<std::size_t>(width) * static_cast<std::size_t>(height) *
                                   static_cast<std::size_t>(texture_format::bytes_per_block(format));

    set_data(std::span<const std::byte>{reinterpret_cast<const std::byte*>(data), size_bytes}, 0, face);

    stbi_image_free(data);
  }

  if (renderer_id_ != 0 && desc_.resolved_mip_levels() > 1) {
    glGenerateTextureMipmap(renderer_id_);
  }
}

OpenglTextureCube::~OpenglTextureCube() { glDeleteTextures(1, &renderer_id_); }

void OpenglTextureCube::set_data(std::span<const std::byte> data, std::uint32_t mip_level, std::uint32_t layer) {
  KEN_PROFILE_FUNCTION();
  KEN_PROFILE_GPU_SCOPE("OpenglTextureCube::set_data");
  KEN_CORE_ASSERT(renderer_id_ != 0);
  KEN_CORE_ASSERT(layer < 6);
  KEN_CORE_ASSERT(mip_level < desc_.resolved_mip_levels());

  const std::uint32_t mip_width  = std::max(1U, desc_.width >> mip_level);
  const std::uint32_t mip_height = std::max(1U, desc_.height >> mip_level);

  [[maybe_unused]] const std::size_t expected_size =
      static_cast<std::size_t>(mip_width) * static_cast<std::size_t>(mip_height) *
      static_cast<std::size_t>(texture_format::bytes_per_block(desc_.format));

  KEN_CORE_ASSERT(data.size_bytes() == expected_size, "Data must match the target mip size");

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  profile::count_upload(data.size_bytes());
  // For a cube map the array layer (z-offset) selects the face.
  glTextureSubImage3D(renderer_id_, static_cast<GLint>(mip_level), 0, 0, static_cast<GLint>(layer),
                      static_cast<GLsizei>(mip_width), static_cast<GLsizei>(mip_height), 1, upload_format_,
                      upload_type_, data.data());
}

void OpenglTextureCube::set_region(std::span<const std::byte> data, const TextureRegion& region,
                                   std::uint32_t row_length, std::uint32_t mip_level, std::uint32_t layer) {
  KEN_PROFILE_FUNCTION();
  KEN_PROFILE_GPU_SCOPE("OpenglTextureCube::set_region");
  KEN_CORE_ASSERT(renderer_id_ != 0);
  KEN_CORE_ASSERT(layer < 6);
  KEN_CORE_ASSERT(mip_level < desc_.resolved_mip_levels());

  const std::uint32_t row = row_length == 0 ? region.width : row_length;

  KEN_CORE_ASSERT(region_inside(region, desc_, mip_level), "Region must lie inside the target mip");
  KEN_CORE_ASSERT(row >= region.width);
  KEN_CORE_ASSERT(data.size_bytes() >= region_span_bytes(region, row, desc_.format), "Data must cover the region");

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, static_cast<GLint>(row));

  profile::count_upload(region_bytes(region, desc_.format));
  // For a cube map the array layer (z-offset) selects the face.
  glTextureSubImage3D(renderer_id_, static_cast<GLint>(mip_level), static_cast<GLint>(region.x),
                      static_cast<GLint>(region.y), static_cast<GLint>(layer), static_cast<GLsizei>(region.width),
                      static_cast<GLsizei>(region.height), 1, upload_format_, upload_type_, data.data());

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
}

void OpenglTextureCube::read_region(std::span<std::byte> data, const TextureRegion& region, std::uint32_t mip_level,
                                    std::uint32_t layer) const {
  KEN_PROFILE_FUNCTION();
  KEN_PROFILE_GPU_SCOPE("OpenglTextureCube::read_region");
  KEN_CORE_ASSERT(renderer_id_ != 0);
  KEN_CORE_ASSERT(layer < 6);
  KEN_CORE_ASSERT(mip_level < desc_.resolved_mip_levels());
  KEN_CORE_ASSERT(region_inside(region, desc_, mip_level), "Region must lie inside the source mip");

  // For a cube map the array layer (z-offset) selects the face.
  read_texture_region(renderer_id_, desc_.format, data, region, mip_level, layer);
}

void OpenglTextureCube::allocate_storage() {
  KEN_CORE_ASSERT(renderer_id_ == 0);

  internal_format_ = texture_format::internal_format(desc_.format);
  upload_format_   = texture_format::pixel_format(desc_.format);
  upload_type_     = texture_format::pixel_type(desc_.format);

  glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &renderer_id_);
  glTextureStorage2D(renderer_id_, static_cast<GLsizei>(desc_.resolved_mip_levels()), internal_format_,
                     static_cast<GLsizei>(desc_.width), static_cast<GLsizei>(desc_.height));

  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void OpenglTextureCube::apply_sampler_state() {
  KEN_CORE_ASSERT(renderer_id_ != 0);

  const auto mip_filter = desc_.resolved_mip_levels() > 1 ? sampler_desc_.mip_filter : SamplerDesc::MipFilter::None;

  glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, opengl_min_filter(sampler_desc_.min_filter, mip_filter));
  glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, opengl_mag_filter(sampler_desc_.mag_filter));

  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, opengl_wrap(sampler_desc_.wrap_u));
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, opengl_wrap(sampler_desc_.wrap_v));
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_R, opengl_wrap(sampler_desc_.wrap_w));
}

OpenglTexture3D::OpenglTexture3D(TextureDesc desc, SamplerDesc sampler) : desc_(desc), sampler_desc_(sampler) {
  KEN_CORE_ASSERT(desc_.kind == TextureKind::Tex3D);
  KEN_CORE_ASSERT(desc_.width > 0);
  KEN_CORE_ASSERT(desc_.height > 0);
  KEN_CORE_ASSERT(desc_.depth > 0);
  KEN_CORE_ASSERT(desc_.layers == 1);
  KEN_CORE_ASSERT(desc_.has_valid_mip_request());

  allocate_storage();
  apply_sampler_state();
}

OpenglTexture3D::~OpenglTexture3D() { glDeleteTextures(1, &renderer_id_); }

void OpenglTexture3D::set_data(std::span<const std::byte> data, std::uint32_t mip_level,
                               [[maybe_unused]] std::uint32_t layer) {
  KEN_PROFILE_FUNCTION();
  KEN_PROFILE_GPU_SCOPE("OpenglTexture3D::set_data");
  KEN_CORE_ASSERT(renderer_id_ != 0);
  KEN_CORE_ASSERT(layer == 0);
  KEN_CORE_ASSERT(mip_level < desc_.resolved_mip_levels());

  const std::uint32_t mip_width  = std::max(1U, desc_.width >> mip_level);
  const std::uint32_t mip_height = std::max(1U, desc_.height >> mip_level);
  const std::uint32_t mip_depth  = std::max(1U, desc_.depth >> mip_level);

  [[maybe_unused]] const std::size_t expected_size =
      static_cast<std::size_t>(mip_width) * static_cast<std::size_t>(mip_height) * static_cast<std::size_t>(mip_depth) *
      static_cast<std::size_t>(texture_format::bytes_per_block(desc_.format));

  KEN_CORE_ASSERT(data.size_bytes() == expected_size, "Data must match the target mip size");

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  profile::count_upload(data.size_bytes());
  glTextureSubImage3D(renderer_id_, static_cast<GLint>(mip_level), 0, 0, 0, static_cast<GLsizei>(mip_width),
                      static_cast<GLsizei>(mip_height), static_cast<GLsizei>(mip_depth), upload_format_, upload_type_,
                      data.data());

  if (mip_level == 0 && desc_.resolved_mip_levels() > 1) {
    glGenerateTextureMipmap(renderer_id_);
  }
}

void OpenglTexture3D::set_region(std::span<const std::byte> data, const TextureRegion& region, std::uint32_t row_length,
                                 std::uint32_t mip_level, [[maybe_unused]] std::uint32_t layer) {
  KEN_PROFILE_FUNCTION();
  KEN_PROFILE_GPU_SCOPE("OpenglTexture3D::set_region");
  KEN_CORE_ASSERT(renderer_id_ != 0);
  KEN_CORE_ASSERT(layer == 0);
  KEN_CORE_ASSERT(mip_level < desc_.resolved_mip_levels());

  const std::uint32_t row = row_length == 0 ? region.width : row_length;

  KEN_CORE_ASSERT(region_inside(region, desc_, mip_level), "Region must lie inside the target mip");
  KEN_CORE_ASSERT(row >= region.width);
  KEN_CORE_ASSERT(data.size_bytes() >= region_span_bytes(region, row, desc_.format), "Data must cover the region");

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, static_cast<GLint>(row));

  profile::count_upload(region_bytes(region, desc_.format));
  glTextureSubImage3D(renderer_id_, static_cast<GLint>(mip_level), static_cast<GLint>(region.x),
                      static_cast<GLint>(region.y), static_cast<GLint>(region.z), static_cast<GLsizei>(region.width),
                      static_cast<GLsizei>(region.height), static_cast<GLsizei>(region.depth), upload_format_,
                      upload_type_, data.data());

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

  if (mip_level == 0 && desc_.resolved_mip_levels() > 1) {
    glGenerateTextureMipmap(renderer_id_);
  }
}

void OpenglTexture3D::read_region(std::span<std::byte> data, const TextureRegion& region, std::uint32_t mip_level,
                                  [[maybe_unused]] std::uint32_t layer) const {
  KEN_PROFILE_FUNCTION();
  KEN_PROFILE_GPU_SCOPE("OpenglTexture3D::read_region");
  KEN_CORE_ASSERT(renderer_id_ != 0);
  KEN_CORE_ASSERT(layer == 0);
  KEN_CORE_ASSERT(mip_level < desc_.resolved_mip_levels());
  KEN_CORE_ASSERT(region_inside(region, desc_, mip_level), "Region must lie inside the source mip");

  read_texture_region(renderer_id_, desc_.format, data, region, mip_level, region.z);
}

void OpenglTexture3D::allocate_storage() {
  KEN_CORE_ASSERT(renderer_id_ == 0);

  internal_format_ = texture_format::internal_format(desc_.format);
  upload_format_   = texture_format::pixel_format(desc_.format);
  upload_type_     = texture_format::pixel_type(desc_.format);

  glCreateTextures(GL_TEXTURE_3D, 1, &renderer_id_);
  glTextureStorage3D(renderer_id_, static_cast<GLsizei>(desc_.resolved_mip_levels()), internal_format_,
                     static_cast<GLsizei>(desc_.width), static_cast<GLsizei>(desc_.height),
                     static_cast<GLsizei>(desc_.depth));
}

void OpenglTexture3D::apply_sampler_state() {
  KEN_CORE_ASSERT(renderer_id_ != 0);

  const auto mip_filter = desc_.resolved_mip_levels() > 1 ? sampler_desc_.mip_filter : SamplerDesc::MipFilter::None;

  glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, opengl_min_filter(sampler_desc_.min_filter, mip_filter));
  glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, opengl_mag_filter(sampler_desc_.mag_filter));

  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, opengl_wrap(sampler_desc_.wrap_u));
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, opengl_wrap(sampler_desc_.wrap_v));
  glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_R, opengl_wrap(sampler_desc_.wrap_w));
}

}  // namespace kEn
