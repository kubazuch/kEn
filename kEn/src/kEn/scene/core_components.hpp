#pragma once
#include <kEn/event/application_events.hpp>
#include <kEn/scene/camera/camera.hpp>
#include <kEn/scene/component.hpp>
#include <kEn/scene/mesh/model.hpp>

namespace kEn {

class ModelComponent : public GameComponent {
 public:
  explicit ModelComponent(const std::shared_ptr<Model>& model) : model_(model) {}

  void update(float) override {}
  void render(Shader& shader) override;
  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override;

 private:
  std::shared_ptr<Model> model_;
};

class FreeLookComponent : public GameComponent {
 public:
  explicit FreeLookComponent(float sensitivity);

  void update(float delta) override;
  void render(Shader&) override {}
  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override;

  float sensitivity() const { return sensitivity_; }
  void set_sensitivity(float sensitivity) { sensitivity_ = sensitivity; }

  bool on_window_resize(const kEn::WindowResizeEvent& event);

 private:
  float sensitivity_;
  float pitch_ = 0, yaw_ = 0;
  bool update_ = false;
  glm::vec2 window_center_;
};

class FreeMoveComponent : public GameComponent {
 public:
  explicit FreeMoveComponent(float speed, bool world_y = true) : speed_(speed), world_y_(world_y) {}

  void update(float delta) override;
  void render(Shader&) override {}
  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override;

  float speed() const { return speed_; }
  void set_speed(float speed) { speed_ = speed; }

 private:
  float speed_;
  bool world_y_;
};

class LookAtComponent : public GameComponent {
 public:
  explicit LookAtComponent(const GameObject& target) : target_(target) {}

  void set_target(const GameObject& target);
  const GameObject& target() const { return target_; }

  void update(float delta) override;
  void render(Shader&) override {}
  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override;

 private:
  std::reference_wrapper<const GameObject> target_;
};

}  // namespace kEn
