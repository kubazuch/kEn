#pragma once
#include <glm/vec3.hpp>
#include <kEn/core/core.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/scene/component.hpp>

namespace kEn {

class shader;

struct attenuation {
  float constant  = 1.0f;
  float linear    = 0.0f;
  float quadratic = 0.0f;

  inline void load(const std::string& name, shader& shader) const;
};

class base_light : public game_component {
 public:
  base_light()                        = default;
  virtual ~base_light()               = default;
  virtual void imgui(bool subsection) = 0;

  [[nodiscard]] std::shared_ptr<game_component> clone() const = 0;
  void update(float delta) override {}
  void render(shader& shader) override {}

  VIRTUAL_FIVE(base_light);

  virtual void load(const std::string& name, shader& shader) const = 0;

 public:
  glm::vec3 color = glm::vec3{1.f};
};

class directional_light : public base_light {
 public:
  void imgui(bool subsection = true) override;

  [[nodiscard]] std::shared_ptr<game_component> clone() const override;

  void load(const std::string& name, shader& shader) const override;
};

class point_light : public base_light {
 public:
  void imgui(bool subsection = true) override;

  [[nodiscard]] std::shared_ptr<game_component> clone() const override;

  void load(const std::string& name, shader& shader) const override;

 public:
  attenuation atten;
};

class spot_light : public base_light {
 public:
  void imgui(bool subsection = true) override;

  [[nodiscard]] std::shared_ptr<game_component> clone() const override;

  void load(const std::string& name, shader& shader) const override;

 public:
  attenuation atten;
  float inner_cutoff_angle = 0.1f;
  float outer_cutoff_angle = 0.1f;
};

}  // namespace kEn
