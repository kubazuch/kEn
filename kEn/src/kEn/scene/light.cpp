#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/scene/light.hpp>
#include <kenpch.hpp>

namespace kEn {

inline void attenuation::load(const std::string& name, shader& shader) const {
  shader.set_float(name + ".constant", constant);
  shader.set_float(name + ".linear", linear);
  shader.set_float(name + ".quadratic", quadratic);
}

void directional_light::imgui(bool subsection) {
  if (subsection) {
    if (ImGui::TreeNode("Directional light")) {
      imgui(false);
      ImGui::TreePop();
    }

    return;
  }

  if (ImGui::ColorEdit3("Color##light", glm::value_ptr(color))) {
  }
  glm::vec3 front = transform().front();
  ImGui::InputFloat3("Dir##light", glm::value_ptr(front));
}

void directional_light::load(const std::string& name, shader& shader) const {
  shader.set_float3(name + ".color", color);
  shader.set_float3(name + ".dir", transform().front());
}

std::shared_ptr<game_component> directional_light::clone() const { return std::make_shared<directional_light>(); }

void point_light::imgui(bool subsection) {
  if (subsection) {
    if (ImGui::TreeNode("Point light")) {
      imgui(false);
      ImGui::TreePop();
    }

    return;
  }

  if (ImGui::ColorEdit3("Color##light", glm::value_ptr(color))) {
  }
  if (ImGui::DragFloat3("Pos##light", glm::value_ptr(transform().local_pos()), 0.01f)) {
    transform().set_local_pos(transform().local_pos());
  }

  if (ImGui::SliderFloat("Constant", &atten.constant, 0, 2)) {
  }

  if (ImGui::SliderFloat("Linear", &atten.linear, 0, 2)) {
  }

  if (ImGui::SliderFloat("Quadratic", &atten.quadratic, 0, 2)) {
  }
}

void point_light::load(const std::string& name, shader& shader) const {
  shader.set_float3(name + ".color", color);

  atten.load(name + ".atten", shader);

  shader.set_float3(name + ".pos", transform().pos());
}

std::shared_ptr<game_component> point_light::clone() const {
  auto ptr   = std::make_shared<point_light>();
  ptr->atten = atten;
  return ptr;
}

void spot_light::imgui(bool subsection) {
  if (subsection) {
    if (ImGui::TreeNode("Point light")) {
      imgui(false);
      ImGui::TreePop();
    }

    return;
  }

  if (ImGui::ColorEdit3("Color##light", glm::value_ptr(color))) {
  }
  if (ImGui::DragFloat3("Pos##light", glm::value_ptr(transform().local_pos()), 0.01f)) {
    transform().set_local_pos(transform().local_pos());
  }

  if (ImGui::SliderFloat("Constant", &atten.constant, 0, 2)) {
  }

  if (ImGui::SliderFloat("Linear", &atten.linear, 0, 2)) {
  }

  if (ImGui::SliderFloat("Quadratic", &atten.quadratic, 0, 2)) {
  }

  if (ImGui::SliderFloat("Outer angle", &outer_cutoff_angle, 0, glm::pi<float>() / 2)) {
  }

  if (ImGui::SliderFloat("Inner angle", &inner_cutoff_angle, 0, glm::pi<float>() / 2)) {
  }
}

void spot_light::load(const std::string& name, shader& shader) const {
  shader.set_float3(name + ".color", color);

  atten.load(name + ".atten", shader);

  shader.set_float3(name + ".pos", transform().pos());
  shader.set_float3(name + ".dir", glm::normalize(transform().front()));
  shader.set_float(name + ".cutoff", glm::cos(inner_cutoff_angle));
  shader.set_float(name + ".outerCutoff", glm::cos(outer_cutoff_angle));
}

std::shared_ptr<game_component> spot_light::clone() const {
  auto ptr                = std::make_shared<spot_light>();
  ptr->atten              = atten;
  ptr->inner_cutoff_angle = inner_cutoff_angle;
  ptr->outer_cutoff_angle = outer_cutoff_angle;
  return ptr;
}

}  // namespace kEn
