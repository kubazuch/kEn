#pragma once
#include <glad/gl.h>

#include <filesystem>
#include <functional>
#include <kEn/renderer/shader.hpp>
#include <kEn/util/string_hash.hpp>
#include <mEn.hpp>
#include <regex>
#include <unordered_set>

namespace kEn {

class OpenglShader final : public Shader {
 public:
  OpenglShader(std::string_view name, std::string_view vertex_src, std::string_view fragment_src);
  OpenglShader(const std::filesystem::path& path, ShaderConfig config);
  ~OpenglShader() override;

  void bind() const override;
  void unbind() const override;

  // <Uniforms>
  template <typename T>
  inline void set_uniform(std::string_view name, const T& value) {
    const GLint location = get_uniform_location(name);
    if constexpr (std::is_same_v<T, bool>) {
      glProgramUniform1i(renderer_id_, location, value ? 1 : 0);
    } else if constexpr (std::is_same_v<T, int>) {
      glProgramUniform1i(renderer_id_, location, value);
    } else if constexpr (std::is_same_v<T, uint32_t>) {
      glProgramUniform1ui(renderer_id_, location, value);
    } else if constexpr (std::is_same_v<T, float>) {
      glProgramUniform1f(renderer_id_, location, value);
    } else if constexpr (std::is_same_v<T, mEn::Vec2>) {
      glProgramUniform2f(renderer_id_, location, value.x, value.y);
    } else if constexpr (std::is_same_v<T, mEn::Vec3>) {
      glProgramUniform3f(renderer_id_, location, value.x, value.y, value.z);
    } else if constexpr (std::is_same_v<T, mEn::Vec4>) {
      glProgramUniform4f(renderer_id_, location, value.x, value.y, value.z, value.w);
    } else if constexpr (std::is_same_v<T, mEn::Mat3>) {
      glProgramUniformMatrix3fv(renderer_id_, location, 1, GL_FALSE, mEn::value_ptr(value));
    } else if constexpr (std::is_same_v<T, mEn::Mat4>) {
      glProgramUniformMatrix4fv(renderer_id_, location, 1, GL_FALSE, mEn::value_ptr(value));
    } else {
      static_assert(false, "Unsupported uniform type");
    }
  }

  void set_uniform(std::string_view name, bool value) override { set_uniform<bool>(name, value); }
  void set_uniform(std::string_view name, int value) override { set_uniform<int>(name, value); }
  void set_uniform(std::string_view name, uint32_t value) override { set_uniform<uint32_t>(name, value); }
  void set_uniform(std::string_view name, float value) override { set_uniform<float>(name, value); }
  void set_uniform(std::string_view name, const mEn::Vec2& value) override { set_uniform<mEn::Vec2>(name, value); }
  void set_uniform(std::string_view name, const mEn::Vec3& value) override { set_uniform<mEn::Vec3>(name, value); }
  void set_uniform(std::string_view name, const mEn::Vec4& value) override { set_uniform<mEn::Vec4>(name, value); }
  void set_uniform(std::string_view name, const mEn::Mat3& value) override { set_uniform<mEn::Mat3>(name, value); }
  void set_uniform(std::string_view name, const mEn::Mat4& value) override { set_uniform<mEn::Mat4>(name, value); }

  template <typename T>
  inline void set_uniform_array(std::string_view name, std::span<T> values) const {
    GLint location = get_uniform_location(name);
    if constexpr (std::is_same_v<T, int>) {
      glProgramUniform1iv(renderer_id_, location, static_cast<GLsizei>(values.size()), values.data());
    } else if constexpr (std::is_same_v<T, uint32_t>) {
      glProgramUniform1uiv(renderer_id_, location, static_cast<GLsizei>(values.size()), values.data());
    } else {
      static_assert(false, "Unsupported uniform array type");
    }
  }

  void set_uniform_array(std::string_view name, std::span<int> values) override {
    set_uniform_array<int>(name, values);
  }

  void set_uniform_array(std::string_view name, std::span<uint32_t> values) override {
    set_uniform_array<uint32_t>(name, values);
  }

  void bind_uniform_buffer(std::string_view name, size_t binding) const override;
  void bind_uniform_buffer(std::string_view name, const UniformBuffer& ubo) const override;

  // </Uniforms>

  std::string_view get_name() override { return name_; }

 private:
  std::string read_shader_src_internal(const std::filesystem::path& filePath,
                                       std::unordered_set<std::filesystem::path>& included, bool internal = false);
  std::string read_shader_src(const std::filesystem::path& file);
  static GLuint create_shader(std::string_view src, GLenum type);
  void create_program(std::string_view vertex_src, std::string_view fragment_src);
  void link_shader() const;

  GLint get_uniform_location(std::string_view name) const;

 private:
  static const std::filesystem::path kVertexExt;
  static const std::filesystem::path kFragmentExt;
  static const std::filesystem::path kGeometryExt;
  static const std::filesystem::path kTessControlExt;
  static const std::filesystem::path kTessEvalExt;
  static const std::regex kIncludeRegex;
  static const std::unordered_map<std::string_view, std::string_view> kInternalLibs;

  uint32_t renderer_id_;

  mutable std::unordered_map<std::string, GLint, StringHash, std::equal_to<>> uniform_locations_;
  mutable std::unordered_map<GLuint, GLuint> uniform_block_bindings_;

  std::string name_;
};

}  // namespace kEn
