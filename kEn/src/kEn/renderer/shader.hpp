#pragma once

#include <span>
#include <string_view>
#include <variant>

#include <mEn.hpp>

#include <kEn/renderer/buffer.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief Variant of supported uniform value types.
 *
 * Backends are expected to support setting uniforms for all contained types.
 */
using UniformValue = std::variant<bool, int, uint32_t, float, mEn::Vec2, mEn::Vec3, mEn::Vec4, mEn::Mat3, mEn::Mat4>;

/**
 * @brief Variant of supported uniform array types.
 *
 * Arrays are intended for 1D scalar uniform arrays (e.g. int[], uint[], float[]).
 */
using UniformArray = std::variant<std::span<const int>, std::span<const uint32_t>, std::span<const float>>;

/**
 * @brief Optional shader stages and features to enable when loading from files.
 */
struct ShaderConfig {
  /** @brief Enable a geometry stage (.geom) in addition to vertex/fragment. */
  bool geometry = false;

  /** @brief Enable tessellation stages (.tesc/.tese) in addition to vertex/fragment. */
  bool tessellation = false;

  /** @brief Enable compute shader loading (backend-dependent). Currently not implemented. */
  bool compute = false;
};

/**
 * @brief Abstract GPU shader program interface.
 *
 * A Shader represents a linked GPU program. Implementations are responsible for
 * compiling, linking, binding, and updating uniforms/uniform blocks.
 */
class Shader {
 public:
  /**
   * @brief Virtual destructor.
   *
   * Allows deleting backend implementations via a Shader pointer.
   */
  virtual ~Shader() = default;

  /**
   * @brief Bind this shader program for subsequent rendering/dispatch.
   *
   * After binding, draw/dispatch calls will use this program until another
   * shader is bound or unbind() is called.
   */
  virtual void bind() const = 0;

  /**
   * @brief Unbind the current shader program.
   *
   * Typically restores the API's "no program" state (backend-dependent).
   */
  virtual void unbind() const = 0;

  // <Uniforms>

  /**
   * @brief Set a uniform by name using a type-erased value.
   *
   * @param name  Uniform name as declared in the shader.
   * @param value Value to upload; must be one of UniformValue's alternatives.
   *
   * @note Backends may log a warning if the uniform is not found/active.
   * @note Not guaranteed to be thread-safe; intended for the render thread.
   */
  virtual void set_uniform_any(std::string_view name, const UniformValue& value) const = 0;

  /**
   * @brief Set a 1D scalar uniform array by name using a type-erased span.
   *
   * @param name   Uniform name as declared in the shader.
   * @param values Scalar values to upload; must be one of UniformArray's alternatives.
   *
   * @note Backends may log a warning if the uniform is not found/active.
   */
  virtual void set_uniform_array_any(std::string_view name, const UniformArray& values) const = 0;

  /**
   * @brief Convenience typed wrapper around set_uniform_any().
   *
   * @tparam T One of the types supported by UniformValue.
   * @param name  Uniform name as declared in the shader.
   * @param value Value to upload.
   */
  template <class T>
  void set_uniform(std::string_view name, const T& value) const {
    set_uniform_any(name, UniformValue{value});
  }

  /**
   * @brief Convenience typed wrapper around set_uniform_array_any().
   *
   * @tparam T One of the scalar element types supported by UniformArray.
   * @param name   Uniform name as declared in the shader.
   * @param values Scalar values to upload.
   */
  template <class T>
  void set_uniform_array(std::string_view name, std::span<T> values) const {
    set_uniform_array_any(name, UniformArray{values});
  }

  /**
   * @brief Bind a named uniform block to a binding point.
   *
   * This associates a shader's uniform block with a binding index. You must also
   * bind an actual UniformBuffer to the same binding point for data to be visible.
   *
   * @param name    Uniform block name as declared in the shader.
   * @param binding Binding point index to use.
   */
  virtual void bind_uniform_buffer(std::string_view name, size_t binding) const = 0;

  /**
   * @brief Bind a named uniform block to the binding point of an existing UBO.
   *
   * @param name Uniform block name as declared in the shader.
   * @param ubo  Uniform buffer whose binding point will be used.
   *
   * @note Backends may validate block size/layout against the provided UBO.
   */
  virtual void bind_uniform_buffer(std::string_view name, const UniformBuffer& ubo) const = 0;

  // </Uniforms>

  /**
   * @brief Get the human-readable name of this shader.
   *
   * Typically used for logging/debug UI.
   */
  virtual std::string_view get_name() const = 0;

  /**
   * @brief Base directory for file-based shader loading.
   *
   * Backends may interpret file-based create() paths relative to this directory.
   */
  static const std::filesystem::path kShaderPath;

  /**
   * @brief Create a shader from in-memory GLSL sources.
   *
   * @param name         Debug/display name for the program.
   * @param vertex_src   Vertex shader source code.
   * @param fragment_src Fragment shader source code.
   *
   * @return Shared pointer to the created shader, or nullptr on unsupported API.
   *
   * @note The concrete backend is selected by RendererApi::get_api().
   * @warning May assert/abort on compilation or link errors (backend-dependent).
   */
  static std::shared_ptr<Shader> create(std::string_view name, std::string_view vertex_src,
                                        std::string_view fragment_src);

  /**
   * @brief Create a shader by loading sources from disk.
   *
   * The backend typically loads <stem>.vert and <stem>.frag and optionally other
   * stages based on @p config.
   *
   * @param path   Path (often relative to kShaderPath) identifying the shader set.
   * @param config Optional stages/features to enable.
   *
   * @return Shared pointer to the created shader, or nullptr on unsupported API.
   *
   * @note The concrete backend is selected by RendererApi::get_api().
   * @warning May assert/abort or throw on IO/compilation/link errors (backend-dependent).
   */
  static std::shared_ptr<Shader> create(const std::filesystem::path& path, ShaderConfig config = ShaderConfig());
};

}  // namespace kEn
