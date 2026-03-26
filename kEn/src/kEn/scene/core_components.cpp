#include "core_components.hpp"

#include <memory>

#include <mEn/constants.hpp>
#include <mEn/functions/common.hpp>
#include <mEn/functions/geometric.hpp>
#include <mEn/functions/quaternion_transform.hpp>
#include <mEn/functions/trigonometric.hpp>
#include <mEn/functions/vector_relational.hpp>
#include <mEn/fwd.hpp>
#include <mEn/vec2.hpp>
#include <mEn/vec3.hpp>

#include <kEn/core/application.hpp>
#include <kEn/core/input.hpp>
#include <kEn/core/key_codes.hpp>
#include <kEn/core/timestep.hpp>
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
  dispatcher_.subscribe(this, &FreeLookComponent::on_window_resize);
  const Window& main = kEn::Application::instance().main_window();
  window_center_     = {main.width() / 2, main.height() / 2};
}

void FreeLookComponent::update(Timestep /*delta*/, Timestep /*time*/) {
  if (kEn::input::is_key_pressed(kEn::key::escape)) {
    kEn::input::set_cursor_visible(true);
    update_ = false;
  }

  if (kEn::input::is_key_pressed(kEn::key::f)) {
    kEn::input::set_mouse_pos(window_center_);
    kEn::input::set_cursor_visible(false);
    update_ = true;
  }

  if (!update_) {
    return;
  }

  auto delta_pos              = kEn::input::mouse_pos() - window_center_;
  const mEn::vec<2, bool> rot = mEn::notNear(delta_pos, 0.0F);

  if (rot.x) {
    yaw_ -= mEn::radians(delta_pos.x) * sensitivity_;
  }

  if (rot.y) {
    pitch_ -= mEn::radians(delta_pos.y) * sensitivity_;
    pitch_ = mEn::clamp(pitch_, (-mEn::kHalfPi<float>)+0.01F, mEn::kHalfPi<float> - 0.01F);
  }

  if (mEn::any(rot)) {
    kEn::input::set_mouse_pos(window_center_);

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

void FreeMoveComponent::update(Timestep delta, Timestep /*time*/) {
  const auto dt           = static_cast<float>(delta.seconds());
  const float move_amount = kEn::input::is_key_pressed(kEn::key::left_control) ? 3.F * dt * speed_ : dt * speed_;

  mEn::Vec3 direction{0.F};
  if (kEn::input::is_key_pressed(kEn::key::up) || kEn::input::is_key_pressed(kEn::key::w)) {
    direction += transform().local_front();
  }
  if (kEn::input::is_key_pressed(kEn::key::down) || kEn::input::is_key_pressed(kEn::key::s)) {
    direction -= transform().local_front();
  }
  if (kEn::input::is_key_pressed(kEn::key::right) || kEn::input::is_key_pressed(kEn::key::d)) {
    direction += transform().local_right();
  }
  if (kEn::input::is_key_pressed(kEn::key::left) || kEn::input::is_key_pressed(kEn::key::a)) {
    direction -= transform().local_right();
  }
  if (kEn::input::is_key_pressed(kEn::key::space) || kEn::input::is_key_pressed(kEn::key::e)) {
    direction += world_y_ ? mEn::Vec3(0, 1, 0) : transform().local_up();
  }
  if (kEn::input::is_key_pressed(kEn::key::left_shift) || kEn::input::is_key_pressed(kEn::key::q)) {
    direction -= world_y_ ? mEn::Vec3(0, 1, 0) : transform().local_up();
  }

  if (direction.x != 0.0F || direction.y != 0.0F || direction.z != 0.0F) {
    transform().translate_local(move_amount * mEn::normalize(direction));
  }
}

std::shared_ptr<GameComponent> FreeMoveComponent::clone() const {
  return std::make_shared<FreeMoveComponent>(speed_, world_y_);
}

void LookAtComponent::set_target(const GameObject& target) { target_ = target; }

void LookAtComponent::update(Timestep /*delta*/, Timestep /*time*/) {
  transform().look_at(target_.get().transform().world_pos());
}

std::shared_ptr<GameComponent> LookAtComponent::clone() const { return std::make_shared<LookAtComponent>(target_); }

}  // namespace kEn
