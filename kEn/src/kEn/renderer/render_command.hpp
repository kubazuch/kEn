#pragma once

#include <mEn/vec4.hpp>

#include <kEn/renderer/renderer_api.hpp>

namespace kEn {

class RenderCommand {
  using RenderMode = RendererApi::RenderMode;

 public:
  static void init() { renderer_api_->init(); }
  static void set_viewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) { renderer_api_->set_viewport(x, y, w, h); }
  static void set_clear_color(const mEn::Vec4& color) { renderer_api_->set_clear_color(color); }
  static void clear() { renderer_api_->clear(); }

  static void depth_testing(bool enabled = true) { renderer_api_->depth_testing(enabled); }

  static void draw_indexed(const VertexArray& vertex_array, size_t index_count,
                           RenderMode mode = RenderMode::Triangles) {
    renderer_api_->draw_indexed(vertex_array, index_count, mode);
  }

  static void draw(const VertexArray& vertex_array, uint32_t vertex_count, RenderMode mode = RenderMode::Triangles) {
    renderer_api_->draw(vertex_array, vertex_count, mode);
  }

  static void draw_indexed_instanced(const VertexArray& vertex_array, size_t index_count, size_t instance_count,
                                     RenderMode mode = RenderMode::Triangles) {
    renderer_api_->draw_indexed_instanced(vertex_array, index_count, instance_count, mode);
  }

  static void draw_instanced(const VertexArray& vertex_array, uint32_t vertex_count, size_t instance_count,
                             RenderMode mode = RenderMode::Triangles) {
    renderer_api_->draw_instanced(vertex_array, vertex_count, instance_count, mode);
  }

  static void set_tessellation_patch_vertices(uint32_t count = 4) {
    renderer_api_->set_tessellation_patch_vertices(count);
  }

  static void set_wireframe(bool wireframe) { renderer_api_->set_wireframe(wireframe); }

  static int max_tesselation_level() { return renderer_api_->max_tesselation_level(); }

 private:
  static std::unique_ptr<RendererApi> renderer_api_;
};

}  // namespace kEn
