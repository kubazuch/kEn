#pragma once

#include <glad/gl.h>

#include <filesystem>
#include <functional>
#include <regex>

#include <mEn.hpp>

#include <kEn/core/core.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/util/string_hash.hpp>

namespace kEn {

/**
 * @brief Upload a single uniform value to an OpenGL program at a known location.
 *
 * @tparam T Supported types: bool, int, uint32_t, float, mEn::Vec2/3/4, mEn::Mat3/4.
 * @param program  OpenGL program object.
 * @param location Uniform location (as returned by glGetUniformLocation).
 * @param value    Value to upload.
 *
 * @note Uses glProgramUniform* (Direct State Access) calls.
 * @warning Passing an invalid location (e.g. -1) is a no-op for most drivers but
 *          you should avoid it when possible.
 */
template <typename T>
inline void set_uniform(GLuint program, GLint location, const T& value) {
  if constexpr (std::is_same_v<T, bool>) {
    glProgramUniform1i(program, location, value ? 1 : 0);
  } else if constexpr (std::is_same_v<T, int>) {
    glProgramUniform1i(program, location, value);
  } else if constexpr (std::is_same_v<T, uint32_t>) {
    glProgramUniform1ui(program, location, value);
  } else if constexpr (std::is_same_v<T, float>) {
    glProgramUniform1f(program, location, value);
  } else if constexpr (std::is_same_v<T, mEn::Vec2>) {
    glProgramUniform2f(program, location, value.x, value.y);
  } else if constexpr (std::is_same_v<T, mEn::Vec3>) {
    glProgramUniform3f(program, location, value.x, value.y, value.z);
  } else if constexpr (std::is_same_v<T, mEn::Vec4>) {
    glProgramUniform4f(program, location, value.x, value.y, value.z, value.w);
  } else if constexpr (std::is_same_v<T, mEn::Mat3>) {
    glProgramUniformMatrix3fv(program, location, 1, GL_FALSE, mEn::value_ptr(value));
  } else if constexpr (std::is_same_v<T, mEn::Mat4>) {
    glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, mEn::value_ptr(value));
  } else {
    static_assert(always_false_v<T>, "Unsupported uniform type");
  }
}

/**
 * @brief Upload a 1D scalar uniform array to an OpenGL program at a known location.
 *
 * @tparam T Supported element types: int, uint32_t, float.
 * @param program  OpenGL program object.
 * @param location Uniform location (as returned by glGetUniformLocation).
 * @param values   Elements to upload.
 *
 * @note Uses glProgramUniform1*v (Direct State Access) calls.
 */
template <typename T>
inline void set_uniform_array(GLuint program, GLint location, std::span<const T> values) {
  using U = std::remove_cv_t<T>;
  if constexpr (std::is_same_v<U, int>) {
    glProgramUniform1iv(program, location, static_cast<GLsizei>(values.size()), values.data());
  } else if constexpr (std::is_same_v<U, uint32_t>) {
    glProgramUniform1uiv(program, location, static_cast<GLsizei>(values.size()), values.data());
  } else if constexpr (std::is_same_v<U, float>) {
    glProgramUniform1fv(program, location, static_cast<GLsizei>(values.size()), values.data());
  } else {
    static_assert(always_false_v<T>, "Unsupported uniform array type");
  }
}

/**
 * @brief OpenGL implementation of Shader.
 *
 * Supports:
 * - Vertex/fragment programs created from in-memory sources
 * - File-based loading with optional geometry/tessellation stages
 * - Basic preprocessing for `#include "..."` and `#pragma once`
 * - Uniform location caching and uniform-block binding helpers
 *
 * @note This class is not thread-safe: it mutates internal caches in const methods.
 */
class OpenglShader final : public Shader {
 public:
  /**
   * @brief Construct and compile/link from provided vertex/fragment sources.
   *
   * @param name         Debug/display name for logs.
   * @param vertex_src   Vertex shader GLSL source.
   * @param fragment_src Fragment shader GLSL source.
   *
   * @warning Compilation/link errors may trigger engine assertions.
   */
  OpenglShader(std::string_view name, std::string_view vertex_src, std::string_view fragment_src);

  /**
   * @brief Construct and compile/link by loading sources from disk.
   *
   * The loader typically resolves:
   * - <stem>.vert
   * - <stem>.frag
   * and optionally:
   * - <stem>.geom            if config.geometry
   * - <stem>.tesc/.tese      if config.tessellation
   *
   * Additionally supports basic preprocessing:
   * - `#include "file"` resolved relative to the current file, then Shader::kShaderPath
   * - internal includes resolved from an engine-generated library map
   * - `#pragma once` to avoid duplicate inclusion
   * - automatic `#line` insertion to improve compiler error locations
   *
   * @param path   Path appended to Shader::kShaderPath to locate the shader set.
   * @param config Optional stages/features to enable.
   *
   * @throws std::runtime_error On unrecoverable IO or stage creation failures.
   * @warning Compilation/link errors may trigger engine assertions.
   */
  OpenglShader(const std::filesystem::path& path, ShaderConfig config);

  /**
   * @brief Destroy the OpenGL program object.
   */
  ~OpenglShader() override;

  /// @copydoc Shader::bind
  void bind() const override;

  /// @copydoc Shader::unbind
  void unbind() const override;

  // <Uniforms>

  /// @copydoc Shader::set_uniform_any
  void set_uniform_any(std::string_view name, const UniformValue& value) const override {
    std::visit(
        [&](auto const& x) {
          const GLint location = get_uniform_location(name);
          kEn::set_uniform(renderer_id_, location, x);
        },
        value);
  }

  /// @copydoc Shader::set_uniform_array_any
  void set_uniform_array_any(std::string_view name, const UniformArray& values) const override {
    std::visit(
        [&](auto span) {
          GLint location = get_uniform_location(name);
          kEn::set_uniform_array(renderer_id_, location, span);
        },
        values);
  }

  /// @copydoc Shader::bind_uniform_buffer(std::string_view, size_t)
  void bind_uniform_buffer(std::string_view name, size_t binding) const override;

  /// @copydoc Shader::bind_uniform_buffer(std::string_view, const UniformBuffer&)
  void bind_uniform_buffer(std::string_view name, const UniformBuffer& ubo) const override;

  // </Uniforms>

  /// @copydoc Shader::get_name
  std::string_view get_name() const override { return name_; }

  VIRTUAL_FIVE(OpenglShader);

 private:
  struct PreprocessContext;

  /**
   * @brief Read and preprocess a shader source file, recursively resolving includes.
   *
   * @param filePath  File to read (or key into internal library when @p internal is true).
   * @param ctx       Preprocessing context for include tracking, ids, and #pragma once.
   * @param internal  If true, read from the internal library map instead of disk.
   *
   * @return Fully expanded shader source for @p filePath.
   *
   * @note Prevents circular include recursion.
   * @note Strips accidental `#version` directives from non-root includes.
   */
  std::string read_shader_src_internal(const std::filesystem::path& filePath, PreprocessContext& ctx,
                                       bool internal = false);

  /**
   * @brief Read and preprocess a shader source file (root entry point).
   *
   * @param file               Root file to read from disk.
   * @param out_source_id_map  Optional map of source-id -> display name for log annotation.
   * @return Preprocessed GLSL source string.
   */
  std::string read_shader_src(const std::filesystem::path& file, std::vector<std::string>* out_source_id_map = nullptr);

  /**
   * @brief Compile a single GLSL stage.
   *
   * @param src            GLSL source.
   * @param type           GL shader stage enum (e.g. GL_VERTEX_SHADER).
   * @param debug_name     Name used in error messages/logs.
   * @param source_id_map  Optional mapping used to annotate compiler logs (#line ids).
   * @return Compiled shader object id, or 0 on failure.
   *
   * @warning On compilation failure, logs diagnostics and may assert.
   */
  static GLuint create_shader(std::string_view src, GLenum type, std::string_view debug_name,
                              std::vector<std::string>* source_id_map = nullptr);

  /**
   * @brief Create and link a program from vertex/fragment sources.
   *
   * @param vertex_src   Vertex stage GLSL source.
   * @param fragment_src Fragment stage GLSL source.
   */
  void create_program(std::string_view vertex_src, std::string_view fragment_src);

  /**
   * @brief Link (and in debug builds validate) the currently attached program.
   *
   * @warning On link/validation failure, logs diagnostics and asserts.
   */
  void link_shader() const;

  /**
   * @brief Get (and cache) the uniform location for a given name.
   *
   * @param name Uniform name as declared in the shader.
   * @return Location, or -1 if not found/active.
   *
   * @note Caches lookups in @ref uniform_locations_ to avoid repeated driver queries.
   */
  GLint get_uniform_location(std::string_view name) const;

 private:
  static const std::filesystem::path kVertexExt;
  static const std::filesystem::path kFragmentExt;
  static const std::filesystem::path kGeometryExt;
  static const std::filesystem::path kTessControlExt;
  static const std::filesystem::path kTessEvalExt;
  static const std::regex kIncludeRegex;
  static const std::regex kPragmaOnceRegex;
  static const std::unordered_map<std::string_view, std::string_view> kInternalLibs;

  /** @brief OpenGL program object id. */
  uint32_t renderer_id_;

  /** @brief Cached uniform locations (mutated on demand). */
  mutable std::unordered_map<std::string, GLint, StringHash, std::equal_to<>> uniform_locations_;

  /** @brief Cached uniform block index -> binding mapping (mutated on demand). */
  mutable std::unordered_map<GLuint, GLuint> uniform_block_bindings_;

  /** @brief Debug/display name for this shader program. */
  std::string name_;
};

}  // namespace kEn
