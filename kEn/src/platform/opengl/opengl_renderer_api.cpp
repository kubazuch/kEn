#include <glad/gl.h>

#include <kEn/renderer/renderer_api.hpp>
#include <kEn/renderer/vertex_array.hpp>
#include <kenpch.hpp>
#include <platform/opengl/opengl_renderer_api.hpp>

namespace kEn {

RendererApi::Api RendererApi::api_ = Api::OpenGL;

void gl_message_callback(unsigned /*src*/, unsigned /*type*/, unsigned /*id*/, unsigned lvl, int /*len*/,
                         const char* msg, const void* /*params*/) {
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
  }

  KEN_CORE_ASSERT(false, "Unknown message severity!");
}

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
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_MULTISAMPLE);

  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  // glFrontFace(GL_CCW);
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

void OpenglRendererApi::draw_indexed(const VertexArray& vertex_array, size_t index_count) {
  vertex_array.bind();
  uint32_t count = index_count ? index_count : vertex_array.index_buffer()->get_count();

  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
}

void OpenglRendererApi::draw_lines(const VertexArray& vertex_array, bool strip, size_t index_count) {
  vertex_array.bind();
  uint32_t count = index_count ? index_count : vertex_array.index_buffer()->get_count();

  glDrawElements(strip ? GL_LINE_STRIP : GL_LINES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
}

void OpenglRendererApi::draw_patches(const VertexArray& vertex_array, size_t vertex_count) {
  vertex_array.bind();
  glDrawArrays(GL_PATCHES, 0, static_cast<GLsizei>(vertex_count));
}

void OpenglRendererApi::set_tessellation_patch_vertices(size_t count) {
  glPatchParameteri(GL_PATCH_VERTICES, static_cast<GLsizei>(count));
}

void OpenglRendererApi::set_wireframe(bool wireframe) {
  glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}

}  // namespace kEn
