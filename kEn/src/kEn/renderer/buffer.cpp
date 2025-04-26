#include <kEn/core/assert.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/renderer_api.hpp>
#include <kenpch.hpp>
#include <platform/opengl/opengl_buffer.hpp>

namespace kEn {

std::shared_ptr<VertexBuffer> VertexBuffer::create(void* vertices, size_t size) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglVertexBuffer>(vertices, size);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

std::shared_ptr<MutableVertexBuffer> MutableVertexBuffer::create(void* vertices, size_t size) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglMutableVertexBuffer>(vertices, size);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

std::shared_ptr<IndexBuffer> IndexBuffer::create(uint32_t* indices, size_t size) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglIndexBuffer>(indices, size);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

std::shared_ptr<MutableIndexBuffer> MutableIndexBuffer::create(uint32_t* indices, size_t size) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglMutableIndexBuffer>(indices, size);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

}  // namespace kEn
