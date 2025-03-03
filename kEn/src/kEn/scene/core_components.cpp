#include <kEn/core/application.hpp>
#include <kEn/core/input.hpp>
#include <kEn/scene/core_components.hpp>
#include <kEn/scene/game_object.hpp>
#include <kenpch.hpp>

namespace kEn {

void ModelComponent::render(Shader& shader) {
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

void FreeLookComponent::update(float) {
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

  auto delta_pos = kEn::Input::get_mouse_pos() - window_center_;
  bool rot_y     = delta_pos.x != 0;
  bool rot_x     = delta_pos.y != 0;

  if (rot_y) {
    yaw_ -= glm::radians(delta_pos.x) * sensitivity_;
  }

  if (rot_x) {
    pitch_ -= glm::radians(delta_pos.y) * sensitivity_;
    pitch_ = glm::clamp(pitch_, -glm::pi<float>() / 2.F + 0.01F, glm::pi<float>() / 2.F - 0.01F);
  }

  if (rot_x || rot_y) {
    kEn::Input::set_mouse_pos(window_center_);
    glm::quat q_pitch = glm::angleAxis(pitch_, glm::vec3(1, 0, 0));
    glm::quat q_yaw   = glm::angleAxis(yaw_, glm::vec3(0, 1, 0));
    transform().set_local_rot(q_yaw * q_pitch);
  }
}

std::shared_ptr<GameComponent> FreeLookComponent::clone() const {
  return std::make_shared<FreeLookComponent>(sensitivity_);
}

bool FreeLookComponent::on_window_resize(const kEn::WindowResizeEvent& event) {
  window_center_ = glm::vec2(event.width() / 2, event.height() / 2);
  return false;
}

void FreeMoveComponent::update(float delta) {
  float move_amount = kEn::Input::is_key_pressed(kEn::key::left_control) ? 3.F * delta * speed_ : delta * speed_;
  glm::vec3 direction{0.F};
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
  if (kEn::Input::is_key_pressed(kEn::key::space) || kEn::Input::is_key_pressed(kEn::key::q)) {
    direction += world_y_ ? glm::vec3(0, 1, 0) : transform().local_up();
  }
  if (kEn::Input::is_key_pressed(kEn::key::left_shift) || kEn::Input::is_key_pressed(kEn::key::e)) {
    direction -= world_y_ ? glm::vec3(0, 1, 0) : transform().local_up();
  }

  if (direction.x || direction.y || direction.z) {
    transform().fma(glm::normalize(direction), move_amount);
  }
}

std::shared_ptr<GameComponent> FreeMoveComponent::clone() const {
  return std::make_shared<FreeMoveComponent>(speed_, world_y_);
}

void LookAtComponent::set_target(const GameObject& target) { target_ = target; }

void LookAtComponent::update(float) { transform().look_at(target_.get().transform().pos()); }

std::shared_ptr<GameComponent> LookAtComponent::clone() const { return std::make_shared<LookAtComponent>(target_); }

}  // namespace kEn
