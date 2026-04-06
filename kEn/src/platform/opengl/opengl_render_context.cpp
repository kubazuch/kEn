#include "opengl_render_context.hpp"

#include <glad/gl.h>

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <mEn/fwd.hpp>

#include <kEn/core/assert.hpp>
#include <kEn/core/log.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/framebuffer.hpp>
#include <kEn/renderer/render_context.hpp>
#include <kEn/renderer/render_state.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/renderer/vertex_input.hpp>

#include "opengl_state.hpp"

namespace kEn {

namespace {

void gl_message_callback(GLenum /*src*/, GLenum /*type*/, GLuint /*id*/, GLenum lvl, GLsizei /*len*/, const GLchar* msg,
                         const void* /*params*/) {
  switch (lvl) {
    case GL_DEBUG_SEVERITY_HIGH:
      KEN_CORE_CRITICAL(msg);
      return;
    case GL_DEBUG_SEVERITY_MEDIUM:
      KEN_CORE_ERROR(msg);
      return;
    case GL_DEBUG_SEVERITY_LOW:
      KEN_CORE_WARN(msg);
      return;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      KEN_CORE_TRACE(msg);
      return;
    default:
      KEN_CORE_ASSERT(false, "Unknown message severity!");
      return;
  }
}

}  // namespace

void OpenglRenderContext::init(bool enable_debug) {
  if (enable_debug) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_message_callback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
  }

  glEnable(GL_LINE_SMOOTH);
  // glEnable(GL_MULTISAMPLE);

  set_depth_state(OpenglDepthState(DepthStateDesc{}));
  set_blend_state(OpenglBlendState(BlendStateDesc{}));
  set_raster_state(OpenglRasterState(RasterStateDesc{}));
}

void OpenglRenderContext::set_clear_color(const mEn::Vec4& color) { glClearColor(color.r, color.g, color.b, color.a); }
void OpenglRenderContext::set_clear_depth(float depth) { glClearDepthf(depth); }

void OpenglRenderContext::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }
void OpenglRenderContext::clear_color() { glClear(GL_COLOR_BUFFER_BIT); }
void OpenglRenderContext::clear_depth() { glClear(GL_DEPTH_BUFFER_BIT); }
void OpenglRenderContext::clear_stencil() { glClear(GL_STENCIL_BUFFER_BIT); }
void OpenglRenderContext::clear_depth_stencil() { glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }

void OpenglRenderContext::set_viewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) {
  glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void OpenglRenderContext::set_scissor_rect(std::uint32_t x, std::uint32_t y, std::uint32_t width,
                                           std::uint32_t height) {
  glScissor(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void OpenglRenderContext::set_depth_state(const DepthState& state) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
  const DepthStateDesc& d = static_cast<const OpenglDepthState&>(state).desc();

  if (d.depth_test_enabled) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
  glDepthMask(d.depth_write_enabled ? GL_TRUE : GL_FALSE);
  glDepthFunc(render_state::opengl_compare_op(d.depth_compare_op));

  if (d.stencil_test_enabled) {
    glEnable(GL_STENCIL_TEST);
  } else {
    glDisable(GL_STENCIL_TEST);
  }

  const auto apply_face = [](GLenum face, const StencilFaceDesc& f) {
    glStencilFuncSeparate(face, render_state::opengl_compare_op(f.compare_op), f.reference, f.read_mask);
    glStencilOpSeparate(face, render_state::opengl_stencil_op(f.fail_op),
                        render_state::opengl_stencil_op(f.depth_fail_op), render_state::opengl_stencil_op(f.pass_op));
    glStencilMaskSeparate(face, f.write_mask);
  };
  apply_face(GL_FRONT, d.front_face);
  apply_face(GL_BACK, d.back_face);
}

void OpenglRenderContext::set_blend_state(const BlendState& state) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
  const BlendStateDesc& d = static_cast<const OpenglBlendState&>(state).desc();

  if (d.blend_enabled) {
    glEnable(GL_BLEND);
  } else {
    glDisable(GL_BLEND);
  }
  glBlendFuncSeparate(render_state::opengl_blend_factor(d.src_color), render_state::opengl_blend_factor(d.dst_color),
                      render_state::opengl_blend_factor(d.src_alpha), render_state::opengl_blend_factor(d.dst_alpha));
  glBlendEquationSeparate(render_state::opengl_blend_op(d.color_op), render_state::opengl_blend_op(d.alpha_op));
  glColorMask(d.write_r ? GL_TRUE : GL_FALSE, d.write_g ? GL_TRUE : GL_FALSE, d.write_b ? GL_TRUE : GL_FALSE,
              d.write_a ? GL_TRUE : GL_FALSE);
}

void OpenglRenderContext::set_raster_state(const RasterState& state) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
  const RasterStateDesc& d = static_cast<const OpenglRasterState&>(state).desc();

  glPolygonMode(GL_FRONT_AND_BACK, d.fill_mode == FillMode::Wireframe ? GL_LINE : GL_FILL);

  if (d.cull_mode == CullMode::None) {
    glDisable(GL_CULL_FACE);
  } else {
    glEnable(GL_CULL_FACE);
    glCullFace(d.cull_mode == CullMode::Front ? GL_FRONT : GL_BACK);
  }
  glFrontFace(d.front_face == FrontFace::Ccw ? GL_CCW : GL_CW);

  if (d.scissor_test_enabled) {
    glEnable(GL_SCISSOR_TEST);
  } else {
    glDisable(GL_SCISSOR_TEST);
  }

  const bool has_bias = d.depth_bias_constant != 0.0F || d.depth_bias_slope != 0.0F;
  if (has_bias) {
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(d.depth_bias_slope, d.depth_bias_constant);
  } else {
    glDisable(GL_POLYGON_OFFSET_FILL);
  }
}

void OpenglRenderContext::set_shader(const Shader& shader) {
  glUseProgram(static_cast<GLuint>(shader.native_handle()));
}

void OpenglRenderContext::set_vertex_input(const VertexInput& vertex_input) {
  glBindVertexArray(static_cast<GLuint>(vertex_input.native_handle()));
}

void OpenglRenderContext::bind_texture(std::uint32_t slot, ShaderStage /*stage*/, const Texture& texture) {
  glBindTextureUnit(slot, static_cast<GLuint>(texture.native_handle()));
}

void OpenglRenderContext::bind_attachment(std::uint32_t slot, ShaderStage /*stage*/, AttachmentHandle handle) {
  glBindTextureUnit(slot, static_cast<GLuint>(handle.value));
}

void OpenglRenderContext::bind_uniform_buffer(std::uint32_t binding, ShaderStage /*stage*/, const UniformBuffer& ubo) {
  glBindBufferBase(GL_UNIFORM_BUFFER, binding, static_cast<GLuint>(ubo.underlying_buffer()->native_handle()));
}

void OpenglRenderContext::bind_storage_buffer(std::uint32_t binding, ShaderStage /*stage*/,
                                              const ShaderStorageBuffer& ssbo) {
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, static_cast<GLuint>(ssbo.underlying_buffer()->native_handle()));
}

void OpenglRenderContext::set_render_target(Framebuffer& framebuffer) {
  glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(framebuffer.native_handle()));
}

void OpenglRenderContext::bind_default_framebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenglRenderContext::draw(std::size_t vertex_count, std::uint32_t start_vertex, RenderMode mode) {
  glDrawArrays(render_mode::opengl_mode(mode), static_cast<GLint>(start_vertex), static_cast<GLsizei>(vertex_count));
}

void OpenglRenderContext::draw_instanced(std::size_t vertex_count, std::size_t instance_count,
                                         std::uint32_t start_vertex, std::uint32_t start_instance, RenderMode mode) {
  glDrawArraysInstancedBaseInstance(render_mode::opengl_mode(mode), static_cast<GLint>(start_vertex),
                                    static_cast<GLsizei>(vertex_count), static_cast<GLsizei>(instance_count),
                                    start_instance);
}

void OpenglRenderContext::draw_indexed(std::size_t index_count, std::uint32_t start_index, std::int32_t base_vertex,
                                       RenderMode mode) {
  const auto byte_offset = static_cast<uintptr_t>(start_index * sizeof(GLuint));
  glDrawElementsBaseVertex(  //
      render_mode::opengl_mode(mode), static_cast<GLsizei>(index_count), GL_UNSIGNED_INT,
      reinterpret_cast<void*>(byte_offset), base_vertex);  // NOLINT(performance-no-int-to-ptr)
}

void OpenglRenderContext::draw_indexed_instanced(std::size_t index_count, std::size_t instance_count,
                                                 std::uint32_t start_index, std::int32_t base_vertex,
                                                 std::uint32_t start_instance, RenderMode mode) {
  const auto byte_offset = static_cast<uintptr_t>(start_index * sizeof(GLuint));
  glDrawElementsInstancedBaseVertexBaseInstance(  //
      render_mode::opengl_mode(mode), static_cast<GLsizei>(index_count), GL_UNSIGNED_INT,
      reinterpret_cast<void*>(byte_offset),  // NOLINT(performance-no-int-to-ptr)
      static_cast<GLsizei>(instance_count), base_vertex, start_instance);
}

void OpenglRenderContext::set_tessellation_patch_vertices(std::size_t count) {
  glPatchParameteri(GL_PATCH_VERTICES, static_cast<GLsizei>(count));
}

std::size_t OpenglRenderContext::max_tessellation_level() const {
  if (!max_tessellation_level_) {
    GLint val = 0;
    glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &val);
    max_tessellation_level_ = static_cast<std::size_t>(val);
  }
  return *max_tessellation_level_;
}

}  // namespace kEn
