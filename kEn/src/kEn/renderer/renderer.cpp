#include <kEn/renderer/render_command.hpp>
#include <kEn/renderer/renderer.hpp>
#include <kEn/renderer/vertex_array.hpp>
#include <kEn/scene/core_components.hpp>
#include <kenpch.hpp>

namespace kEn {

std::unique_ptr<Renderer::SceneData> Renderer::scene_data_ = std::make_unique<SceneData>();

void Renderer::begin_scene(const std::shared_ptr<Camera>& camera) {  // NOLINT
  scene_data_->VP_matrix  = camera->view_projection_matrix();
  scene_data_->camera_pos = camera->transform().pos();
}

void Renderer::end_scene() {}

void Renderer::prepare(Shader& shader) {
  shader.set_int("u_PointCount", static_cast<int>(scene_data_->point_lights.size()));
  int i = 0;
  for (const auto& light : scene_data_->point_lights) {
    light->load("u_PointLights[" + std::to_string(i++) + "]", shader);
  }

  shader.set_int("u_DirectionalCount", static_cast<int>(scene_data_->directional_lights.size()));
  i = 0;
  for (const auto& light : scene_data_->directional_lights) {
    light->load("u_DirectionalLights[" + std::to_string(i++) + "]", shader);
  }

  shader.set_int("u_SpotCount", static_cast<int>(scene_data_->spot_lights.size()));
  i = 0;
  for (const auto& light : scene_data_->spot_lights) {
    light->load("u_SpotLights[" + std::to_string(i++) + "]", shader);
  }
}

void Renderer::submit(Shader& shader, const VertexArray& vertex_array, const Transform& transform) {
  shader.set_mat4("u_VP", scene_data_->VP_matrix);
  shader.set_float3("u_CameraPos", scene_data_->camera_pos);
  shader.set_mat4("u_M", transform.local_to_world_matrix());
  shader.set_float3("u_Ambient", scene_data_->ambient);
  shader.set_float("u_Fog", scene_data_->fog);

  shader.bind();
  RenderCommand::draw_indexed(vertex_array);
}

void Renderer::submit_tessellated(Shader& shader, const VertexArray& vertex_array, const uint32_t& count,
                                  const Transform& transform) {
  shader.set_mat4("u_VP", scene_data_->VP_matrix);
  shader.set_float3("u_CameraPos", scene_data_->camera_pos);
  shader.set_mat4("u_M", transform.local_to_world_matrix());
  shader.set_float3("u_Ambient", scene_data_->ambient);
  shader.set_float("u_Fog", scene_data_->fog);

  shader.bind();
  RenderCommand::draw_patches(vertex_array, count);
}

}  // namespace kEn
