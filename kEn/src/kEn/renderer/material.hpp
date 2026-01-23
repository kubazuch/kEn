#pragma once

#include <unordered_map>

#include <mEn.hpp>

#include <kEn/renderer/texture.hpp>

namespace kEn {

class Shader;

class Material {
 public:
  Material() = default;

  void set_texture(texture_type_t type, const std::shared_ptr<Texture2D>& texture, size_t id = 0);
  const std::shared_ptr<Texture2D>& texture(texture_type_t type, size_t id = 0);

  void load(Shader& shader, const std::string& name = "u_Material") const;
  void bind() const;

  void imgui();

 private:
  friend class shader;

 public:
  float ambient_factor   = 0.5F;
  float diffuse_factor   = 0.5F;
  float specular_factor  = 0.5F;
  float shininess_factor = 50.F;

  mEn::Vec3 surface_color = mEn::Vec3(1.F, 1.F, 1.F);

  float sigma         = 1.F;
  mEn::Vec3 sss_color = mEn::Vec3(1.F, 0.F, 0.F);
  float sss_strength  = 1.0;

  bool transparent = false;
  bool emissive    = false;

 private:
  std::unordered_map<texture_type_t, std::vector<std::shared_ptr<kEn::Texture2D>>> textures_;
};

}  // namespace kEn
