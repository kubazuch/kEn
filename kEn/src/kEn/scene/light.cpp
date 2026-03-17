#include "light.hpp"

#include <imgui/imgui.h>

#include <format>
#include <memory>
#include <string_view>

#include <mEn/constants.hpp>
#include <mEn/features/type_ptr.hpp>
#include <mEn/functions/trigonometric.hpp>
#include <mEn/fwd.hpp>

#include <kEn/renderer/shader.hpp>
#include <kEn/scene/component.hpp>

namespace kEn {

void Attenuation::load(std::string_view name, Shader& shader) const {
  shader.set_uniform(std::format("{}.constant", name), constant);
  shader.set_uniform(std::format("{}.linear", name), linear);
  shader.set_uniform(std::format("{}.quadratic", name), quadratic);
}

void DirectionalLight::imgui() {
  ImGui::PushID(this);
  ImGui::ColorEdit3("Color", mEn::value_ptr(color));
  mEn::Vec3 front = transform().front();
  // TODO(kuzu): InputFloat3 receives a pointer to a local copy of front(); drive the rotation
  //   through edit_local_trs() or calculate rotation quaternion and multiply it.
  ImGui::InputFloat3("Dir", mEn::value_ptr(front));
  ImGui::PopID();
}

void DirectionalLight::load(std::string_view name, Shader& shader) const {
  shader.set_uniform(std::format("{}.color", name), color);
  shader.set_uniform(std::format("{}.dir", name), transform().front());
}

std::shared_ptr<GameComponent> DirectionalLight::clone() const {
  auto ptr   = std::make_shared<DirectionalLight>();
  ptr->color = color;
  return ptr;
}

void PointLight::imgui() {
  ImGui::PushID(this);
  ImGui::ColorEdit3("Color", mEn::value_ptr(color));
  {
    auto edit = transform().edit_local_trs();
    edit.set_active(ImGui::DragFloat3("Pos", edit.pos_ptr(), 0.01F));
  }

  ImGui::SliderFloat("Constant", &atten.constant, 0, 2);
  ImGui::SliderFloat("Linear", &atten.linear, 0, 2);
  ImGui::SliderFloat("Quadratic", &atten.quadratic, 0, 2);
  ImGui::PopID();
}

void PointLight::load(std::string_view name, Shader& shader) const {
  shader.set_uniform(std::format("{}.color", name), color);
  atten.load(std::format("{}.atten", name), shader);
  shader.set_uniform(std::format("{}.pos", name), transform().pos());
}

std::shared_ptr<GameComponent> PointLight::clone() const {
  auto ptr   = std::make_shared<PointLight>();
  ptr->color = color;
  ptr->atten = atten;
  return ptr;
}

void SpotLight::imgui() {
  ImGui::PushID(this);
  ImGui::ColorEdit3("Color", mEn::value_ptr(color));
  {
    auto edit = transform().edit_local_trs();
    edit.set_active(ImGui::DragFloat3("Pos", edit.pos_ptr(), 0.01F));
  }
  mEn::Vec3 front = transform().front();
  // TODO(kuzu): InputFloat3 receives a pointer to a local copy of front(); drive the rotation
  //   through edit_local_trs() or calculate rotation quaternion and multiply it.
  ImGui::InputFloat3("Dir", mEn::value_ptr(front));

  ImGui::SliderFloat("Constant", &atten.constant, 0, 2);
  ImGui::SliderFloat("Linear", &atten.linear, 0, 2);
  ImGui::SliderFloat("Quadratic", &atten.quadratic, 0, 2);
  ImGui::SliderFloat("Outer angle", &outer_cutoff_angle, 0, mEn::kHalfPi<float>);
  ImGui::SliderFloat("Inner angle", &inner_cutoff_angle, 0, outer_cutoff_angle);
  ImGui::PopID();
}

void SpotLight::load(std::string_view name, Shader& shader) const {
  shader.set_uniform(std::format("{}.color", name), color);
  atten.load(std::format("{}.atten", name), shader);
  shader.set_uniform(std::format("{}.pos", name), transform().pos());
  shader.set_uniform(std::format("{}.dir", name), transform().front());
  shader.set_uniform(std::format("{}.cutoff", name), mEn::cos(inner_cutoff_angle));
  shader.set_uniform(std::format("{}.outerCutoff", name), mEn::cos(outer_cutoff_angle));
}

std::shared_ptr<GameComponent> SpotLight::clone() const {
  auto ptr                = std::make_shared<SpotLight>();
  ptr->color              = color;
  ptr->atten              = atten;
  ptr->inner_cutoff_angle = inner_cutoff_angle;
  ptr->outer_cutoff_angle = outer_cutoff_angle;
  return ptr;
}

}  // namespace kEn