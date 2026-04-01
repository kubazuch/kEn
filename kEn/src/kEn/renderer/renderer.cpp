#include "renderer.hpp"

#include <cstddef>
#include <memory>
#include <string>

#include <kEn/core/assert.hpp>
#include <kEn/renderer/render_context.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/vertex_input.hpp>
#include <kEn/scene/camera/camera.hpp>

namespace kEn {

std::unique_ptr<Renderer::SceneData> Renderer::scene_data_ = std::make_unique<SceneData>();

void Renderer::begin_scene(const Camera& camera, RenderContext& ctx) {
  scene_data_->ctx        = &ctx;
  scene_data_->v_matrix   = camera.view_matrix();
  scene_data_->p_matrix   = camera.projection_matrix();
  scene_data_->vp_matrix  = camera.view_projection_matrix();
  scene_data_->camera_pos = camera.transform().world_pos();
}

void Renderer::begin_scene(const mEn::Vec3& camera_pos, const mEn::Mat4& view, const mEn::Mat4& projection,
                           RenderContext& ctx) {
  scene_data_->ctx        = &ctx;
  scene_data_->v_matrix   = view;
  scene_data_->p_matrix   = projection;
  scene_data_->vp_matrix  = projection * view;
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

void Renderer::set_render_target(Framebuffer& fb) {
  KEN_CORE_ASSERT(scene_data_->ctx != nullptr, "Renderer::set_render_target called outside begin_scene/end_scene");
  scene_data_->ctx->set_render_target(fb);
}

void Renderer::bind_default_render_target() {
  KEN_CORE_ASSERT(scene_data_->ctx != nullptr,
                  "Renderer::bind_default_render_target called outside begin_scene/end_scene");
  scene_data_->ctx->bind_default_framebuffer();
}

void Renderer::submit(Shader& shader, const VertexInput& vertex_input, RenderMode mode) {
  shader.set_uniform("u_V", scene_data_->v_matrix);
  shader.set_uniform("u_P", scene_data_->p_matrix);
  shader.set_uniform("u_VP", scene_data_->vp_matrix);
  shader.set_uniform("u_CameraPos", scene_data_->camera_pos);
  shader.set_uniform("u_Ambient", scene_data_->ambient);

  KEN_CORE_ASSERT(scene_data_->ctx != nullptr, "Renderer::submit called outside begin_scene/end_scene");
  auto& ctx = *scene_data_->ctx;
  ctx.set_shader(shader);
  ctx.set_vertex_input(vertex_input);
  if (vertex_input.index_buffer()) {
    ctx.draw_indexed(vertex_input.element_count(), 0, 0, mode);
  } else {
    ctx.draw(vertex_input.element_count(), 0, mode);
  }
}

void Renderer::submit(Shader& shader, const VertexInput& vertex_input, const Transform& transform, RenderMode mode) {
  shader.set_uniform("u_V", scene_data_->v_matrix);
  shader.set_uniform("u_P", scene_data_->p_matrix);
  shader.set_uniform("u_VP", scene_data_->vp_matrix);
  shader.set_uniform("u_CameraPos", scene_data_->camera_pos);
  shader.set_uniform("u_M", transform.local_to_world_matrix());
  shader.set_uniform("u_Ambient", scene_data_->ambient);

  KEN_CORE_ASSERT(scene_data_->ctx != nullptr, "Renderer::submit called outside begin_scene/end_scene");
  auto& ctx = *scene_data_->ctx;
  ctx.set_shader(shader);
  ctx.set_vertex_input(vertex_input);
  if (vertex_input.index_buffer()) {
    ctx.draw_indexed(vertex_input.element_count(), 0, 0, mode);
  } else {
    ctx.draw(vertex_input.element_count(), 0, mode);
  }
}

void Renderer::submit_instanced(Shader& shader, const VertexInput& vertex_input, std::size_t instance_count,
                                RenderMode mode) {
  shader.set_uniform("u_V", scene_data_->v_matrix);
  shader.set_uniform("u_P", scene_data_->p_matrix);
  shader.set_uniform("u_VP", scene_data_->vp_matrix);
  shader.set_uniform("u_CameraPos", scene_data_->camera_pos);
  shader.set_uniform("u_Ambient", scene_data_->ambient);

  KEN_CORE_ASSERT(scene_data_->ctx != nullptr, "Renderer::submit_instanced called outside begin_scene/end_scene");
  auto& ctx = *scene_data_->ctx;
  ctx.set_shader(shader);
  ctx.set_vertex_input(vertex_input);
  if (vertex_input.index_buffer()) {
    ctx.draw_indexed_instanced(vertex_input.element_count(), instance_count, 0, 0, 0, mode);
  } else {
    ctx.draw_instanced(vertex_input.element_count(), instance_count, 0, 0, mode);
  }
}

void Renderer::submit_tessellated(Shader& shader, const VertexInput& vertex_input, std::size_t patch_vertex_count,
                                  const Transform& transform) {
  shader.set_uniform("u_V", scene_data_->v_matrix);
  shader.set_uniform("u_P", scene_data_->p_matrix);
  shader.set_uniform("u_VP", scene_data_->vp_matrix);
  shader.set_uniform("u_CameraPos", scene_data_->camera_pos);
  shader.set_uniform("u_M", transform.local_to_world_matrix());
  shader.set_uniform("u_Ambient", scene_data_->ambient);

  KEN_CORE_ASSERT(scene_data_->ctx != nullptr, "Renderer::submit_tessellated called outside begin_scene/end_scene");
  auto& ctx = *scene_data_->ctx;
  ctx.set_tessellation_patch_vertices(patch_vertex_count);
  ctx.set_shader(shader);
  ctx.set_vertex_input(vertex_input);
  if (vertex_input.index_buffer()) {
    ctx.draw_indexed(vertex_input.element_count(), 0, 0, render_mode::Patches);
  } else {
    ctx.draw(vertex_input.element_count(), 0, render_mode::Patches);
  }
}

}  // namespace kEn
