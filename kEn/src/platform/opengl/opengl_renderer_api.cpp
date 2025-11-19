#include "opengl_renderer_api.hpp"

#include <glad/gl.h>

#include <cstddef>

#include <kEn/renderer/renderer_api.hpp>
#include <kEn/renderer/vertex_array.hpp>

namespace kEn {

RendererApi::Api RendererApi::api_ = Api::OpenGL;

namespace {

constexpr GLenum draw_mode(RendererApi::RenderMode mode) {
  switch (mode) {
    case RendererApi::RenderMode::Points:
      return GL_POINTS;
    case RendererApi::RenderMode::LineStrip:
      return GL_LINE_STRIP;
    case RendererApi::RenderMode::LineLoop:
      return GL_LINE_LOOP;
    case RendererApi::RenderMode::Lines:
      return GL_LINES;
    case RendererApi::RenderMode::TriangleStrip:
      return GL_TRIANGLE_STRIP;
    case RendererApi::RenderMode::TriangleFan:
      return GL_TRIANGLE_FAN;
    case RendererApi::RenderMode::Triangles:
      return GL_TRIANGLES;
    case RendererApi::RenderMode::Patches:
      return GL_PATCHES;
    case kEn::RendererApi::RenderMode::LinesAdjacency:
      return GL_LINES_ADJACENCY;
    default:
      KEN_CORE_ASSERT(false, "Unknown draw mode!");
      return 0;
  }
}

void gl_message_callback(unsigned /*src*/, unsigned /*type*/, unsigned /*id*/, unsigned lvl, int /*len*/,
                         const char* msg, const void* /*params*/) {
  switch (lvl) {  // NOLINT
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
  }

  KEN_CORE_ASSERT(false, "Unknown message severity!");
}

}  // namespace

void OpenglRendererApi::init() {
#ifdef _KEN_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(gl_message_callback, nullptr);

  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_LINE_SMOOTH);
  // glEnable(GL_MULTISAMPLE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
}

void OpenglRendererApi::set_viewport(size_t x, size_t y, size_t width, size_t height) {
  glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void OpenglRendererApi::set_clear_color(const mEn::Vec4& color) { glClearColor(color.r, color.g, color.b, color.a); }

void OpenglRendererApi::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenglRendererApi::depth_testing(bool enabled) {
  if (enabled) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
}

void OpenglRendererApi::draw_indexed(const VertexArray& vertex_array, size_t index_count,
                                     RendererApi::RenderMode mode) {
  vertex_array.bind();
  glDrawElements(draw_mode(mode), static_cast<GLsizei>(index_count), GL_UNSIGNED_INT, nullptr);
}

void OpenglRendererApi::draw(const VertexArray& vertex_array, size_t vertex_count, RendererApi::RenderMode mode) {
  vertex_array.bind();
  glDrawArrays(draw_mode(mode), 0, static_cast<GLsizei>(vertex_count));
}

void OpenglRendererApi::draw_indexed_instanced(const VertexArray& vertex_array, size_t index_count,
                                               size_t instance_count, RendererApi::RenderMode mode) {
  vertex_array.bind();
  glDrawElementsInstanced(draw_mode(mode), static_cast<GLsizei>(index_count), GL_UNSIGNED_INT, nullptr,
                          static_cast<GLsizei>(instance_count));
}

void OpenglRendererApi::draw_instanced(const VertexArray& vertex_array, size_t vertex_count, size_t instance_count,
                                       RendererApi::RenderMode mode) {
  vertex_array.bind();
  glDrawArraysInstanced(draw_mode(mode), 0, static_cast<GLsizei>(vertex_count), static_cast<GLsizei>(instance_count));
}

void OpenglRendererApi::set_tessellation_patch_vertices(size_t count) {
  glPatchParameteri(GL_PATCH_VERTICES, static_cast<GLsizei>(count));
}

void OpenglRendererApi::set_wireframe(bool wireframe) {
  glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(wireframe ? GL_LINE : GL_FILL));
}

int OpenglRendererApi::max_tesselation_level() const {
  if (max_tesselation_level_ > 0) {
    return max_tesselation_level_;
  }

  glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &max_tesselation_level_);
  return max_tesselation_level_;
}

}  // namespace kEn
