#pragma once

#include <span>
#include <string_view>

#include <mEn.hpp>

#include <kEn/renderer/buffer.hpp>
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
  virtual void set_uniform(std::string_view name, bool value)             = 0;
  virtual void set_uniform(std::string_view name, int value)              = 0;
  virtual void set_uniform(std::string_view name, uint32_t value)         = 0;
  virtual void set_uniform(std::string_view name, float value)            = 0;
  virtual void set_uniform(std::string_view name, const mEn::Vec2& value) = 0;
  virtual void set_uniform(std::string_view name, const mEn::Vec3& value) = 0;
  virtual void set_uniform(std::string_view name, const mEn::Vec4& value) = 0;
  virtual void set_uniform(std::string_view name, const mEn::Mat3& value) = 0;
  virtual void set_uniform(std::string_view name, const mEn::Mat4& value) = 0;

  virtual void set_uniform_array(std::string_view name, std::span<int> values)      = 0;
  virtual void set_uniform_array(std::string_view name, std::span<uint32_t> values) = 0;

  virtual void bind_uniform_buffer(std::string_view name, size_t binding) const           = 0;
  virtual void bind_uniform_buffer(std::string_view name, const UniformBuffer& ubo) const = 0;
  // </Uniforms>

  virtual std::string_view get_name() = 0;

  static const std::filesystem::path kShaderPath;
  static std::shared_ptr<Shader> create(std::string_view name, std::string_view vertex_src,
                                        std::string_view fragment_src);
  static std::shared_ptr<Shader> create(const std::filesystem::path& path, ShaderConfig config = ShaderConfig());
};

}  // namespace kEn
