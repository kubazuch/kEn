#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string_view>

#include <kEn/imgui/imgui_backend.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/command.hpp>
#include <kEn/renderer/framebuffer.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/renderer/vertex_input.hpp>

struct GLFWwindow;

namespace kEn {

class Device {
 public:
  enum class Api : std::uint8_t { OpenGL, D3D11 };

  [[nodiscard]] static std::unique_ptr<Device> create(Api api, GLFWwindow* window, bool enable_debug = false);

  virtual ~Device() = default;

  virtual void swap_buffers()              = 0;
  [[nodiscard]] virtual Command& command() = 0;

  [[nodiscard]] virtual std::shared_ptr<Buffer> create_buffer(const BufferDesc& desc, const void* data) = 0;
  [[nodiscard]] std::shared_ptr<Buffer> create_buffer(const BufferDesc& desc) { return create_buffer(desc, nullptr); }

  [[nodiscard]] virtual std::shared_ptr<MutableBuffer> create_mutable_buffer(const BufferDesc& desc,
                                                                             const void* data) = 0;
  [[nodiscard]] std::shared_ptr<MutableBuffer> create_mutable_buffer(const BufferDesc& desc) {
    return create_mutable_buffer(desc, nullptr);
  }

  [[nodiscard]] virtual std::shared_ptr<UniformBuffer> create_uniform_buffer(const std::shared_ptr<Buffer>&,
                                                                             std::size_t slot, ShaderStage stage) = 0;
  [[nodiscard]] std::shared_ptr<UniformBuffer> create_uniform_buffer(const std::shared_ptr<Buffer>& buf,
                                                                     std::size_t slot) {
    return create_uniform_buffer(buf, slot, ShaderStage::Vertex);
  }

  [[nodiscard]] virtual std::shared_ptr<ShaderStorageBuffer> create_shader_storage_buffer(
      const std::shared_ptr<Buffer>&, std::size_t slot, ShaderStage stage) = 0;
  [[nodiscard]] std::shared_ptr<ShaderStorageBuffer> create_shader_storage_buffer(const std::shared_ptr<Buffer>& buf,
                                                                                  std::size_t slot) {
    return create_shader_storage_buffer(buf, slot, ShaderStage::Compute);
  }

  [[nodiscard]] virtual std::shared_ptr<Shader> create_shader(std::string_view name, std::string_view vertex_src,
                                                              std::string_view fragment_src) = 0;
  [[nodiscard]] virtual std::shared_ptr<Shader> create_shader(const std::filesystem::path& path,
                                                              ShaderConfig config)           = 0;
  [[nodiscard]] std::shared_ptr<Shader> create_shader(const std::filesystem::path& path) {
    return create_shader(path, {});
  }

  [[nodiscard]] virtual std::shared_ptr<Texture> create_texture(const TextureDesc& desc,
                                                                const SamplerDesc& sampler) = 0;
  [[nodiscard]] std::shared_ptr<Texture> create_texture(const TextureDesc& desc) { return create_texture(desc, {}); }

  // TODO(remove): transitional path-based loader -- remove when asset pipeline is separate from GPU resources
  [[nodiscard]] virtual std::shared_ptr<Texture> create_texture(const std::filesystem::path& path,
                                                                const SamplerDesc& sampler, TextureFormat format,
                                                                std::uint32_t mip_levels) = 0;
  [[nodiscard]] std::shared_ptr<Texture> create_texture(const std::filesystem::path& path,
                                                        const SamplerDesc& sampler = {},
                                                        TextureFormat format       = TextureFormat::RGBA8) {
    return create_texture(path, sampler, format, kFullMipChain);
  }

  [[nodiscard]] virtual std::unique_ptr<VertexInput> create_vertex_input()                           = 0;
  [[nodiscard]] virtual std::shared_ptr<Framebuffer> create_framebuffer(const FramebufferSpec& spec) = 0;

  [[nodiscard]] virtual std::unique_ptr<ImguiBackend> create_imgui_backend() = 0;
};

}  // namespace kEn
