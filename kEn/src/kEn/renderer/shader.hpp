#pragma once

#include <optional>
#include <span>
#include <string_view>
#include <variant>

#include <mEn.hpp>

#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

class UniformBuffer;

/**
 * @brief Identifies a single programmable stage of the GPU pipeline.
 *
 * Passed to binding calls (e.g. @ref Shader::bind_uniform_block,
 * @ref RenderContext::bind_texture) so that backends can map the request to the
 * correct OpenGL binding point or Vulkan descriptor set.
 */
enum class ShaderStage : std::uint8_t {
  Vertex,         /**< Vertex processing stage. */
  Fragment,       /**< Fragment (pixel) shading stage. */
  Geometry,       /**< Optional geometry stage; requires @ref ShaderConfig::geometry. */
  TessControl,    /**< Tessellation control stage; requires @ref ShaderConfig::tessellation. */
  TessEvaluation, /**< Tessellation evaluation stage; requires @ref ShaderConfig::tessellation. */
  Compute,        /**< Compute stage; requires @ref ShaderConfig::compute. */
};

/**
 * @brief C++20 convenience namespace: imports @ref ShaderStage enumerators without qualification.
 *
 * Include-and-use pattern: @code{.cpp}
 * using namespace kEn::shader_stage;
 * ctx.bind_texture(0, Fragment, *tex);
 * @endcode
 */
namespace shader_stage {

using enum ShaderStage;

}  // namespace shader_stage

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

  /** @brief Return the platform-native GPU program handle. */
  [[nodiscard]] virtual std::uintptr_t native_handle() const noexcept = 0;

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
   * Associates the shader's uniform block with a binding index.  An actual
   * UniformBuffer must also be bound to the same point for data to be visible.
   *
   * @param block_name  Uniform block name as declared in the shader.
   * @param stage       Shader stage hint used by the backend to select the correct binding point.
   * @param binding     Binding point index to use.
   */
  virtual void bind_uniform_block(std::string_view block_name, ShaderStage stage, std::uint32_t binding) const = 0;

  /**
   * @brief Query the binding point currently assigned to a named uniform block.
   *
   * @param block_name  Uniform block name as declared in the shader.
   * @param stage       Shader stage hint used by the backend to select the correct binding point.
   * @return The binding point, or @c std::nullopt if the block does not exist or
   *         has not been bound yet.
   */
  [[nodiscard]] virtual std::optional<std::uint32_t> uniform_block_binding(std::string_view block_name,
                                                                           ShaderStage stage) const = 0;

  // </Uniforms>

  /**
   * @brief Get the human-readable name of this shader.
   *
   * Typically used for logging/debug UI.
   */
  virtual std::string_view name() const = 0;

  /**
   * @brief Base directory for file-based shader loading.
   *
   * Backends may interpret file-based Device::create_shader() paths relative to this directory.
   */
  static constexpr std::string_view kShaderPath = "assets/shaders";
};

}  // namespace kEn
