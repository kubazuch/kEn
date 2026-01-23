#include "light.hpp"

#include <imgui/imgui.h>

#include <kEn/renderer/shader.hpp>

namespace kEn {

inline void Attenuation::load(const std::string& name, Shader& shader) const {
  shader.set_uniform(name + ".constant", constant);
  shader.set_uniform(name + ".linear", linear);
  shader.set_uniform(name + ".quadratic", quadratic);
}

void DirectionalLight::imgui() {
  if (ImGui::ColorEdit3("Color##light", mEn::value_ptr(color))) {
  }
  mEn::Vec3 front = transform().front();
  ImGui::InputFloat3("Dir##light", mEn::value_ptr(front));
}

void DirectionalLight::load(const std::string& name, Shader& shader) const {
  shader.set_uniform(name + ".color", color);
  shader.set_uniform(name + ".dir", transform().front());
}

std::shared_ptr<GameComponent> DirectionalLight::clone() const { return std::make_shared<DirectionalLight>(); }

void PointLight::imgui() {
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
  shader.set_uniform(name + ".color", color);

  atten.load(name + ".atten", shader);

  shader.set_uniform(name + ".pos", transform().pos());
}

std::shared_ptr<GameComponent> PointLight::clone() const {
  auto ptr   = std::make_shared<PointLight>();
  ptr->atten = atten;
  return ptr;
}

SpotLight::SpotLight() {
  static int light_texture_id = 1;

  light_id_ = light_texture_id++;

  kEn::FramebufferSpec spec;
  spec.width       = 512;
  spec.height      = 512;
  spec.attachments = {kEn::FramebufferTextureFormat::RED32, kEn::FramebufferTextureFormat::Depth};

  sss_framebuffer_ = kEn::Framebuffer::create(spec);
  proj_matrix_     = mEn::perspective(2 * outer_cutoff_angle, 1.F, 0.01F, 100.F);
}

void SpotLight::set_angles(float inner, float outer) {
  inner_cutoff_angle = inner;
  outer_cutoff_angle = outer;
  proj_matrix_       = mEn::perspective(2 * outer_cutoff_angle, 1.F, 0.01F, 100.F);
}

void SpotLight::imgui() {
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
    proj_matrix_ = mEn::perspective(2 * outer_cutoff_angle, 1.F, 0.01F, 100.F);
  }

  if (ImGui::SliderFloat("Inner angle", &inner_cutoff_angle, 0, mEn::pi<float>() / 2)) {
  }

  if (ImGui::TreeNode("Texture")) {
    ImGui::Image(sss_framebuffer_->get_color_attachment_renderer_id(0), ImVec2(512, 512), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::TreePop();
  }
}

void SpotLight::load(const std::string& name, Shader& shader) const {
  shader.set_uniform(name + ".color", color);

  atten.load(name + ".atten", shader);

  shader.set_uniform(name + ".pos", transform().pos());
  shader.set_uniform(name + ".dir", mEn::normalize(transform().front()));
  shader.set_uniform(name + ".cutoff", mEn::cos(inner_cutoff_angle));
  shader.set_uniform(name + ".outerCutoff", mEn::cos(outer_cutoff_angle));
  shader.set_uniform(name + ".sss_texture", light_id_);
  shader.set_uniform(name + ".view_matrix", transform().world_to_local_matrix());
  shader.set_uniform(name + ".proj_matrix", proj_matrix_);
}

std::shared_ptr<GameComponent> SpotLight::clone() const {
  auto ptr                = std::make_shared<SpotLight>();
  ptr->atten              = atten;
  ptr->inner_cutoff_angle = inner_cutoff_angle;
  ptr->outer_cutoff_angle = outer_cutoff_angle;
  return ptr;
}

void SpotLight::bind_sss_texture() const { sss_framebuffer_->bind_attachment_as_texture(0, light_id_); }

}  // namespace kEn
