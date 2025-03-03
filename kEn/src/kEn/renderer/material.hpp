#pragma once
#include <glm/vec3.hpp>
#include <kEn/renderer/texture.hpp>
#include <unordered_map>

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

  bool transparent = false;
  bool emissive    = false;

 private:
  std::unordered_map<texture_type_t, std::vector<std::shared_ptr<kEn::Texture2D>>> textures_;
};

}  // namespace kEn
