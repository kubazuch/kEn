#include "opengl_render_context.hpp"

#include <glad/gl.h>

#include <cstddef>
#include <cstdint>

#include <mEn/fwd.hpp>

#include <kEn/core/assert.hpp>
#include <kEn/core/log.hpp>
#include <kEn/renderer/attachment_handle.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/framebuffer.hpp>
#include <kEn/renderer/render_context.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/renderer/vertex_input.hpp>

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

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // TODO(kuzu): Add mask as some vendors dont care bout depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_LINE_SMOOTH);
  // glEnable(GL_MULTISAMPLE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
}

void OpenglRenderContext::set_viewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) {
  glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void OpenglRenderContext::set_clear_color(const mEn::Vec4& color) { glClearColor(color.r, color.g, color.b, color.a); }

void OpenglRenderContext::clear_color() { glClear(GL_COLOR_BUFFER_BIT); }

void OpenglRenderContext::clear_depth() { glClear(GL_DEPTH_BUFFER_BIT); }

void OpenglRenderContext::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenglRenderContext::depth_testing(bool enabled) {
  if (enabled) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
}

void OpenglRenderContext::set_cull_mode(CullMode mode) {
  if (mode == CullMode::None) {
    glDisable(GL_CULL_FACE);
    return;
  }
  glEnable(GL_CULL_FACE);
  glCullFace(mode == CullMode::Front ? GL_FRONT : GL_BACK);
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

void OpenglRenderContext::bind_attachment(std::uint32_t slot, ShaderStage /*stage*/, AttachmentHandle handle) {
  glBindTextureUnit(slot, static_cast<GLuint>(handle.value));
}

void OpenglRenderContext::draw(std::size_t vertex_count, RenderMode mode) {
  glDrawArrays(render_mode::opengl_mode(mode), 0, static_cast<GLsizei>(vertex_count));
}

void OpenglRenderContext::draw_indexed(std::size_t index_count, RenderMode mode) {
  glDrawElements(render_mode::opengl_mode(mode), static_cast<GLsizei>(index_count), GL_UNSIGNED_INT, nullptr);
}

void OpenglRenderContext::draw_instanced(std::size_t vertex_count, std::size_t instance_count, RenderMode mode) {
  glDrawArraysInstanced(render_mode::opengl_mode(mode), 0, static_cast<GLsizei>(vertex_count),
                        static_cast<GLsizei>(instance_count));
}

void OpenglRenderContext::draw_indexed_instanced(std::size_t index_count, std::size_t instance_count, RenderMode mode) {
  glDrawElementsInstanced(render_mode::opengl_mode(mode), static_cast<GLsizei>(index_count), GL_UNSIGNED_INT, nullptr,
                          static_cast<GLsizei>(instance_count));
}

void OpenglRenderContext::set_tessellation_patch_vertices(std::size_t count) {
  glPatchParameteri(GL_PATCH_VERTICES, static_cast<GLsizei>(count));
}

int OpenglRenderContext::max_tessellation_level() const {
  if (max_tessellation_level_ >= 0) {
    return max_tessellation_level_;
  }

  glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &max_tessellation_level_);
  return max_tessellation_level_;
}

void OpenglRenderContext::set_wireframe(bool wireframe) {
  glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(wireframe ? GL_LINE : GL_FILL));
}

}  // namespace kEn
