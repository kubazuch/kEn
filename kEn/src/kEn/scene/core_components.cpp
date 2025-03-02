#include <kEn/core/application.hpp>
#include <kEn/core/input.hpp>
#include <kEn/scene/core_components.hpp>
#include <kEn/scene/game_object.hpp>
#include <kenpch.hpp>

namespace kEn {

void model_component::render(shader& shader) {
  if (!parent_.has_value()) return;
  model_->render(shader, transform());
}

std::shared_ptr<game_component> model_component::clone() const { return std::make_shared<model_component>(model_); }

free_look_component::free_look_component(float sensitivity) : sensitivity_(sensitivity) {
  dispatcher_.subscribe<window_resize_event>(KEN_EVENT_SUBSCRIBER(on_window_resize));
  const window& main = kEn::application::instance().main_window();
  window_center_     = {main.width() / 2, main.height() / 2};
}

void free_look_component::update(float delta) {
  if (kEn::input::is_key_pressed(kEn::key::escape)) {
    kEn::input::set_cursor_visible(true);
    update_ = false;
  }

  if (kEn::input::is_key_pressed(kEn::key::f)) {
    kEn::input::set_mouse_pos(window_center_);
    kEn::input::set_cursor_visible(false);
    update_ = true;
  }

  if (!update_) return;

  glm::vec2 delta_pos = kEn::input::get_mouse_pos() - window_center_;
  bool rotY           = delta_pos.x != 0;
  bool rotX           = delta_pos.y != 0;

  if (rotY) {
    yaw_ -= glm::radians(delta_pos.x) * sensitivity_;
  }

  if (rotX) {
    pitch_ -= glm::radians(delta_pos.y) * sensitivity_;
    pitch_ = glm::clamp(pitch_, -glm::pi<float>() / 2.f + 0.01f, glm::pi<float>() / 2.f - 0.01f);
  }

  if (rotX || rotY) {
    kEn::input::set_mouse_pos(window_center_);
    glm::quat qPitch = glm::angleAxis(pitch_, glm::vec3(1, 0, 0));
    glm::quat qYaw   = glm::angleAxis(yaw_, glm::vec3(0, 1, 0));
    transform().set_local_rot(qYaw * qPitch);
  }
}

std::shared_ptr<game_component> free_look_component::clone() const {
  return std::make_shared<free_look_component>(sensitivity_);
}

bool free_look_component::on_window_resize(const kEn::window_resize_event& event) {
  window_center_ = glm::vec2(event.width() / 2, event.height() / 2);
  return false;
}

void free_move_component::update(float delta) {
  float move_amount = kEn::input::is_key_pressed(kEn::key::left_control) ? 3.f * delta * speed_ : delta * speed_;
  glm::vec3 direction{0.f};
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
  if (kEn::input::is_key_pressed(kEn::key::space) || kEn::input::is_key_pressed(kEn::key::q)) {
    direction += world_y_ ? glm::vec3(0, 1, 0) : transform().local_up();
  }
  if (kEn::input::is_key_pressed(kEn::key::left_shift) || kEn::input::is_key_pressed(kEn::key::e)) {
    direction -= world_y_ ? glm::vec3(0, 1, 0) : transform().local_up();
  }

  if (direction.x || direction.y || direction.z) {
    transform().fma(glm::normalize(direction), move_amount);
  }
}

std::shared_ptr<game_component> free_move_component::clone() const {
  return std::make_shared<free_move_component>(speed_, world_y_);
}

void look_at_component::set_target(const game_object& target) { target_ = target; }

void look_at_component::update(float delta) { transform().look_at(target_.get().transform().pos()); }

std::shared_ptr<game_component> look_at_component::clone() const {
  return std::make_shared<look_at_component>(target_);
}

}  // namespace kEn
