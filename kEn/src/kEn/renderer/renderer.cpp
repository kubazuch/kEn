#include "renderer.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include <kEn/core/application.hpp>
#include <kEn/renderer/command.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/vertex_input.hpp>
#include <kEn/scene/camera/camera.hpp>

namespace kEn {

std::unique_ptr<Renderer::SceneData> Renderer::scene_data_ = std::make_unique<SceneData>();

void Renderer::begin_scene(const std::shared_ptr<Camera>& camera) {
  scene_data_->V_matrix   = camera->view_matrix();
  scene_data_->P_matrix   = camera->projection_matrix();
  scene_data_->VP_matrix  = camera->view_projection_matrix();
  scene_data_->camera_pos = camera->transform().world_pos();
}

void Renderer::begin_scene(const mEn::Vec3& camera_pos, const mEn::Mat4& view, const mEn::Mat4& projection) {
  scene_data_->V_matrix   = view;
  scene_data_->P_matrix   = projection;
  scene_data_->VP_matrix  = projection * view;
  scene_data_->camera_pos = camera_pos;
}

void Renderer::end_scene() {}

void Renderer::prepare(Shader& shader) {
  shader.set_uniform("u_PointCount", static_cast<int>(scene_data_->point_lights.size()));
  int i = 0;
  for (const auto& light : scene_data_->point_lights) {
    light->load("u_PointLights[" + std::to_string(i++) + "]", shader);
  }

  shader.set_uniform("u_DirectionalCount", static_cast<int>(scene_data_->directional_lights.size()));
  i = 0;
  for (const auto& light : scene_data_->directional_lights) {
    light->load("u_DirectionalLights[" + std::to_string(i++) + "]", shader);
  }

  shader.set_uniform("u_SpotCount", static_cast<int>(scene_data_->spot_lights.size()));
  i = 0;
  for (const auto& light : scene_data_->spot_lights) {
    light->load("u_SpotLights[" + std::to_string(i++) + "]", shader);
  }
}

void Renderer::submit(Shader& shader, const VertexInput& vertex_input, RenderMode mode) {
  shader.set_uniform("u_VP", scene_data_->VP_matrix);
  shader.set_uniform("u_CameraPos", scene_data_->camera_pos);
  shader.set_uniform("u_Ambient", scene_data_->ambient);

  shader.bind();
  device().command().draw_indexed(vertex_input, vertex_input.element_count(), mode);
}

void Renderer::submit(Shader& shader, const VertexInput& vertex_input, const Transform& transform, RenderMode mode) {
  shader.set_uniform("u_V", scene_data_->V_matrix);
  shader.set_uniform("u_P", scene_data_->P_matrix);
  shader.set_uniform("u_VP", scene_data_->VP_matrix);
  shader.set_uniform("u_CameraPos", scene_data_->camera_pos);
  shader.set_uniform("u_M", transform.local_to_world_matrix());
  shader.set_uniform("u_Ambient", scene_data_->ambient);

  shader.bind();
  if (vertex_input.index_buffer()) {
    device().command().draw_indexed(vertex_input, vertex_input.element_count(), mode);
  } else {
    device().command().draw(vertex_input, vertex_input.element_count(), mode);
  }
}

void Renderer::submit_instanced(Shader& shader, const VertexInput& vertex_input, size_t instance_count,
                                RenderMode mode) {
  shader.set_uniform("u_V", scene_data_->V_matrix);
  shader.set_uniform("u_P", scene_data_->P_matrix);
  shader.set_uniform("u_Ambient", scene_data_->ambient);

  shader.bind();
  if (vertex_input.index_buffer()) {
    device().command().draw_indexed_instanced(vertex_input, vertex_input.element_count(), instance_count, mode);
  } else {
    device().command().draw_instanced(vertex_input, vertex_input.element_count(), instance_count, mode);
  }
}

void Renderer::submit_tessellated(Shader& shader, const VertexInput& vertex_input, const uint32_t& count,
                                  const Transform& transform) {
  shader.set_uniform("u_VP", scene_data_->VP_matrix);
  shader.set_uniform("u_CameraPos", scene_data_->camera_pos);
  shader.set_uniform("u_M", transform.local_to_world_matrix());
  shader.set_uniform("u_Ambient", scene_data_->ambient);

  shader.bind();
  device().command().draw(vertex_input, count, render_mode::Patches);
}

}  // namespace kEn
