#pragma once
#include <glad/gl.h>

#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include <kEn/renderer/shader.hpp>
#include <regex>
#include <unordered_set>

namespace kEn {

class OpenglShader final : public Shader {
 public:
  OpenglShader(std::string name, const std::string& vertex_src, const std::string& fragment_src);
  OpenglShader(const std::filesystem::path& path, ShaderConfig config);
  ~OpenglShader() override;

  void bind() const override;
  void unbind() const override;

  // <Uniforms>
  void set_bool(const std::string& name, bool value) override;

  void set_int(const std::string& name, int value) override;
  void set_int_array(const std::string& name, int* values, size_t count) override;

  void set_uint(const std::string& name, uint32_t value) override;
  void set_uint_array(const std::string& name, uint32_t* values, size_t count) override;

  void set_float(const std::string& name, float value) override;
  void set_float2(const std::string& name, const glm::vec2& value) override;
  void set_float3(const std::string& name, const glm::vec3& value) override;
  void set_float4(const std::string& name, const glm::vec4& value) override;

  void set_mat3(const std::string& name, const glm::mat3& value) override;
  void set_mat4(const std::string& name, const glm::mat4& value) override;
  // </Uniforms>

  const std::string& get_name() override { return name_; }

 private:
  std::string read_shader_src_internal(const std::filesystem::path& filePath,
                                       std::unordered_set<std::filesystem::path>& included, bool internal = false);
  std::string read_shader_src(const std::filesystem::path& file);
  static GLuint create_shader(const std::string& src, GLenum type);
  void create_program(const std::string& vertex_src, const std::string& fragment_src);
  void link_shader() const;

  inline GLint get_uniform_location(const std::string& name) const;

 private:
  static const std::filesystem::path kVertexExt;
  static const std::filesystem::path kFragmentExt;
  static const std::filesystem::path kGeometryExt;
  static const std::filesystem::path kTessControlExt;
  static const std::filesystem::path kTessEvalExt;
  static const std::regex kIncludeRegex;
  static const std::unordered_map<std::string, std::string> kInternalLibs;

  uint32_t renderer_id_;

  mutable std::unordered_map<std::string, GLint> uniform_locations_;

  std::string name_;
};

}  // namespace kEn
