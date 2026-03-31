#pragma once

#include <cstddef>
#include <cstdint>

#include <mEn/vec4.hpp>

#include <kEn/renderer/attachment_handle.hpp>
#include <kEn/renderer/render_state.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/vertex_input.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

class Texture;
class UniformBuffer;
class ShaderStorageBuffer;
class Framebuffer;

/** @brief Primitive topology used in draw calls. */
enum class RenderMode : uint8_t {
  Points,        /**< @brief Individual points. */
  LineStrip,     /**< @brief Connected line segments forming a strip. */
  LineLoop,      /**< @brief Connected line segments forming a closed loop. */
  Lines,         /**< @brief Pairs of vertices forming independent lines. */
  TriangleStrip, /**< @brief Connected triangles sharing edges in a strip. */
  TriangleFan,   /**< @brief Fan of triangles sharing a common first vertex. */
  Triangles,     /**< @brief Independent triangles, three vertices each. */
  Patches,       /**< @brief Tessellation control shader input patches. */
  LinesAdjacency /**< @brief Lines with adjacency data for geometry shaders. */
};

/** @brief Convenience namespace that imports all @ref RenderMode enumerators. */
namespace render_mode {

using enum RenderMode;

}  // namespace render_mode

/**
 * @brief Abstract rendering context providing GPU pipeline control.
 *
 * RenderContext is the central interface for submitting draw calls and configuring
 * pipeline state: clear operations, viewport and scissor rectangles, depth/blend/raster
 * state, shader and vertex input binding, resource binding, render targets, and
 * tessellation parameters.
 *
 * Instances are obtained from the platform layer and are not created directly.
 */
class RenderContext {
 public:
  virtual ~RenderContext() = default;

  /**
   * @brief Initializes the rendering context.
   * @param enable_debug Enables backend debug output when @c true.
   */
  virtual void init(bool enable_debug) = 0;

  /** @brief Initializes the rendering context with debug output disabled. */
  void init() { init(false); }

  /**
   * @brief Sets the color used by clear() and clear_color().
   * @param color RGBA clear color.
   */
  virtual void set_clear_color(const mEn::Vec4& color) = 0;

  /**
   * @brief Sets the value written to the depth buffer by clear() and clear_depth().
   * @param depth Depth clear value in [0, 1].
   */
  virtual void set_clear_depth(float depth) = 0;

  /** @brief Clears the color, depth, and stencil buffers. */
  virtual void clear() = 0;
  /** @brief Clears the color buffer. */
  virtual void clear_color() = 0;
  /** @brief Clears the depth buffer. */
  virtual void clear_depth() = 0;
  /** @brief Clears the stencil buffer. */
  virtual void clear_stencil() = 0;
  /** @brief Clears the depth and stencil buffers. */
  virtual void clear_depth_stencil() = 0;

  /**
   * @brief Sets the rendering viewport.
   * @param x Left edge in pixels.
   * @param y Bottom edge in pixels.
   * @param w Width in pixels.
   * @param h Height in pixels.
   */
  virtual void set_viewport(std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h) = 0;

  /**
   * @brief Sets the scissor rectangle.
   * @param x Left edge in pixels.
   * @param y Bottom edge in pixels.
   * @param w Width in pixels.
   * @param h Height in pixels.
   */
  virtual void set_scissor_rect(std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h) = 0;

  /**
   * @brief Applies depth and stencil test state.
   * @param state Depth/stencil state descriptor.
   */
  virtual void set_depth_state(const DepthState& state) = 0;

  /**
   * @brief Applies blending state.
   * @param state Blend state descriptor.
   */
  virtual void set_blend_state(const BlendState& state) = 0;

  /**
   * @brief Applies rasterizer state.
   * @param state Raster state descriptor.
   */
  virtual void set_raster_state(const RasterState& state) = 0;

  /**
   * @brief Binds a compiled shader program for subsequent draw calls.
   * @param shader Shader program to bind.
   */
  virtual void set_shader(const Shader& shader) = 0;

  /**
   * @brief Binds vertex input (buffer bindings and attribute layout) for subsequent draw calls.
   * @param vertex_input Vertex input object to bind.
   */
  virtual void set_vertex_input(const VertexInput& vertex_input) = 0;

  /**
   * @brief Binds a texture to a shader-accessible slot.
   * @param slot    Texture unit index.
   * @param stage   Pipeline stage hint; used by the backend to select the correct binding point.
   * @param texture Texture to bind.
   */
  virtual void bind_texture(std::uint32_t slot, ShaderStage stage, const Texture& texture) = 0;

  /**
   * @brief Binds a framebuffer attachment as a read-only texture.
   * @param slot   Texture unit index.
   * @param stage  Pipeline stage hint; used by the backend to select the correct binding point.
   * @param handle Attachment handle referencing the underlying texture resource.
   */
  virtual void bind_attachment(std::uint32_t slot, ShaderStage stage, AttachmentHandle handle) = 0;

  /**
   * @brief Binds a uniform buffer to a shader binding point.
   * @param binding Uniform buffer binding index.
   * @param stage   Pipeline stage hint; used by the backend to select the correct binding point.
   * @param ubo     Uniform buffer to bind.
   */
  virtual void bind_uniform_buffer(std::uint32_t binding, ShaderStage stage, const UniformBuffer& ubo) = 0;

  /**
   * @brief Binds a shader storage buffer to a binding point.
   * @param binding Storage buffer binding index.
   * @param stage   Pipeline stage hint; used by the backend to select the correct binding point.
   * @param ssbo    Shader storage buffer to bind.
   */
  virtual void bind_storage_buffer(std::uint32_t binding, ShaderStage stage, const ShaderStorageBuffer& ssbo) = 0;

  /**
   * @brief Redirects subsequent draw calls to the given framebuffer.
   * @param framebuffer Target framebuffer.
   */
  virtual void set_render_target(Framebuffer& framebuffer) = 0;

  /** @brief Restores the default (window) framebuffer as the render target. */
  virtual void bind_default_framebuffer() = 0;

  /**
   * @brief Issues a non-indexed draw call.
   * @param vertex_count Number of vertices to draw.
   * @param start_vertex Index of the first vertex.
   * @param mode         Primitive topology.
   */
  virtual void draw(std::size_t vertex_count, std::uint32_t start_vertex, RenderMode mode) = 0;

  /**
   * @brief Issues a non-indexed instanced draw call.
   * @param vertex_count   Number of vertices per instance.
   * @param instance_count Number of instances to draw.
   * @param start_vertex   Index of the first vertex.
   * @param start_instance Instance ID offset added to each instance index.
   * @param mode           Primitive topology.
   */
  virtual void draw_instanced(std::size_t vertex_count, std::size_t instance_count, std::uint32_t start_vertex,
                              std::uint32_t start_instance, RenderMode mode) = 0;

  /**
   * @brief Issues an indexed draw call.
   * @param index_count Number of indices to read.
   * @param start_index Logical index of the first element in the index buffer.
   * @param base_vertex Constant offset added to each index value before fetching a vertex.
   * @param mode        Primitive topology.
   */
  virtual void draw_indexed(std::size_t index_count, std::uint32_t start_index, std::int32_t base_vertex,
                            RenderMode mode) = 0;

  /**
   * @brief Issues an indexed instanced draw call.
   * @param index_count    Number of indices to read.
   * @param instance_count Number of instances to draw.
   * @param start_index    Logical index of the first element in the index buffer.
   * @param base_vertex    Constant offset added to each index value before fetching a vertex.
   * @param start_instance Instance ID offset added to each instance index.
   * @param mode           Primitive topology.
   */
  virtual void draw_indexed_instanced(std::size_t index_count, std::size_t instance_count, std::uint32_t start_index,
                                      std::int32_t base_vertex, std::uint32_t start_instance, RenderMode mode) = 0;

  /**
   * @brief Sets the number of vertices per tessellation patch.
   * @param count Vertices per patch.
   */
  virtual void set_tessellation_patch_vertices(std::size_t count) = 0;

  /**
   * @brief Returns the maximum tessellation level supported by the GPU.
   * @return Maximum tessellation generation level.
   */
  [[nodiscard]] virtual std::size_t max_tessellation_level() const = 0;
};

}  // namespace kEn
