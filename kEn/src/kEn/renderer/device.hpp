#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string_view>

#include <kEn/imgui/imgui_backend.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/framebuffer.hpp>
#include <kEn/renderer/render_context.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture.hpp>
#include <kEn/renderer/vertex_input.hpp>

/** @file
 *  @ingroup ken
 */

struct GLFWwindow;

namespace kEn {

/**
 * @brief Abstract GPU device: resource factory and render context provider.
 *
 * Provides the primary entry point for creating all GPU resources (buffers,
 * shaders, textures, framebuffers, vertex inputs, and the ImGui backend) and
 * for accessing the active @ref RenderContext.  Obtain the active device via
 * @c Application::instance().device() or the free function @c kEn::device().
 *
 * Concrete implementations are selected at runtime via @ref create().
 * Currently only @c Api::OpenGL is supported.
 */
class Device {
 public:
  /** @brief Selects the graphics API for the concrete Device implementation. */
  enum class Api : std::uint8_t { OpenGL, D3D11 };

  /**
   * @brief Creates and returns the concrete Device for the given API.
   * @param api           Graphics API to instantiate.
   * @param window        Native window handle used to create the graphics context.
   * @param enable_debug  Request a debug context with validation messages.
   */
  [[nodiscard]] static std::unique_ptr<Device> create(Api api, GLFWwindow* window, bool enable_debug = false);

  virtual ~Device() = default;

  /** @brief Present the back buffer to the screen (swap chain flip). */
  virtual void swap_buffers() = 0;

  /** @brief Return the active @ref RenderContext for this device. */
  [[nodiscard]] virtual RenderContext& context() = 0;

  /**
   * @brief Allocate an immutable or default-usage GPU buffer.
   * @param desc  Size, usage, and bind flags.
   * @param data  Optional initial data pointer; pass `nullptr` to leave uninitialized.
   */
  [[nodiscard]] virtual std::shared_ptr<Buffer> create_buffer(const BufferDesc& desc, const void* data) = 0;

  /** @brief Convenience overload that allocates with no initial data. */
  [[nodiscard]] std::shared_ptr<Buffer> create_buffer(const BufferDesc& desc) { return create_buffer(desc, nullptr); }

  /**
   * @brief Allocate a CPU-writable (dynamic or staging) GPU buffer.
   * @param desc  Size, usage, and bind flags.
   * @param data  Optional initial data pointer; pass `nullptr` to leave uninitialized.
   */
  [[nodiscard]] virtual std::shared_ptr<MutableBuffer> create_mutable_buffer(const BufferDesc& desc,
                                                                             const void* data) = 0;

  /** @brief Convenience overload that allocates with no initial data. */
  [[nodiscard]] std::shared_ptr<MutableBuffer> create_mutable_buffer(const BufferDesc& desc) {
    return create_mutable_buffer(desc, nullptr);
  }

  /**
   * @brief Wrap an existing @ref Buffer as a uniform buffer binding view.
   * @param buffer  Buffer created with the @c BufferBind::Uniform flag.
   */
  [[nodiscard]] virtual std::shared_ptr<UniformBuffer> create_uniform_buffer(std::shared_ptr<Buffer> buffer) = 0;

  /**
   * @brief Wrap an existing @ref Buffer as a shader storage buffer binding view.
   * @param buffer  Buffer created with the @c BufferBind::Storage flag.
   */
  [[nodiscard]] virtual std::shared_ptr<ShaderStorageBuffer> create_shader_storage_buffer(
      std::shared_ptr<Buffer> buffer) = 0;

  /**
   * @brief Compile and link a shader from in-memory GLSL vertex/fragment source.
   * @param name          Debug name used in logs.
   * @param vertex_src    Vertex shader GLSL source.
   * @param fragment_src  Fragment shader GLSL source.
   */
  [[nodiscard]] virtual std::shared_ptr<Shader> create_shader(std::string_view name, std::string_view vertex_src,
                                                              std::string_view fragment_src) = 0;

  /**
   * @brief Load, preprocess, and link a shader set from disk.
   * @param path    Stem path appended to @c Shader::kShaderPath to locate the files.
   * @param config  Optional extra stages (geometry, tessellation) to enable.
   */
  [[nodiscard]] virtual std::shared_ptr<Shader> create_shader(const std::filesystem::path& path,
                                                              ShaderConfig config) = 0;

  /** @brief Convenience overload with default @ref ShaderConfig (vertex + fragment only). */
  [[nodiscard]] std::shared_ptr<Shader> create_shader(const std::filesystem::path& path) {
    return create_shader(path, {});
  }

  /**
   * @brief Allocate a GPU texture from a descriptor.
   * @param desc     Texture dimensions, format, mip levels, and kind.
   * @param sampler  Sampling parameters applied to the texture.
   */
  [[nodiscard]] virtual std::shared_ptr<Texture> create_texture(const TextureDesc& desc,
                                                                const SamplerDesc& sampler) = 0;

  /** @brief Convenience overload with default sampler parameters. */
  [[nodiscard]] std::shared_ptr<Texture> create_texture(const TextureDesc& desc) { return create_texture(desc, {}); }

  // TODO(remove): transitional path-based loader -- remove when asset pipeline is separate from GPU resources

  /**
   * @brief Load a texture from an image file, returning a cached shared instance.
   * @param path       File path to load.
   * @param sampler    Sampling parameters.
   * @param format     Desired pixel format (default @c TextureFormat::RGBA8).
   * @param mip_levels Number of mip levels to generate; pass @c kFullMipChain for a full chain.
   * @note Repeated calls with the same @p path return the same cached object.
   */
  [[nodiscard]] virtual std::shared_ptr<Texture> create_texture(const std::filesystem::path& path,
                                                                const SamplerDesc& sampler, TextureFormat format,
                                                                std::uint32_t mip_levels) = 0;

  /** @brief Convenience overload with default sampler, RGBA8 format, and a full mip chain. */
  [[nodiscard]] std::shared_ptr<Texture> create_texture(const std::filesystem::path& path,
                                                        const SamplerDesc& sampler = {},
                                                        TextureFormat format       = TextureFormat::RGBA8) {
    return create_texture(path, sampler, format, kFullMipChain);
  }

  /** @brief Create a new @ref VertexInput (VAO abstraction) for this device. */
  [[nodiscard]] virtual std::unique_ptr<VertexInput> create_vertex_input() = 0;

  /**
   * @brief Allocate an off-screen render target matching @p spec.
   * @param spec  Dimensions, sample count, and attachment descriptions.
   */
  [[nodiscard]] virtual std::shared_ptr<Framebuffer> create_framebuffer(const FramebufferSpec& spec) = 0;

  /** @brief Create the renderer-specific Dear ImGui backend for this device. */
  [[nodiscard]] virtual std::unique_ptr<ImguiBackend> create_imgui_backend() = 0;
};

}  // namespace kEn
