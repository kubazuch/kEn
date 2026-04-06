#include "model.hpp"

#include <imgui/imgui.h>

#include <cstddef>
#include <span>

#include <kEn/imgui/editors/material.hpp>
#include <kEn/scene/assets/mesh.hpp>
#include <kEn/scene/assets/model.hpp>

namespace kEn::ui {

void Mesh(kEn::Mesh& mesh) {
  if (ImGui::CollapsingHeader(mesh.name.c_str())) {
    ui::Material(mesh.material);
  }
}

void Model(kEn::Model& model) {
  const auto render_list = [](std::span<kEn::Mesh> meshes) {
    for (std::size_t i = 0; i < meshes.size(); ++i) {
      ImGui::PushID(static_cast<int>(i));
      ui::Mesh(meshes[i]);
      ImGui::PopID();
    }
  };

  if (ImGui::TreeNode("Meshes")) {
    ImGui::BeginChild("Meshes", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY);
    if (!model.opaque_meshes().empty() && ImGui::TreeNode("Opaque")) {
      render_list(model.opaque_meshes());
      ImGui::TreePop();
    }
    if (!model.transparent_meshes().empty() && ImGui::TreeNode("Transparent")) {
      render_list(model.transparent_meshes());
      ImGui::TreePop();
    }
    ImGui::EndChild();
    ImGui::TreePop();
  }
}

}  // namespace kEn::ui
