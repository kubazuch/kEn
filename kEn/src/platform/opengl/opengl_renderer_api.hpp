#pragma once
#include <kEn/renderer/renderer_api.hpp>
#include <kEn/renderer/vertex_array.hpp>

namespace kEn {

class OpenglRendererApi : public RendererApi {
 public:
  ~OpenglRendererApi() override = default;

  void init() override;
  void set_viewport(size_t x, size_t y, size_t w, size_t h) override;

  void set_clear_color(const mEn::Vec4& color) override;
  void clear() override;
  void depth_testing(bool enabled) override;

  void draw_indexed(const VertexArray& vertex_array, size_t index_count, RendererApi::RenderMode mode) override;
  void draw(const VertexArray& vertex_array, size_t vertex_count, RendererApi::RenderMode mode) override;
  void draw_indexed_instanced(const VertexArray& vertex_array, size_t index_count, size_t instance_count,
                              RendererApi::RenderMode mode) override;
  void draw_instanced(const VertexArray& vertex_array, size_t vertex_count, size_t instance_count,
                      RendererApi::RenderMode mode) override;

  void set_tessellation_patch_vertices(size_t count) override;
  void set_wireframe(bool wireframe) override;
};

}  // namespace kEn
