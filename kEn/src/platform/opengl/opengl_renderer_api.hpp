#pragma once
#include <kEn/renderer/renderer_api.hpp>
#include <kEn/renderer/vertex_array.hpp>

namespace kEn {

class OpenglRendererApi : public kEn::RendererApi {
 public:
  ~OpenglRendererApi() override = default;

  void init() override;
  void set_viewport(size_t x, size_t y, size_t w, size_t h) override;

  void set_clear_color(const mEn::Vec4& color) override;
  void clear() override;
  void depth_testing(bool enabled) override;

  void draw_indexed(const VertexArray& vertex_array, size_t index_count) override;
  void draw_lines(const VertexArray& vertex_array, bool strip, size_t index_count) override;
  void draw_patches(const VertexArray& vertex_array, size_t vertex_count) override;
  void set_tessellation_patch_vertices(size_t count) override;
  void set_wireframe(bool wireframe) override;
};

}  // namespace kEn
