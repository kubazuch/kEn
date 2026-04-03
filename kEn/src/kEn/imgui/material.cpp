#include "material.hpp"

#include <imgui/imgui.h>

#include <cstddef>
#include <cstdint>
#include <utility>

#include <kEn/renderer/material.hpp>

namespace kEn::ui {

void Material(kEn::Material& mat) {
  if (ImGui::TreeNode("Phong properties")) {
    ImGui::SliderFloat("ambient", &mat.ambient_factor, 0, 1);
    ImGui::SliderFloat("diffuse", &mat.diffuse_factor, 0, 1);
    ImGui::SliderFloat("specular", &mat.specular_factor, 0, 1);
    ImGui::SliderFloat("shininess", &mat.shininess_factor, 1, 100);

    ImGui::TreePop();
  }

  if (ImGui::TreeNode("Textures")) {
    for (auto i = std::uint8_t{0}; i < std::to_underlying(TextureType::Count); ++i) {
      const auto type         = static_cast<TextureType>(i);
      const std::size_t count = mat.texture_count(type);
      if (count == 0) {
        continue;
      }

      if (ImGui::TreeNode(texture_type::name_of(type).data())) {  // NOLINT(bugprone-suspicious-stringview-data-usage)
        for (std::size_t j = 0; j < count; ++j) {
          const auto& tex = mat.texture(type, j);
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

}  // namespace kEn::ui
