#pragma once

#include <kEn/core/transform.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/scene/component.hpp>
#include <mEn.hpp>

namespace kEn {

class Camera : public GameComponent {
 public:
  virtual ~Camera() = default;

  mEn::Mat4 projection_matrix() const { return projection_matrix_; }
  mEn::Mat4 view_matrix() const { return transform().world_to_local_matrix(); }
  mEn::Mat4 view_projection_matrix() const { return projection_matrix_ * view_matrix(); }

  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override = 0;
  void update(duration_t, duration_t) override {}
  void render(Shader&, double) override {}
  void imgui() override {}
  void on_transform_changed() override {}

  virtual bool on_window_resize(WindowResizeEvent& event) = 0;

 protected:
  mEn::Mat4 projection_matrix_;
};

class OrthographicCamera : public Camera {
 public:
  OrthographicCamera(float left, float right, float bottom, float top);
  void set_projection(float left, float right, float bottom, float top);

  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override;

  bool on_window_resize(WindowResizeEvent& event) override;

 private:
  float left_, right_, bottom_, top_;
};

class PerspectiveCamera : public Camera {
 public:
  PerspectiveCamera(float fov, float aspect, float zNear, float zFar);
  void set_projection(float fov, float aspect, float zNear, float zFar);

  [[nodiscard]] std::shared_ptr<GameComponent> clone() const override;

  bool on_window_resize(WindowResizeEvent& event) override;

 private:
  float fov_, aspect_, zNear_, zFar_;
};

}  // namespace kEn
