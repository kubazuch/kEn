#include <kEn/scene/camera/camera.hpp>
#include <kenpch.hpp>

namespace kEn {

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : Camera(), left_(left), right_(right), bottom_(bottom), top_(top) {
  projection_matrix_ = mEn::ortho(left, right, bottom, top, -1.0F, 1.0F);
  dispatcher_.subscribe<kEn::WindowResizeEvent>(KEN_EVENT_SUBSCRIBER(on_window_resize));
}

void OrthographicCamera::set_projection(float left, float right, float bottom, float top) {
  left_              = left;
  right_             = right;
  bottom_            = bottom;
  top_               = top;
  projection_matrix_ = mEn::ortho(left, right, bottom, top, -1.0F, 1.0F);
}

bool OrthographicCamera::on_window_resize(WindowResizeEvent& event) {
  float aspect       = static_cast<float>(event.width()) / static_cast<float>(event.height());
  left_              = -aspect;
  right_             = aspect;
  projection_matrix_ = mEn::ortho(left_, right_, bottom_, top_, -1.0F, 1.0F);

  return false;
}

std::shared_ptr<GameComponent> OrthographicCamera::clone() const {
  return std::make_shared<OrthographicCamera>(left_, right_, bottom_, top_);
}

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float zNear, float zFar)
    : Camera(), fov_(fov), aspect_(aspect), zNear_(zNear), zFar_(zFar) {
  projection_matrix_ = mEn::perspective(fov, aspect, zNear, zFar);
  dispatcher_.subscribe<kEn::WindowResizeEvent>(KEN_EVENT_SUBSCRIBER(on_window_resize));
}

void PerspectiveCamera::set_projection(float fov, float aspect, float zNear, float zFar) {
  fov_               = fov;
  aspect_            = aspect;
  zNear_             = zNear;
  zFar_              = zFar;
  projection_matrix_ = mEn::perspective(fov, aspect, zNear, zFar);
}

bool PerspectiveCamera::on_window_resize(WindowResizeEvent& event) {
  aspect_            = static_cast<float>(event.width()) / static_cast<float>(event.height());
  projection_matrix_ = mEn::perspective(fov_, aspect_, zNear_, zFar_);

  return false;
}

std::shared_ptr<GameComponent> PerspectiveCamera::clone() const {
  return std::make_shared<PerspectiveCamera>(fov_, aspect_, zNear_, zFar_);
}

}  // namespace kEn
