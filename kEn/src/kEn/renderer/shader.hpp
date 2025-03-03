#pragma once
#include <glm/glm.hpp>
#include <kEn/renderer/material.hpp>
#include <kEn/scene/light.hpp>

namespace kEn {

struct ShaderConfig {
  bool geometry     = false;
  bool tessellation = false;
  bool compute      = false;
};

class Shader {
 public:
  virtual ~Shader() = default;

  virtual void bind() const   = 0;
  virtual void unbind() const = 0;

  // <Uniforms>
  virtual void set_bool(const std::string& name, bool value) = 0;

  virtual void set_int(const std::string& name, int value)                       = 0;
  virtual void set_int_array(const std::string& name, int* values, size_t count) = 0;

  virtual void set_uint(const std::string& name, uint32_t value)                       = 0;
  virtual void set_uint_array(const std::string& name, uint32_t* values, size_t count) = 0;

  virtual void set_float(const std::string& name, float value)             = 0;
  virtual void set_float2(const std::string& name, const glm::vec2& value) = 0;
  virtual void set_float3(const std::string& name, const glm::vec3& value) = 0;
  virtual void set_float4(const std::string& name, const glm::vec4& value) = 0;

  virtual void set_mat3(const std::string& name, const glm::mat3& value) = 0;
  virtual void set_mat4(const std::string& name, const glm::mat4& value) = 0;

  virtual void set_material(const std::string& name, const Material& value) { value.load(*this, name); }
  virtual void set_light(const std::string& name, const BaseLight& value) { value.load(name, *this); }
  // </Uniforms>

  virtual const std::string& get_name() = 0;

  static const std::filesystem::path kShaderPath;
  static std::shared_ptr<Shader> create(const std::string& name, const std::string& vertex_src,
                                        const std::string& fragment_src);
  static std::shared_ptr<Shader> create(const std::filesystem::path& path, ShaderConfig config = ShaderConfig());
};

}  // namespace kEn
