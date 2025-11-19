#include "buffer.hpp"

#include <memory>

#include <kEn/core/assert.hpp>
#include <kEn/renderer/renderer_api.hpp>

#include <platform/opengl/opengl_buffer.hpp>

namespace kEn {

std::shared_ptr<Buffer> Buffer::create(const void* data, size_t size) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglBuffer>(data, size);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

std::shared_ptr<MutableBuffer> MutableBuffer::create(const void* data, size_t size) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglMutableBuffer>(data, size);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

std::shared_ptr<UniformBuffer> UniformBuffer::create(const std::shared_ptr<Buffer>& buffer, size_t binding_point) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglUniformBuffer>(std::dynamic_pointer_cast<OpenglBuffer>(buffer), binding_point);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

std::shared_ptr<ShaderStorageBuffer> ShaderStorageBuffer::create(const std::shared_ptr<Buffer>& buffer,
                                                                 size_t binding_point) {
  switch (RendererApi::get_api()) {
    case RendererApi::Api::None:
      KEN_CORE_ASSERT(false, "Renderer has no api!");
      return nullptr;
    case RendererApi::Api::OpenGL:
      return std::make_shared<OpenglShaderStorageBuffer>(std::dynamic_pointer_cast<OpenglBuffer>(buffer),
                                                         binding_point);
  }

  KEN_CORE_ASSERT(false, "Unknown api!");
  return nullptr;
}

}  // namespace kEn
