#include "light.hpp"

#include <imgui/imgui.h>

#include <mEn/features/type_ptr.hpp>
#include <mEn/functions/quaternion_trigonometric.hpp>
#include <mEn/fwd.hpp>

#include <kEn/scene/light.hpp>

namespace kEn::ui {

void DirectionalLight(kEn::DirectionalLight& light) {
  ImGui::PushID(&light);
  ImGui::ColorEdit3("Color", mEn::value_ptr(light.color));
  {
    auto edit       = light.transform().edit_local_trs();
    mEn::Vec3 euler = mEn::eulerAngles(edit.rot);
    bool changed    = false;
    changed |= ImGui::SliderAngle("Pitch", &euler.x, -90.0F, 90.0F);
    changed |= ImGui::SliderAngle("Yaw", &euler.y, -180.0F, 180.0F);
    if (changed) {
      edit.rot = mEn::angleAxis(euler.y, mEn::Vec3(0, 1, 0)) * mEn::angleAxis(euler.x, mEn::Vec3(1, 0, 0));
    }
    edit.set_active(changed);
  }
  ImGui::PopID();
}

void PointLight(kEn::PointLight& light) {
  ImGui::PushID(&light);
  ImGui::ColorEdit3("Color", mEn::value_ptr(light.color));
  {
    auto edit = light.transform().edit_local_trs();
    edit.set_active(ImGui::DragFloat3("Pos", edit.pos_ptr(), 0.01F));
  }

  float r = light.atten.radius();
  if (ImGui::SliderFloat("Radius", &r, 0.1F, 100.0F)) {
    light.atten = Attenuation::from_radius(r);
  }
  float h = light.atten.half_life();
  if (ImGui::SliderFloat("Half-life", &h, 0.01F, 10.0F)) {
    light.atten = Attenuation::from_half_life(h);
  }
  ImGui::LabelText("Linear", "%.4f", static_cast<double>(light.atten.linear()));
  ImGui::LabelText("Quadratic", "%.4f", static_cast<double>(light.atten.quadratic()));
  ImGui::PopID();
}

void SpotLight(kEn::SpotLight& light) {
  ImGui::PushID(&light);
  ImGui::ColorEdit3("Color", mEn::value_ptr(light.color));
  {
    auto edit = light.transform().edit_local_trs();
    edit.set_active(ImGui::DragFloat3("Pos", edit.pos_ptr(), 0.01F));
  }
  {
    auto edit       = light.transform().edit_local_trs();
    mEn::Vec3 euler = mEn::eulerAngles(edit.rot);
    bool changed    = false;
    changed |= ImGui::SliderAngle("Pitch", &euler.x, -90.0F, 90.0F);
    changed |= ImGui::SliderAngle("Yaw", &euler.y, -180.0F, 180.0F);
    if (changed) {
      edit.rot = mEn::angleAxis(euler.y, mEn::Vec3(0, 1, 0)) * mEn::angleAxis(euler.x, mEn::Vec3(1, 0, 0));
    }
    edit.set_active(changed);
  }

  float r = light.atten.radius();
  if (ImGui::SliderFloat("Radius", &r, 0.1F, 100.0F)) {
    light.atten = Attenuation::from_radius(r);
  }
  float h = light.atten.half_life();
  if (ImGui::SliderFloat("Half-life", &h, 0.01F, 10.0F)) {
    light.atten = Attenuation::from_half_life(h);
  }
  ImGui::LabelText("Linear", "%.4f", static_cast<double>(light.atten.linear()));
  ImGui::LabelText("Quadratic", "%.4f", static_cast<double>(light.atten.quadratic()));

  float outer = light.outer_cutoff_angle();
  if (ImGui::SliderFloat("Outer angle", &outer, 0.0F, 90.0F)) {
    light.set_cutoff_angles(light.inner_cutoff_angle(), outer);
  }
  float inner = light.inner_cutoff_angle();
  if (ImGui::SliderFloat("Inner angle", &inner, 0.0F, light.outer_cutoff_angle())) {
    light.set_cutoff_angles(inner, light.outer_cutoff_angle());
  }
  ImGui::PopID();
}

}  // namespace kEn::ui
