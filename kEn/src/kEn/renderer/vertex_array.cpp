#include <kEn/renderer/renderer_api.hpp>
#include <kEn/renderer/vertex_array.hpp>
#include <kenpch.hpp>
#include <platform/opengl/opengl_vertex_array.hpp>

namespace kEn {

std::unique_ptr<VertexArray> VertexArray::create() {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_unique<OpenglVertexArray>();
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

}  // namespace kEn
