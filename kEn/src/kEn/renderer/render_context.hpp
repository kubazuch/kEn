#pragma once

#include <cstddef>
#include <cstdint>

#include <mEn/vec4.hpp>

#include <kEn/renderer/attachment_handle.hpp>
#include <kEn/renderer/render_state.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/vertex_input.hpp>

namespace kEn {

class Texture;
class UniformBuffer;
class ShaderStorageBuffer;
class Framebuffer;

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

  virtual void set_clear_color(const mEn::Vec4& color) = 0;
  virtual void set_clear_depth(float depth)            = 0;

  virtual void clear()               = 0;
  virtual void clear_color()         = 0;
  virtual void clear_depth()         = 0;
  virtual void clear_stencil()       = 0;
  virtual void clear_depth_stencil() = 0;

  virtual void set_viewport(std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h)     = 0;
  virtual void set_scissor_rect(std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h) = 0;

  virtual void set_depth_state(const DepthState& state)   = 0;
  virtual void set_blend_state(const BlendState& state)   = 0;
  virtual void set_raster_state(const RasterState& state) = 0;

  virtual void set_shader(const Shader& shader)                  = 0;
  virtual void set_vertex_input(const VertexInput& vertex_input) = 0;

  // NOTE: The `stage` parameter in the four bind_* methods below is a hint for stage-aware backends (e.g. D3D11,
  // where textures and buffers are bound per-stage). OpenGL implementations ignore it -- all resource bindings
  // are global and not stage-scoped.
  virtual void bind_texture(std::uint32_t slot, ShaderStage stage, const Texture& texture)                    = 0;
  virtual void bind_attachment(std::uint32_t slot, ShaderStage stage, AttachmentHandle handle)                = 0;
  virtual void bind_uniform_buffer(std::uint32_t binding, ShaderStage stage, const UniformBuffer& ubo)        = 0;
  virtual void bind_storage_buffer(std::uint32_t binding, ShaderStage stage, const ShaderStorageBuffer& ssbo) = 0;

  virtual void set_render_target(Framebuffer& framebuffer) = 0;
  virtual void bind_default_framebuffer()                  = 0;

  virtual void draw(std::size_t vertex_count, std::uint32_t start_vertex, RenderMode mode)                     = 0;
  virtual void draw_instanced(std::size_t vertex_count, std::size_t instance_count, std::uint32_t start_vertex,
                              std::uint32_t start_instance, RenderMode mode)                                   = 0;
  virtual void draw_indexed(std::size_t index_count, std::uint32_t start_index, std::int32_t base_vertex,
                            RenderMode mode)                                                                   = 0;
  virtual void draw_indexed_instanced(std::size_t index_count, std::size_t instance_count, std::uint32_t start_index,
                                      std::int32_t base_vertex, std::uint32_t start_instance, RenderMode mode) = 0;

  virtual void set_tessellation_patch_vertices(std::size_t count)  = 0;
  [[nodiscard]] virtual std::size_t max_tessellation_level() const = 0;
};

}  // namespace kEn
