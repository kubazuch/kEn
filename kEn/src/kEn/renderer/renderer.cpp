#include "renderer.hpp"

#include <kEn/renderer/render_command.hpp>
#include <kEn/renderer/vertex_array.hpp>
#include <kEn/scene/core_components.hpp>

namespace kEn {

std::unique_ptr<Renderer::SceneData> Renderer::scene_data_ = std::make_unique<SceneData>();

void Renderer::begin_scene(const std::shared_ptr<Camera>& camera) {  // NOLINT
  scene_data_->V_matrix   = camera->view_matrix();
  scene_data_->P_matrix   = camera->projection_matrix();
  scene_data_->VP_matrix  = camera->view_projection_matrix();
  scene_data_->camera_pos = camera->transform().pos();
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

void Renderer::submit(Shader& shader, const VertexArray& vertex_array, RenderMode mode) {
  shader.set_uniform("u_VP", scene_data_->VP_matrix);
  shader.set_uniform("u_CameraPos", scene_data_->camera_pos);
  shader.set_uniform("u_Ambient", scene_data_->ambient);

  shader.bind();
  RenderCommand::draw_indexed(vertex_array, vertex_array.element_count(), mode);
}

void Renderer::submit(Shader& shader, const VertexArray& vertex_array, const Transform& transform, RenderMode mode) {
  shader.set_uniform("u_VP", scene_data_->VP_matrix);
  shader.set_uniform("u_CameraPos", scene_data_->camera_pos);
  shader.set_uniform("u_M", transform.local_to_world_matrix());
  shader.set_uniform("u_Ambient", scene_data_->ambient);

  shader.bind();
  if (vertex_array.index_buffer()) {
    RenderCommand::draw_indexed(vertex_array, vertex_array.element_count(), mode);
  } else {
    RenderCommand::draw(vertex_array, vertex_array.element_count(), mode);
  }
}

void Renderer::submit_instanced(Shader& shader, const VertexArray& vertex_array, size_t instance_count,
                                RenderMode mode) {
  shader.set_uniform("u_V", scene_data_->V_matrix);
  shader.set_uniform("u_P", scene_data_->P_matrix);
  shader.set_uniform("u_Ambient", scene_data_->ambient);

  shader.bind();
  RenderCommand::draw_indexed_instanced(vertex_array, vertex_array.element_count(), instance_count, mode);
}

void Renderer::submit_tessellated(Shader& shader, const VertexArray& vertex_array, const uint32_t& count,
                                  const Transform& transform) {
  shader.set_uniform("u_VP", scene_data_->VP_matrix);
  shader.set_uniform("u_CameraPos", scene_data_->camera_pos);
  shader.set_uniform("u_M", transform.local_to_world_matrix());
  shader.set_uniform("u_Ambient", scene_data_->ambient);

  shader.bind();
  RenderCommand::draw(vertex_array, count, RenderMode::Patches);
}

}  // namespace kEn
