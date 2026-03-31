#pragma once

#include <glad/gl.h>

#include <optional>

#include <kEn/renderer/attachment_handle.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/framebuffer.hpp>
#include <kEn/renderer/render_context.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/util/enum_map.hpp>

namespace kEn {

/** @brief OpenGL-specific utilities for @ref RenderMode. */
namespace render_mode {

namespace detail {

/** @brief Lookup table mapping each @ref RenderMode to its OpenGL primitive constant. */
inline constexpr auto kOpenglRenderModes = util::make_enum_map<GLenum>({
    std::pair{Points, GL_POINTS},
    std::pair{LineStrip, GL_LINE_STRIP},
    std::pair{LineLoop, GL_LINE_LOOP},
    std::pair{Lines, GL_LINES},
    std::pair{TriangleStrip, GL_TRIANGLE_STRIP},
    std::pair{TriangleFan, GL_TRIANGLE_FAN},
    std::pair{Triangles, GL_TRIANGLES},
    std::pair{Patches, GL_PATCHES},
    std::pair{LinesAdjacency, GL_LINES_ADJACENCY},
});

}  // namespace detail

/**
 * @brief Converts a @ref RenderMode to the corresponding OpenGL primitive constant.
 * @param mode Render mode to convert.
 * @return OpenGL @c GLenum primitive type.
 */
[[nodiscard]] constexpr GLenum opengl_mode(RenderMode mode) { return detail::kOpenglRenderModes[mode]; }

}  // namespace render_mode

/**
 * @brief OpenGL implementation of @ref RenderContext.
 *
 * On init(), line smoothing is enabled and default depth, blend, and raster states are
 * applied.  When @c enable_debug is @c true, @c GL_DEBUG_OUTPUT is enabled and driver
 * messages are forwarded to the engine logging system at matching severity levels.
 *
 * The @c stage parameter accepted by the four @c bind_* methods is ignored: all OpenGL
 * resource bindings are global and not scoped to a pipeline stage.
 */
class OpenglRenderContext final : public RenderContext {
 public:
  void init(bool enable_debug) override;

  void set_clear_color(const mEn::Vec4& color) override;
  void set_clear_depth(float depth) override;

  void clear() override;
  void clear_color() override;
  void clear_depth() override;
  void clear_stencil() override;
  void clear_depth_stencil() override;

  void set_viewport(std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h) override;
  void set_scissor_rect(std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h) override;

  void set_depth_state(const DepthState& state) override;
  void set_blend_state(const BlendState& state) override;
  void set_raster_state(const RasterState& state) override;

  void set_shader(const Shader& shader) override;
  void set_vertex_input(const VertexInput& vertex_input) override;

  void bind_texture(std::uint32_t slot, ShaderStage stage, const Texture& texture) override;
  void bind_attachment(std::uint32_t slot, ShaderStage stage, AttachmentHandle handle) override;
  void bind_uniform_buffer(std::uint32_t binding, ShaderStage stage, const UniformBuffer& ubo) override;
  void bind_storage_buffer(std::uint32_t binding, ShaderStage stage, const ShaderStorageBuffer& ssbo) override;

  void set_render_target(Framebuffer& framebuffer) override;
  void bind_default_framebuffer() override;

  void draw(std::size_t vertex_count, std::uint32_t start_vertex, RenderMode mode) override;
  void draw_instanced(std::size_t vertex_count, std::size_t instance_count, std::uint32_t start_vertex,
                      std::uint32_t start_instance, RenderMode mode) override;

  void draw_indexed(std::size_t index_count, std::uint32_t start_index, std::int32_t base_vertex,
                    RenderMode mode) override;
  void draw_indexed_instanced(std::size_t index_count, std::size_t instance_count, std::uint32_t start_index,
                              std::int32_t base_vertex, std::uint32_t start_instance, RenderMode mode) override;

  void set_tessellation_patch_vertices(std::size_t count) override;
  [[nodiscard]] std::size_t max_tessellation_level() const override;

 private:
  /** @brief Lazily cached result of querying @c GL_MAX_TESS_GEN_LEVEL. */
  mutable std::optional<std::size_t> max_tessellation_level_;
};

}  // namespace kEn
