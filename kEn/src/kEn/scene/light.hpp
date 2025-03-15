#pragma once
#include <kEn/core/core.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/scene/component.hpp>
#include <mEn/vec3.hpp>

namespace kEn {

class Shader;

struct Attenuation {
  float constant  = 1.0F;
  float linear    = 0.0F;
  float quadratic = 0.0F;

  inline void load(const std::string& name, Shader& shader) const;
};

class BaseLight : public GameComponent {
 public:
  BaseLight()                         = default;
  virtual ~BaseLight()                = default;
  virtual void imgui(bool subsection) = 0;

  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override = 0;
  void update(float) override {}
  void render(Shader&) override {}

  VIRTUAL_FIVE(BaseLight);

  virtual void load(const std::string& name, Shader& shader) const = 0;

 public:
  mEn::Vec3 color{1.F};
};

class DirectionalLight : public BaseLight {
 public:
  void imgui(bool subsection) override;

  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override;

  void load(const std::string& name, Shader& shader) const override;
};

class PointLight : public BaseLight {
 public:
  void imgui(bool subsection) override;

  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override;

  void load(const std::string& name, Shader& shader) const override;

 public:
  Attenuation atten;
};

class SpotLight : public BaseLight {
 public:
  void imgui(bool subsection) override;

  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override;

  void load(const std::string& name, Shader& shader) const override;

 public:
  Attenuation atten;
  float inner_cutoff_angle = 0.1F;
  float outer_cutoff_angle = 0.1F;
};

}  // namespace kEn
