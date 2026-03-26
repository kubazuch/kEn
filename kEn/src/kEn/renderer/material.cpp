#include "material.hpp"

#include <imgui/imgui.h>

#include <cstddef>
#include <cstdint>
#include <format>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <utility>

#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>

namespace kEn {

void Material::set_texture(TextureType type, std::shared_ptr<kEn::Texture> texture, std::size_t id) {
  auto& texs = textures_[type];
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

const std::shared_ptr<Texture>& Material::texture(TextureType type, std::size_t id) const {
  return textures_.at(type).at(id);
}

void Material::load(Shader& shader, std::string_view name) const {
  shader.set_uniform(std::format("{}.ka", name), ambient_factor);
  shader.set_uniform(std::format("{}.kd", name), diffuse_factor);
  shader.set_uniform(std::format("{}.ks", name), specular_factor);
  shader.set_uniform(std::format("{}.m", name), shininess_factor);
  shader.set_uniform(std::format("{}.emissive", name), emissive);
  shader.set_uniform(std::format("{}.surface_color", name), surface_color);

  for (auto i = std::uint8_t{0}; i < std::to_underlying(TextureType::Count); ++i) {
    const auto type = static_cast<TextureType>(i);
    const auto it   = textures_.find(type);
    const int count = (it != textures_.end()) ? static_cast<int>(it->second.size()) : 0;
    shader.set_uniform(std::format("{}.{}_count", name, texture_type::name_of(type)), count);
  }

  uint32_t texture_id = 0;
  for (const auto& [type, textures] : textures_) {
    for (std::size_t i = 0; i < textures.size(); ++i) {
      shader.set_uniform(std::format("{}.{}[{}]", name, texture_type::name_of(type), i), static_cast<int>(texture_id));
      ++texture_id;
    }
  }
}

void Material::bind() const {
  uint32_t texture_id = 0;
  for (const auto& textures : textures_ | std::views::values) {
    for (const auto& texture : textures) {
      texture->bind(texture_id);
      texture_id++;
    }
  }
}

void Material::imgui() {
  if (ImGui::TreeNode("Phong properties")) {
    ImGui::SliderFloat("ambient", &ambient_factor, 0, 1);
    ImGui::SliderFloat("diffuse", &diffuse_factor, 0, 1);
    ImGui::SliderFloat("specular", &specular_factor, 0, 1);
    ImGui::SliderFloat("shininess", &shininess_factor, 1, 100);

    ImGui::TreePop();
  }

  if (ImGui::TreeNode("Textures")) {
    for (auto i = std::uint8_t{0}; i < std::to_underlying(TextureType::Count); ++i) {
      const auto type = static_cast<TextureType>(i);
      const auto it   = textures_.find(type);
      if (it == textures_.end() || it->second.empty()) {
        continue;
      }

      if (ImGui::TreeNode(texture_type::name_of(type).data())) {  // NOLINT(bugprone-suspicious-stringview-data-usage)
        for (std::size_t j = 0; j < it->second.size(); ++j) {
          const auto& tex = it->second[j];
          const float w   = ImGui::GetContentRegionAvail().x;
          const float h   = w * static_cast<float>(tex->height()) / static_cast<float>(tex->width());
          ImGui::Text("Texture %zu: %ux%u", j, tex->width(), tex->height());
          ImGui::Image(tex->imgui_id(), {w, h}, {0, 1}, {1, 0});
        }
        ImGui::TreePop();
      }
    }

    ImGui::TreePop();
  }
}

}  // namespace kEn
