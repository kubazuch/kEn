#pragma once

#include <filesystem>
#include <memory>
#include <unordered_map>

#include <kEn/renderer/device.hpp>

#include "opengl_command.hpp"
#include "opengl_context.hpp"

namespace kEn {

class OpenglDevice final : public Device {
 public:
  OpenglDevice(GLFWwindow* window, bool enable_debug);

  void swap_buffers() override;
  Command& command() override { return command_; }

  std::shared_ptr<Buffer> create_buffer(const BufferDesc& desc, const void* data) override;
  std::shared_ptr<MutableBuffer> create_mutable_buffer(const BufferDesc& desc, const void* data) override;
  std::shared_ptr<UniformBuffer> create_uniform_buffer(std::shared_ptr<Buffer> buffer) override;
  std::shared_ptr<ShaderStorageBuffer> create_shader_storage_buffer(std::shared_ptr<Buffer> buffer) override;

  std::shared_ptr<Shader> create_shader(std::string_view, std::string_view, std::string_view) override;
  std::shared_ptr<Shader> create_shader(const std::filesystem::path&, ShaderConfig) override;

  std::shared_ptr<Texture> create_texture(const TextureDesc&, const SamplerDesc&) override;
  std::shared_ptr<Texture> create_texture(const std::filesystem::path&, const SamplerDesc&, TextureFormat,
                                          std::uint32_t mip_levels) override;

  std::unique_ptr<VertexInput> create_vertex_input() override;
  std::shared_ptr<Framebuffer> create_framebuffer(const FramebufferSpec&) override;

  std::unique_ptr<ImguiBackend> create_imgui_backend() override;

 private:
  OpenglContext context_;
  OpenglCommand command_;
  std::unordered_map<std::filesystem::path, std::shared_ptr<Texture>> loaded_textures_;
};

}  // namespace kEn
