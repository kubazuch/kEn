#include "camera_controllers.hpp"

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
#include <kEn/core/assert.hpp>
#include <kEn/core/input/input.hpp>
#include <kEn/core/input/key_codes.hpp>
#include <kEn/core/timestep.hpp>
#include <kEn/core/window.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/event/event.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/scene/component.hpp>
#include <kEn/scene/game_object.hpp>

namespace kEn {

FreeLookComponent::FreeLookComponent(float sensitivity) : sensitivity_(sensitivity), window_center_() {
  dispatcher_.subscribe(this, &FreeLookComponent::on_window_resize);
}

void FreeLookComponent::on_attach() {
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

  auto delta_pos                     = kEn::input::mouse_pos() - window_center_;
  const mEn::vec<2, bool> did_change = mEn::notNear(delta_pos, 0.0F);

  if (did_change.x) {
    yaw_ -= mEn::radians(delta_pos.x * sensitivity_);
    yaw_ = mEn::wrap(yaw_, -mEn::kPi<>, mEn::kPi<>);
  }

  if (did_change.y) {
    pitch_ -= mEn::radians(delta_pos.y * sensitivity_);
    pitch_ = mEn::clamp(pitch_, -mEn::kHalfPi<> + mEn::kEpsilon<>, mEn::kHalfPi<> - mEn::kEpsilon<>);
  }

  if (mEn::any(did_change)) {
    kEn::input::set_mouse_pos(window_center_);

    mEn::Quat rot(1, 0, 0, 0);
    rot = mEn::rotate(rot, yaw_, mEn::Vec3(0.0F, 1.0F, 0.0F));
    rot = mEn::rotate(rot, pitch_, mEn::Vec3(1.0F, 0.0F, 0.0F));
    transform().set_local_rot(rot);
  }
}

std::unique_ptr<GameComponent> FreeLookComponent::clone() const {
  return std::make_unique<FreeLookComponent>(sensitivity_);
}

bool FreeLookComponent::on_window_resize(const kEn::WindowResizeEvent& event) {
  window_center_ = {event.width() / 2, event.height() / 2};
  return false;
}

void FreeMoveComponent::update(Timestep delta, Timestep /*time*/) {
  constexpr float kSprintMultiplier = 3.0F;
  constexpr mEn::Vec3 kWorldUp{0.0F, 1.0F, 0.0F};

  using enum kEn::Key;
  const bool sprint       = kEn::input::is_key_pressed(left_control);
  const auto dt           = static_cast<float>(delta.seconds());
  const float move_amount = sprint ? kSprintMultiplier * dt * speed_ : dt * speed_;

  mEn::Vec3 direction{0.F};
  if (kEn::input::is_key_pressed(up) || kEn::input::is_key_pressed(w)) {
    direction += transform().local_front();
  }
  if (kEn::input::is_key_pressed(down) || kEn::input::is_key_pressed(s)) {
    direction -= transform().local_front();
  }
  if (kEn::input::is_key_pressed(right) || kEn::input::is_key_pressed(d)) {
    direction += transform().local_right();
  }
  if (kEn::input::is_key_pressed(left) || kEn::input::is_key_pressed(a)) {
    direction -= transform().local_right();
  }
  if (kEn::input::is_key_pressed(space) || kEn::input::is_key_pressed(e)) {
    direction += world_y_ ? kWorldUp : transform().local_up();
  }
  if (kEn::input::is_key_pressed(left_shift) || kEn::input::is_key_pressed(q)) {
    direction -= world_y_ ? kWorldUp : transform().local_up();
  }

  if (direction.x != 0.0F || direction.y != 0.0F || direction.z != 0.0F) {
    transform().translate_local(move_amount * mEn::normalize(direction));
  }
}

std::unique_ptr<GameComponent> FreeMoveComponent::clone() const {
  return std::make_unique<FreeMoveComponent>(speed_, world_y_);
}

void LookAtComponent::update(Timestep /*delta*/, Timestep /*time*/) {
  KEN_CORE_ASSERT(target_ != nullptr, "LookAtComponent target has been destroyed");
  transform().look_at(target_->transform().world_pos());
}

std::unique_ptr<GameComponent> LookAtComponent::clone() const { return std::make_unique<LookAtComponent>(*target_); }

}  // namespace kEn
