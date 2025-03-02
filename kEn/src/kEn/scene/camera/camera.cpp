#include <kEn/scene/camera/camera.hpp>
#include <kenpch.hpp>

namespace kEn {

orthographic_camera::orthographic_camera(float left, float right, float bottom, float top)
    : camera(), left_(left), right_(right), bottom_(bottom), top_(top) {
  projection_matrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
  dispatcher_.subscribe<kEn::window_resize_event>(KEN_EVENT_SUBSCRIBER(on_window_resize));
}

void orthographic_camera::set_projection(float left, float right, float bottom, float top) {
  left_              = left;
  right_             = right;
  bottom_            = bottom;
  top_               = top;
  projection_matrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
}

bool orthographic_camera::on_window_resize(window_resize_event& event) {
  float aspect       = (float)event.width() / (float)event.height();
  left_              = -aspect;
  right_             = aspect;
  projection_matrix_ = glm::ortho(left_, right_, bottom_, top_, -1.0f, 1.0f);

  return false;
}

std::shared_ptr<game_component> orthographic_camera::clone() const {
  return std::make_shared<orthographic_camera>(left_, right_, bottom_, top_);
}

perspective_camera::perspective_camera(float fov, float aspect, float zNear, float zFar)
    : camera(), fov_(fov), aspect_(aspect), zNear_(zNear), zFar_(zFar) {
  projection_matrix_ = glm::perspective(fov, aspect, zNear, zFar);
  dispatcher_.subscribe<kEn::window_resize_event>(KEN_EVENT_SUBSCRIBER(on_window_resize));
}

void perspective_camera::set_projection(float fov, float aspect, float zNear, float zFar) {
  fov_               = fov;
  aspect_            = aspect;
  zNear_             = zNear;
  zFar_              = zFar;
  projection_matrix_ = glm::perspective(fov, aspect, zNear, zFar);
}

bool perspective_camera::on_window_resize(window_resize_event& event) {
  aspect_            = (float)event.width() / (float)event.height();
  projection_matrix_ = glm::perspective(fov_, aspect_, zNear_, zFar_);

  return false;
}

std::shared_ptr<game_component> perspective_camera::clone() const {
  return std::make_shared<perspective_camera>(fov_, aspect_, zNear_, zFar_);
}

}  // namespace kEn
