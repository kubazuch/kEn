#pragma once
#include <glm/vec3.hpp>
#include <kEn/renderer/texture.hpp>
#include <map>

namespace kEn {

class shader;

class material {
 public:
  material() = default;

  void set_texture(texture_type_t type, const std::shared_ptr<texture2D>& texture, int id = 0);
  const std::shared_ptr<texture2D>& texture(texture_type_t type, int id = 0);

  void load(shader& shader, const std::string& name = "u_Material") const;
  void bind() const;

  void imgui();

 private:
  friend class shader;

 public:
  float ambient_factor   = 0.5f;
  float diffuse_factor   = 0.5f;
  float specular_factor  = 0.5f;
  float shininess_factor = 50.f;

  bool transparent = false;
  bool emissive    = false;

 private:
  std::unordered_map<texture_type_t, std::vector<std::shared_ptr<kEn::texture2D>>> textures_;
};

}  // namespace kEn
