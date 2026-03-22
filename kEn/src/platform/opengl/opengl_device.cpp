#include "opengl_device.hpp"

#include <filesystem>
#include <memory>
#include <string_view>

#include <kEn/core/assert.hpp>
#include <kEn/core/log.hpp>
#include <kEn/imgui/imgui_backend.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/device.hpp>
#include <kEn/renderer/framebuffer.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/renderer/vertex_array.hpp>

#include "opengl_buffer.hpp"
#include "opengl_framebuffer.hpp"
#include "opengl_imgui_backend.hpp"
#include "opengl_shader.hpp"
#include "opengl_texture.hpp"
#include "opengl_vertex_array.hpp"

namespace kEn {

std::unique_ptr<Device> Device::create(Device::Api api, GLFWwindow* window, bool enable_debug) {
  switch (api) {
    case Device::Api::OpenGL:
      return std::make_unique<OpenglDevice>(window, enable_debug);
    default:
      KEN_CORE_CRITICAL("Only OpenGL is currently supported");
      KEN_UNREACHABLE();
  }
}

OpenglDevice::OpenglDevice(GLFWwindow* window, bool enable_debug) : context_(window) {
  context_.init();
  command_.init(enable_debug);
}

void OpenglDevice::swap_buffers() { context_.swap_buffers(); }

std::shared_ptr<Buffer> OpenglDevice::create_buffer(const void* data, std::size_t size) {
  return std::make_shared<OpenglBuffer>(data, size);
}

std::shared_ptr<MutableBuffer> OpenglDevice::create_mutable_buffer(const void* data, std::size_t size) {
  return std::make_shared<OpenglMutableBuffer>(data, size);
}

std::shared_ptr<UniformBuffer> OpenglDevice::create_uniform_buffer(const std::shared_ptr<Buffer>& buffer,
                                                                   std::size_t binding_point) {
  return std::make_shared<OpenglUniformBuffer>(std::dynamic_pointer_cast<OpenglBuffer>(buffer), binding_point);
}

std::shared_ptr<ShaderStorageBuffer> OpenglDevice::create_shader_storage_buffer(const std::shared_ptr<Buffer>& buffer,
                                                                                std::size_t binding_point) {
  return std::make_shared<OpenglShaderStorageBuffer>(std::dynamic_pointer_cast<OpenglBuffer>(buffer), binding_point);
}

std::shared_ptr<Shader> OpenglDevice::create_shader(std::string_view name, std::string_view vertex_src,
                                                    std::string_view fragment_src) {
  return std::make_shared<OpenglShader>(name, vertex_src, fragment_src);
}

std::shared_ptr<Shader> OpenglDevice::create_shader(const std::filesystem::path& path, ShaderConfig config) {
  return std::make_shared<OpenglShader>(path, config);
}

std::shared_ptr<Texture2D> OpenglDevice::create_texture(const TextureSpec& spec) {
  return std::make_shared<OpenglTexture2D>(spec);
}

std::shared_ptr<Texture2D> OpenglDevice::create_texture(const std::filesystem::path& name, const TextureSpec& spec) {
  const std::filesystem::path path = std::filesystem::path{Texture2D::kTexturePath} / name;

  if (const auto it = loaded_textures_.find(path); it != loaded_textures_.end()) {
    return it->second;
  }

  return loaded_textures_[path] = std::make_shared<OpenglTexture2D>(path, spec);
}

std::unique_ptr<VertexArray> OpenglDevice::create_vertex_array() { return std::make_unique<OpenglVertexArray>(); }

std::shared_ptr<Framebuffer> OpenglDevice::create_framebuffer(const FramebufferSpec& spec) {
  return std::make_shared<OpenglFramebuffer>(spec);
}

std::unique_ptr<ImguiBackend> OpenglDevice::create_imgui_backend() { return std::make_unique<OpenglImguiBackend>(); }

}  // namespace kEn
