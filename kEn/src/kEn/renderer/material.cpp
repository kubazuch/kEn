#include "material.hpp"

#include <cstddef>
#include <cstdint>
#include <format>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <utility>

#include <kEn/renderer/render_context.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>

namespace kEn {

void Material::set_texture(TextureType type, std::shared_ptr<kEn::Texture> texture, std::size_t id) {
  auto& texs = textures_[to_index(type)];
  if (id < texs.size()) {
    texs[id] = std::move(texture);
    return;
  }

  if (id == texs.size()) {
    texs.push_back(std::move(texture));
    return;
  }

  throw std::runtime_error("Textures should be added in order!");
}

std::shared_ptr<Texture> Material::texture(TextureType type, std::size_t id) const {
  return textures_[to_index(type)].at(id);
}

std::size_t Material::texture_count(TextureType type) const noexcept { return textures_[to_index(type)].size(); }

void Material::apply(Shader& shader, RenderContext& context, std::string_view name) const {
  shader.set_uniform(std::format("{}.ka", name), ambient_factor);
  shader.set_uniform(std::format("{}.kd", name), diffuse_factor);
  shader.set_uniform(std::format("{}.ks", name), specular_factor);
  shader.set_uniform(std::format("{}.m", name), shininess_factor);
  shader.set_uniform(std::format("{}.emissive", name), emissive);
  shader.set_uniform(std::format("{}.surface_color", name), surface_color);

  for (auto i = std::uint8_t{0}; i < std::to_underlying(TextureType::Count); ++i) {
    const int count = static_cast<int>(textures_[i].size());
    shader.set_uniform(std::format("{}.{}_count", name, texture_type::name_of(static_cast<TextureType>(i))), count);
  }

  std::uint32_t texture_id = 0;
  for (auto i = std::uint8_t{0}; i < std::to_underlying(TextureType::Count); ++i) {
    const auto& texs = textures_[i];
    for (std::size_t j = 0; j < texs.size(); ++j) {
      shader.set_uniform(std::format("{}.{}[{}]", name, texture_type::name_of(static_cast<TextureType>(i)), j),
                         static_cast<int>(texture_id));
      context.bind_texture(texture_id, ShaderStage::Fragment, *texs[j]);
      ++texture_id;
    }
  }
}

}  // namespace kEn
