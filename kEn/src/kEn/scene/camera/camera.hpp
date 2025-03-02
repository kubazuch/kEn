#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/scene/component.hpp>

namespace kEn {

class camera : public game_component {
 public:
  virtual ~camera() = default;

  glm::mat4 projection_matrix() const { return projection_matrix_; }
  glm::mat4 view_matrix() const { return transform().world_to_local_matrix(); }
  glm::mat4 view_projection_matrix() const { return projection_matrix_ * view_matrix(); }

  [[nodiscard]] std::shared_ptr<game_component> clone() const override = 0;
  void update(float delta) override {}
  void render(shader& shader) override {}

  virtual bool on_window_resize(window_resize_event& event) = 0;

 protected:
  glm::mat4 projection_matrix_;
};

class orthographic_camera : public camera {
 public:
  orthographic_camera(float left, float right, float bottom, float top);
  void set_projection(float left, float right, float bottom, float top);

  [[nodiscard]] std::shared_ptr<game_component> clone() const override;

  bool on_window_resize(window_resize_event& event) override;

 private:
  float left_, right_, bottom_, top_;
};

class perspective_camera : public camera {
 public:
  perspective_camera(float fov, float aspect, float zNear, float zFar);
  void set_projection(float fov, float aspect, float zNear, float zFar);

  [[nodiscard]] std::shared_ptr<game_component> clone() const override;

  bool on_window_resize(window_resize_event& event) override;

 private:
  float fov_, aspect_, zNear_, zFar_;
};

}  // namespace kEn
