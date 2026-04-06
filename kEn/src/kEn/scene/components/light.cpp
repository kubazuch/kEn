#include "light.hpp"

#include <format>
#include <memory>
#include <string_view>

#include <mEn/functions/trigonometric.hpp>

#include <kEn/core/assert.hpp>
#include <kEn/imgui/editors/light.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/scene/component.hpp>

namespace kEn {

void DirectionalLight::imgui() { ui::DirectionalLight(*this); }

void DirectionalLight::load(std::string_view name, Shader& shader) const {
  shader.set_uniform(std::format("{}.color", name), color);
  shader.set_uniform(std::format("{}.dir", name), transform().world_front());
}

std::unique_ptr<GameComponent> DirectionalLight::clone() const {
  auto ptr   = std::make_unique<DirectionalLight>();
  ptr->color = color;
  return ptr;
}

void PointLight::imgui() { ui::PointLight(*this); }

void PointLight::load(std::string_view name, Shader& shader) const {
  shader.set_uniform(std::format("{}.color", name), color);
  shader.set_uniform(std::format("{}.atten.linear", name), atten.linear());
  shader.set_uniform(std::format("{}.atten.quadratic", name), atten.quadratic());
  shader.set_uniform(std::format("{}.pos", name), transform().world_pos());
}

std::unique_ptr<GameComponent> PointLight::clone() const {
  auto ptr   = std::make_unique<PointLight>();
  ptr->color = color;
  ptr->atten = atten;
  return ptr;
}

void SpotLight::imgui() { ui::SpotLight(*this); }

void SpotLight::set_cutoff_angles(float inner_deg, float outer_deg) {
  KEN_CORE_ASSERT(inner_deg <= outer_deg);
  inner_cutoff_angle_ = inner_deg;
  outer_cutoff_angle_ = outer_deg;
}

void SpotLight::load(std::string_view name, Shader& shader) const {
  shader.set_uniform(std::format("{}.color", name), color);
  shader.set_uniform(std::format("{}.atten.linear", name), atten.linear());
  shader.set_uniform(std::format("{}.atten.quadratic", name), atten.quadratic());
  shader.set_uniform(std::format("{}.pos", name), transform().world_pos());
  shader.set_uniform(std::format("{}.dir", name), transform().world_front());
  shader.set_uniform(std::format("{}.cutoff", name), mEn::cos(mEn::radians(inner_cutoff_angle_)));
  shader.set_uniform(std::format("{}.outerCutoff", name), mEn::cos(mEn::radians(outer_cutoff_angle_)));
}

std::unique_ptr<GameComponent> SpotLight::clone() const {
  auto ptr   = std::make_unique<SpotLight>();
  ptr->color = color;
  ptr->atten = atten;
  ptr->set_cutoff_angles(inner_cutoff_angle_, outer_cutoff_angle_);
  return ptr;
}

}  // namespace kEn