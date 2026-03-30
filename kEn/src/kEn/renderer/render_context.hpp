#pragma once

#include <cstddef>
#include <cstdint>

#include <mEn/vec4.hpp>

#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/vertex_input.hpp>

namespace kEn {

class Texture;
class UniformBuffer;
class ShaderStorageBuffer;
class Framebuffer;

enum class CullMode : uint8_t { None, Front, Back };

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

class RenderContext {
 public:
  virtual ~RenderContext() = default;

  virtual void init(bool enable_debug) = 0;
  void init() { init(false); }

  virtual void set_viewport(size_t x, size_t y, size_t w, size_t h) = 0;
  virtual void set_clear_color(const mEn::Vec4& color)              = 0;
  virtual void clear_color()                                        = 0;
  virtual void clear_depth()                                        = 0;
  virtual void clear()                                              = 0;

  virtual void depth_testing(bool enabled) = 0;
  void depth_testing() { depth_testing(true); }

  virtual void set_cull_mode(CullMode mode) = 0;

  virtual void set_shader(const Shader& shader)                  = 0;
  virtual void set_vertex_input(const VertexInput& vertex_input) = 0;

  virtual void bind_texture(std::uint32_t slot, ShaderStage stage, const Texture& texture) = 0;

  virtual void bind_uniform_buffer(std::uint32_t binding, ShaderStage stage, const UniformBuffer& ubo)        = 0;
  virtual void bind_storage_buffer(std::uint32_t binding, ShaderStage stage, const ShaderStorageBuffer& ssbo) = 0;

  virtual void set_render_target(Framebuffer& framebuffer) = 0;
  virtual void bind_default_framebuffer()                  = 0;

  virtual void bind_attachment(std::uint32_t slot, ShaderStage stage, std::uintptr_t handle) = 0;

  virtual void draw(size_t vertex_count, RenderMode mode)                                         = 0;
  virtual void draw_indexed(size_t index_count, RenderMode mode)                                  = 0;
  virtual void draw_instanced(size_t vertex_count, size_t instance_count, RenderMode mode)        = 0;
  virtual void draw_indexed_instanced(size_t index_count, size_t instance_count, RenderMode mode) = 0;

  virtual void set_tessellation_patch_vertices(size_t count) = 0;
  virtual int max_tesselation_level() const                  = 0;

  virtual void set_wireframe(bool wireframe) = 0;
};

}  // namespace kEn
