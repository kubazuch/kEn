#include "camera.hpp"

#include <memory>

#include <mEn/functions/matrix_projection.hpp>

#include <kEn/event/application_events.hpp>
#include <kEn/event/event.hpp>
#include <kEn/scene/component.hpp>

namespace kEn {

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : Camera(mEn::ortho(left, right, bottom, top, -1.0F, 1.0F)),
      left_(left),
      right_(right),
      bottom_(bottom),
      top_(top) {
  dispatcher_.subscribe(this, &OrthographicCamera::on_window_resize);
}

void OrthographicCamera::set_projection(float left, float right, float bottom, float top) {
  left_   = left;
  right_  = right;
  bottom_ = bottom;
  top_    = top;
  Camera::set_projection(mEn::ortho(left, right, bottom, top, -1.0F, 1.0F));
}

bool OrthographicCamera::on_window_resize(const WindowResizeEvent& event) {
  const float aspect = static_cast<float>(event.width()) / static_cast<float>(event.height());
  left_              = -aspect;
  right_             = aspect;
  Camera::set_projection(mEn::ortho(left_, right_, bottom_, top_, -1.0F, 1.0F));
  return false;
}

std::unique_ptr<GameComponent> OrthographicCamera::clone() const {
  return std::make_unique<OrthographicCamera>(left_, right_, bottom_, top_);
}

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float z_near, float z_far)
    : Camera(mEn::perspective(fov, aspect, z_near, z_far)), fov_(fov), aspect_(aspect), z_near_(z_near), z_far_(z_far) {
  dispatcher_.subscribe(this, &PerspectiveCamera::on_window_resize);
}

void PerspectiveCamera::set_projection(float fov, float aspect, float z_near, float z_far) {
  fov_    = fov;
  aspect_ = aspect;
  z_near_ = z_near;
  z_far_  = z_far;
  Camera::set_projection(mEn::perspective(fov, aspect, z_near, z_far));
}

bool PerspectiveCamera::on_window_resize(const WindowResizeEvent& event) {
  aspect_ = static_cast<float>(event.width()) / static_cast<float>(event.height());
  Camera::set_projection(mEn::perspective(fov_, aspect_, z_near_, z_far_));
  return false;
}

std::unique_ptr<GameComponent> PerspectiveCamera::clone() const {
  return std::make_unique<PerspectiveCamera>(fov_, aspect_, z_near_, z_far_);
}

}  // namespace kEn
