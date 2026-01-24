#include "core_components.hpp"

#include <chrono>
#include <memory>

#include <mEn/functions.hpp>
#include <mEn/quat.hpp>

#include <kEn/core/application.hpp>
#include <kEn/core/input.hpp>
#include <kEn/core/key_codes.hpp>
#include <kEn/core/window.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/event/event.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/scene/component.hpp>
#include <kEn/scene/game_object.hpp>

namespace kEn {

void ModelComponent::render(Shader& shader, double /*alpha*/) {
  if (!parent_.has_value()) {
    return;
  }
  model_->render(shader, transform());
}

std::shared_ptr<GameComponent> ModelComponent::clone() const { return std::make_shared<ModelComponent>(model_); }

FreeLookComponent::FreeLookComponent(float sensitivity) : sensitivity_(sensitivity), window_center_() {
  dispatcher_.subscribe<WindowResizeEvent>(KEN_EVENT_SUBSCRIBER(on_window_resize));
  const Window& main = kEn::Application::instance().main_window();
  window_center_     = {main.width() / 2, main.height() / 2};
}

void FreeLookComponent::update(duration_t /*delta*/, duration_t /*time*/) {
  if (kEn::Input::is_key_pressed(kEn::key::escape)) {
    kEn::Input::set_cursor_visible(true);
    update_ = false;
  }

  if (kEn::Input::is_key_pressed(kEn::key::f)) {
    kEn::Input::set_mouse_pos(window_center_);
    kEn::Input::set_cursor_visible(false);
    update_ = true;
  }

  if (!update_) {
    return;
  }

  auto delta_pos   = kEn::Input::get_mouse_pos() - window_center_;
  const bool rot_y = delta_pos.x != 0;
  const bool rot_x = delta_pos.y != 0;

  if (rot_y) {
    yaw_ -= mEn::radians(delta_pos.x) * sensitivity_;
  }

  if (rot_x) {
    pitch_ -= mEn::radians(delta_pos.y) * sensitivity_;
    pitch_ = mEn::clamp(pitch_, (-mEn::pi<float>() / 2.F) + 0.01F, (mEn::pi<float>() / 2.F) - 0.01F);
  }

  if (rot_x || rot_y) {
    kEn::Input::set_mouse_pos(window_center_);

    mEn::Quat rot(1, 0, 0, 0);
    rot = mEn::rotate(rot, yaw_, mEn::Vec3(0.0F, 1.0F, 0.0F));
    rot = mEn::rotate(rot, pitch_, mEn::Vec3(1.0F, 0.0F, 0.0F));
    transform().set_local_rot(rot);
  }
}

std::shared_ptr<GameComponent> FreeLookComponent::clone() const {
  return std::make_shared<FreeLookComponent>(sensitivity_);
}

bool FreeLookComponent::on_window_resize(const kEn::WindowResizeEvent& event) {
  window_center_ = mEn::Vec2(event.width() / 2, event.height() / 2);
  return false;
}

void FreeMoveComponent::update(duration_t delta, duration_t /*time*/) {
  const float dt          = std::chrono::duration<float>(delta).count();
  const float move_amount = kEn::Input::is_key_pressed(kEn::key::left_control) ? 3.F * dt * speed_ : dt * speed_;

  mEn::Vec3 direction{0.F};
  if (kEn::Input::is_key_pressed(kEn::key::up) || kEn::Input::is_key_pressed(kEn::key::w)) {
    direction += transform().local_front();
  }
  if (kEn::Input::is_key_pressed(kEn::key::down) || kEn::Input::is_key_pressed(kEn::key::s)) {
    direction -= transform().local_front();
  }
  if (kEn::Input::is_key_pressed(kEn::key::right) || kEn::Input::is_key_pressed(kEn::key::d)) {
    direction += transform().local_right();
  }
  if (kEn::Input::is_key_pressed(kEn::key::left) || kEn::Input::is_key_pressed(kEn::key::a)) {
    direction -= transform().local_right();
  }
  if (kEn::Input::is_key_pressed(kEn::key::space) || kEn::Input::is_key_pressed(kEn::key::e)) {
    direction += world_y_ ? mEn::Vec3(0, 1, 0) : transform().local_up();
  }
  if (kEn::Input::is_key_pressed(kEn::key::left_shift) || kEn::Input::is_key_pressed(kEn::key::q)) {
    direction -= world_y_ ? mEn::Vec3(0, 1, 0) : transform().local_up();
  }

  if (direction.x != 0.0F || direction.y != 0.0F || direction.z != 0.0F) {
    transform().fma(mEn::normalize(direction), move_amount);
  }
}

std::shared_ptr<GameComponent> FreeMoveComponent::clone() const {
  return std::make_shared<FreeMoveComponent>(speed_, world_y_);
}

void LookAtComponent::set_target(const GameObject& target) { target_ = target; }

void LookAtComponent::update(duration_t /*delta*/, duration_t /*time*/) {
  transform().look_at(target_.get().transform().pos());
}

std::shared_ptr<GameComponent> LookAtComponent::clone() const { return std::make_shared<LookAtComponent>(target_); }

}  // namespace kEn
