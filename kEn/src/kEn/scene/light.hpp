#pragma once

#include <mEn/vec3.hpp>

#include <kEn/core/core.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/renderer/framebuffer.hpp>
#include <kEn/scene/component.hpp>

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
  BaseLight()          = default;
  virtual ~BaseLight() = default;

  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override = 0;
  void update(duration_t, duration_t) override {}
  void render(Shader&, double) override {}
  void on_transform_changed() override {}

  VIRTUAL_FIVE(BaseLight);

  virtual void load(const std::string& name, Shader& shader) const = 0;

 public:
  mEn::Vec3 color{1.F};
};

class DirectionalLight : public BaseLight {
 public:
  void imgui() override;

  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override;

  void load(const std::string& name, Shader& shader) const override;
};

class PointLight : public BaseLight {
 public:
  void imgui() override;

  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override;

  void load(const std::string& name, Shader& shader) const override;

 public:
  Attenuation atten;
};

class SpotLight : public BaseLight {
 public:
  SpotLight();

  void imgui() override;

  void set_angles(float inner, float outer);

  Framebuffer& framebuffer() const { return *sss_framebuffer_; };
  mEn::Mat4 view_matrix() const { return transform().world_to_local_matrix(); }
  mEn::Mat4 proj_matrix() const { return proj_matrix_; }
  void bind_sss_texture() const;

  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override;

  void load(const std::string& name, Shader& shader) const override;

 public:
  Attenuation atten;
  float inner_cutoff_angle = 0.1F;
  float outer_cutoff_angle = 0.1F;

 private:
  int light_id_;
  std::shared_ptr<Framebuffer> sss_framebuffer_;
  mEn::Mat4 proj_matrix_;
};

}  // namespace kEn
