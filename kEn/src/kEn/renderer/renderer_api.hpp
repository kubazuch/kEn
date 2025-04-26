#pragma once

#include <kEn/renderer/vertex_array.hpp>
#include <mEn/vec4.hpp>

namespace kEn {

class RendererApi {
 public:
  enum class Api { None = 0, OpenGL = 1 };

 public:
  virtual ~RendererApi() = default;

  virtual void init()                                               = 0;
  virtual void set_viewport(size_t x, size_t y, size_t w, size_t h) = 0;
  virtual void set_clear_color(const mEn::Vec4& color)              = 0;
  virtual void clear()                                              = 0;
  virtual void depth_testing(bool enabled)                          = 0;

  virtual void draw_indexed(const VertexArray& vertex_array, size_t index_count)           = 0;
  virtual void draw_lines(const VertexArray& vertex_array, bool strip, size_t index_count) = 0;
  virtual void draw_patches(const VertexArray& vertex_array, size_t vertex_count)          = 0;

  virtual void set_tessellation_patch_vertices(size_t count) = 0;

  virtual void set_wireframe(bool wireframe) = 0;

  static Api get_api() { return api_; }
  static std::unique_ptr<RendererApi> create();

 private:
  static Api api_;
};

}  // namespace kEn
