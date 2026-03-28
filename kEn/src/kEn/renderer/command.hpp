#pragma once

#include <cstddef>
#include <cstdint>

#include <mEn/vec4.hpp>

#include <kEn/renderer/vertex_input.hpp>

namespace kEn {

enum class RenderMode : uint8_t {
  Points,
  LineStrip,
  LineLoop,
  Lines,
  TriangleStrip,
  TriangleFan,
  Triangles,
  Patches,
  LinesAdjacency
};

namespace render_mode {

using enum RenderMode;

}  // namespace render_mode

class Command {
 public:
  virtual ~Command() = default;

  virtual void init(bool enable_debug)                              = 0;
  virtual void set_viewport(size_t x, size_t y, size_t w, size_t h) = 0;
  virtual void set_clear_color(const mEn::Vec4& color)              = 0;
  virtual void clear()                                              = 0;
  virtual void depth_testing(bool enabled)                          = 0;
  virtual void bind_default_framebuffer()                           = 0;

  void init() { init(false); }
  void depth_testing() { depth_testing(true); }

  virtual void draw_indexed(const VertexInput& vertex_array, size_t index_count, RenderMode mode) = 0;
  virtual void draw(const VertexInput& vertex_array, size_t vertex_count, RenderMode mode)        = 0;
  virtual void draw_indexed_instanced(const VertexInput& vertex_array, size_t index_count, size_t instance_count,
                                      RenderMode mode)                                            = 0;
  virtual void draw_instanced(const VertexInput& vertex_array, size_t vertex_count, size_t instance_count,
                              RenderMode mode)                                                    = 0;

  virtual void set_tessellation_patch_vertices(size_t count) = 0;
  virtual void set_wireframe(bool wireframe)                 = 0;
  virtual int max_tesselation_level() const                  = 0;
};

}  // namespace kEn
