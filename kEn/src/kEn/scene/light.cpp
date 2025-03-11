#include <imgui/imgui.h>

#include <kEn/renderer/shader.hpp>
#include <kEn/scene/light.hpp>
#include <kenpch.hpp>

namespace kEn {

inline void Attenuation::load(const std::string& name, Shader& shader) const {
  shader.set_float(name + ".constant", constant);
  shader.set_float(name + ".linear", linear);
  shader.set_float(name + ".quadratic", quadratic);
}

void DirectionalLight::imgui(bool subsection) {
  if (subsection) {
    if (ImGui::TreeNode("Directional light")) {
      imgui(false);
      ImGui::TreePop();
    }

    return;
  }

  if (ImGui::ColorEdit3("Color##light", mEn::value_ptr(color))) {
  }
  mEn::Vec3 front = transform().front();
  ImGui::InputFloat3("Dir##light", mEn::value_ptr(front));
}

void DirectionalLight::load(const std::string& name, Shader& shader) const {
  shader.set_float3(name + ".color", color);
  shader.set_float3(name + ".dir", transform().front());
}

std::shared_ptr<GameComponent> DirectionalLight::clone() const { return std::make_shared<DirectionalLight>(); }

void PointLight::imgui(bool subsection) {
  if (subsection) {
    if (ImGui::TreeNode("Point light")) {
      imgui(false);
      ImGui::TreePop();
    }

    return;
  }

  if (ImGui::ColorEdit3("Color##light", mEn::value_ptr(color))) {
  }
  if (ImGui::DragFloat3("Pos##light", mEn::value_ptr(transform().local_pos()), 0.01F)) {
    transform().set_local_pos(transform().local_pos());
  }

  if (ImGui::SliderFloat("Constant", &atten.constant, 0, 2)) {
  }

  if (ImGui::SliderFloat("Linear", &atten.linear, 0, 2)) {
  }

  if (ImGui::SliderFloat("Quadratic", &atten.quadratic, 0, 2)) {
  }
}

void PointLight::load(const std::string& name, Shader& shader) const {
  shader.set_float3(name + ".color", color);

  atten.load(name + ".atten", shader);

  shader.set_float3(name + ".pos", transform().pos());
}

std::shared_ptr<GameComponent> PointLight::clone() const {
  auto ptr   = std::make_shared<PointLight>();
  ptr->atten = atten;
  return ptr;
}

void SpotLight::imgui(bool subsection) {
  if (subsection) {
    if (ImGui::TreeNode("Point light")) {
      imgui(false);
      ImGui::TreePop();
    }

    return;
  }

  if (ImGui::ColorEdit3("Color##light", mEn::value_ptr(color))) {
  }
  if (ImGui::DragFloat3("Pos##light", mEn::value_ptr(transform().local_pos()), 0.01F)) {
    transform().set_local_pos(transform().local_pos());
  }

  if (ImGui::SliderFloat("Constant", &atten.constant, 0, 2)) {
  }

  if (ImGui::SliderFloat("Linear", &atten.linear, 0, 2)) {
  }

  if (ImGui::SliderFloat("Quadratic", &atten.quadratic, 0, 2)) {
  }

  if (ImGui::SliderFloat("Outer angle", &outer_cutoff_angle, 0, mEn::pi<float>() / 2)) {
  }

  if (ImGui::SliderFloat("Inner angle", &inner_cutoff_angle, 0, mEn::pi<float>() / 2)) {
  }
}

void SpotLight::load(const std::string& name, Shader& shader) const {
  shader.set_float3(name + ".color", color);

  atten.load(name + ".atten", shader);

  shader.set_float3(name + ".pos", transform().pos());
  shader.set_float3(name + ".dir", mEn::normalize(transform().front()));
  shader.set_float(name + ".cutoff", mEn::cos(inner_cutoff_angle));
  shader.set_float(name + ".outerCutoff", mEn::cos(outer_cutoff_angle));
}

std::shared_ptr<GameComponent> SpotLight::clone() const {
  auto ptr                = std::make_shared<SpotLight>();
  ptr->atten              = atten;
  ptr->inner_cutoff_angle = inner_cutoff_angle;
  ptr->outer_cutoff_angle = outer_cutoff_angle;
  return ptr;
}

}  // namespace kEn
