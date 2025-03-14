#include <imgui/imgui.h>

#include <cstdint>
#include <kEn/renderer/material.hpp>
#include <kEn/renderer/shader.hpp>
#include <kenpch.hpp>
#include <ranges>

namespace kEn {

void Material::set_texture(texture_type_t type, const std::shared_ptr<kEn::Texture2D>& texture, size_t id) {
  auto& texs = textures_[type];
  if (id < texs.size()) {
    texs[id] = texture;
    return;
  }

  if (id == texs.size()) {
    texs.push_back(texture);
    return;
  }

  throw std::exception("Textures should be added in order!");
}

const std::shared_ptr<Texture2D>& Material::texture(texture_type_t type, size_t id) { return textures_[type][id]; }

void Material::load(Shader& shader, const std::string& name) const {
  shader.set_float(name + ".ka", ambient_factor);
  shader.set_float(name + ".kd", diffuse_factor);
  shader.set_float(name + ".ks", specular_factor);
  shader.set_float(name + ".m", shininess_factor);
  shader.set_bool(name + ".emissive", emissive);

  for (const auto& [type, textures] : textures_) {
    for (size_t i = 0; i < textures.size(); i++) {
      std::stringstream ss;
      ss << "u_Material." << texture_type::name_of(type) << "[" << i << "]";
      shader.set_int(ss.str(), static_cast<int>(i));
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
    for (texture_type_t i = 0; i < texture_type::Last; ++i) {
      if (!textures_[i].empty()) {
        if (ImGui::TreeNode(texture_type::name_of(i))) {
          for (int j = 0; j < static_cast<int>(textures_[i].size()); ++j) {
            ImGui::PushID(j);
            ImGui::Text("Texture %d:", j);

            textures_[i][j]->imgui();

            ImGui::PopID();
          }
          ImGui::TreePop();
        }
      }
    }

    ImGui::TreePop();
  }
}

}  // namespace kEn
