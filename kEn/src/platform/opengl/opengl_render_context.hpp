#pragma once

#include <glad/gl.h>

#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/framebuffer.hpp>
#include <kEn/renderer/render_context.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/util/enum_map.hpp>

namespace kEn {

namespace render_mode {

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

[[nodiscard]] constexpr GLenum opengl_mode(RenderMode mode) { return kOpenglRenderModes[mode]; }

}  // namespace render_mode

class OpenglRenderContext final : public RenderContext {
 public:
  void init(bool enable_debug) override;

  void set_viewport(size_t x, size_t y, size_t w, size_t h) override;
  void set_clear_color(const mEn::Vec4& color) override;
  void clear_color() override;
  void clear_depth() override;
  void clear() override;

  void depth_testing(bool enabled) override;

  void set_shader(const Shader& shader) override;
  void set_vertex_input(const VertexInput& vertex_input) override;

  void bind_texture(std::uint32_t slot, ShaderStage stage, const Texture& texture) override;

  void bind_uniform_buffer(std::uint32_t binding, ShaderStage stage, const UniformBuffer& ubo) override;
  void bind_storage_buffer(std::uint32_t binding, ShaderStage stage, const ShaderStorageBuffer& ssbo) override;

  void set_render_target(Framebuffer& framebuffer) override;
  void bind_default_framebuffer() override;

  void bind_attachment(std::uint32_t slot, ShaderStage stage, std::uintptr_t handle) override;

  void draw(size_t vertex_count, RenderMode mode) override;
  void draw_indexed(size_t index_count, RenderMode mode) override;
  void draw_instanced(size_t vertex_count, size_t instance_count, RenderMode mode) override;
  void draw_indexed_instanced(size_t index_count, size_t instance_count, RenderMode mode) override;

  void set_tessellation_patch_vertices(size_t count) override;
  int max_tesselation_level() const override;

  void set_wireframe(bool wireframe) override;

 private:
  mutable int max_tesselation_level_ = -1;
};

}  // namespace kEn
