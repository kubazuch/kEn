#include "opengl_device.hpp"

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string_view>
#include <utility>

#include <kEn/core/assert.hpp>
#include <kEn/core/log.hpp>
#include <kEn/imgui/imgui_backend.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/device.hpp>
#include <kEn/renderer/framebuffer.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/renderer/texture_format.hpp>
#include <kEn/renderer/vertex_input.hpp>

#include "opengl_buffer.hpp"
#include "opengl_framebuffer.hpp"
#include "opengl_imgui_backend.hpp"
#include "opengl_shader.hpp"
#include "opengl_state.hpp"
#include "opengl_texture.hpp"
#include "opengl_vertex_input.hpp"

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

OpenglDevice::OpenglDevice(GLFWwindow* window, bool enable_debug) : swap_chain_(window) {
  swap_chain_.init();
  render_context_.init(enable_debug);
}

void OpenglDevice::swap_buffers() { swap_chain_.swap_buffers(); }

std::shared_ptr<Buffer> OpenglDevice::create_buffer(const BufferDesc& desc, const void* data) {
  return std::make_shared<OpenglBuffer>(desc, data);
}

std::shared_ptr<MutableBuffer> OpenglDevice::create_mutable_buffer(const BufferDesc& desc, const void* data) {
  return std::make_shared<OpenglMutableBuffer>(desc, data);
}

std::shared_ptr<UniformBuffer> OpenglDevice::create_uniform_buffer(std::shared_ptr<Buffer> buffer) {
  return std::make_shared<OpenglUniformBuffer>(std::dynamic_pointer_cast<OpenglBuffer>(std::move(buffer)));
}

std::shared_ptr<ShaderStorageBuffer> OpenglDevice::create_shader_storage_buffer(std::shared_ptr<Buffer> buffer) {
  return std::make_shared<OpenglShaderStorageBuffer>(std::dynamic_pointer_cast<OpenglBuffer>(std::move(buffer)));
}

std::shared_ptr<Shader> OpenglDevice::create_shader(std::string_view name, std::string_view vertex_src,
                                                    std::string_view fragment_src) {
  return std::make_shared<OpenglShader>(name, vertex_src, fragment_src);
}

std::shared_ptr<Shader> OpenglDevice::create_shader(const std::filesystem::path& path, ShaderConfig config) {
  return std::make_shared<OpenglShader>(path, config);
}

std::shared_ptr<Texture> OpenglDevice::create_texture(const TextureDesc& desc, const SamplerDesc& sampler) {
  return std::make_shared<OpenglTexture2D>(desc, sampler);
}

std::shared_ptr<Texture> OpenglDevice::create_texture(const std::filesystem::path& path, const SamplerDesc& sampler,
                                                      TextureFormat format, std::uint32_t mip_levels) {
  if (const auto it = loaded_textures_.find(path); it != loaded_textures_.end()) {
    return it->second;
  }

  return loaded_textures_[path] = std::make_shared<OpenglTexture2D>(path, sampler, format, mip_levels);
}

std::unique_ptr<VertexInput> OpenglDevice::create_vertex_input() { return std::make_unique<OpenglVertexInput>(); }

std::shared_ptr<Framebuffer> OpenglDevice::create_framebuffer(const FramebufferSpec& spec) {
  return std::make_shared<OpenglFramebuffer>(spec);
}

std::shared_ptr<DepthState> OpenglDevice::create_depth_state(const DepthStateDesc& desc) {
  return std::make_shared<OpenglDepthState>(desc);
}

std::shared_ptr<BlendState> OpenglDevice::create_blend_state(const BlendStateDesc& desc) {
  return std::make_shared<OpenglBlendState>(desc);
}

std::shared_ptr<RasterState> OpenglDevice::create_raster_state(const RasterStateDesc& desc) {
  return std::make_shared<OpenglRasterState>(desc);
}

std::unique_ptr<ImguiBackend> OpenglDevice::create_imgui_backend() { return std::make_unique<OpenglImguiBackend>(); }

}  // namespace kEn
