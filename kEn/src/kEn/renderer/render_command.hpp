#pragma once
#include <kEn/renderer/renderer_api.hpp>

namespace kEn {

class RenderCommand {
 public:
  static void init() { renderer_api_->init(); }
  static void set_viewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) { renderer_api_->set_viewport(x, y, w, h); }
  static void set_clear_color(const glm::vec4& color) { renderer_api_->set_clear_color(color); }
  static void clear() { renderer_api_->clear(); }

  static void depth_testing(bool enabled = true) { renderer_api_->depth_testing(enabled); }

  static void draw_indexed(const VertexArray& vertex_array, uint32_t index_count = 0) {
    renderer_api_->draw_indexed(vertex_array, index_count);
  }
  static void draw_patches(const VertexArray& vertex_array, uint32_t vertex_count = 0) {
    renderer_api_->draw_patches(vertex_array, vertex_count);
  }
  static void set_tessellation_patch_vertices(uint32_t count = 4) {
    renderer_api_->set_tessellation_patch_vertices(count);
  }
  static void set_wireframe(bool wireframe) { renderer_api_->set_wireframe(wireframe); }

 private:
  static std::unique_ptr<RendererApi> renderer_api_;
};

}  // namespace kEn
