#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include <kEn/core/transform.hpp>
#include <kenpch.hpp>

namespace kEn {

Transform::Transform(glm::vec3 pos, glm::quat rot, glm::vec3 scale)
    : pos_(pos), rot_(rot), scale_(scale), dirty_(true), inverse_dirty_(true) {}

Transform::~Transform() {
  for (const auto child : children_) {
    child.get().parent_.reset();
  }

  if (parent_.has_value()) {
    unset_parent();
  }
}

void Transform::set_dirty() {
  if (dirty_) {
    return;
  }

  dirty_         = true;
  inverse_dirty_ = true;

  for (const auto child : children_) {
    child.get().set_dirty();
  }
}

void Transform::unset_parent() {
  if (parent_.has_value()) {
    std::erase_if(parent_.value().get().children_, [this](auto ref) { return std::addressof(ref.get()) == this; });
  }

  parent_.reset();
}

void Transform::set_parent(Transform& parent) {
  if (parent_.has_value()) {
    std::erase_if(parent_.value().get().children_, [this](auto ref) { return std::addressof(ref.get()) == this; });
  }

  parent_ = parent;

  parent.children_.emplace_back(*this);

  set_dirty();
}

glm::mat4 Transform::local_to_parent_matrix() const {
  return glm::translate(glm::mat4(1.0F), pos_) * glm::mat4_cast(rot_) * glm::scale(glm::mat4(1.0F), scale_);
}

glm::mat4& Transform::local_to_world_matrix() const {
  if (!dirty_) {
    return model_mat_;
  }

  model_mat_ = local_to_parent_matrix();
  if (parent_) {
    model_mat_ = parent_.value().get().local_to_world_matrix() * model_mat_;
  }

  dirty_ = false;
  return model_mat_;
}

glm::mat4 Transform::world_to_local_matrix() const {
  if (!inverse_dirty_) {
    return inv_model_mat_;
  }
  if (dirty_) {
    local_to_world_matrix();
  }

  inv_model_mat_ = glm::inverse(model_mat_);
  inverse_dirty_ = false;
  return inv_model_mat_;
}

void Transform::model_matrix_updated() {
  glm::vec3 skew;
  glm::vec4 perspective;

  inverse_dirty_ = true;

  glm::mat4 local_mat = model_mat_;
  if (parent_) {
    local_mat = parent_.value().get().world_to_local_matrix() * local_mat;
  }

  glm::decompose(local_mat, scale_, rot_, pos_, skew, perspective);
}

void Transform::rotate(const glm::vec3& axis, float angle) {
  rot_ = glm::rotate(rot_, angle, axis);
  set_dirty();
}

void Transform::rotate(const glm::quat& rotation) {
  rot_ = glm::normalize(rotation * rot_);
  set_dirty();
}

void Transform::rotate_local(const glm::quat& rotation) {
  rot_ = glm::normalize(rot_ * rotation);
  set_dirty();
}

void Transform::look_at(const glm::vec3& point, const glm::vec3& up) {
  const glm::vec3 local_point =
      parent_.has_value() ? glm::vec3(parent_.value().get().world_to_local_matrix() * glm::vec4(point, 1.F)) : point;
  glm::vec3 direction = local_point - pos_;
  direction           = glm::normalize(direction);
  rot_                = glm::quatLookAt(direction, up);

  set_dirty();
}

void Transform::fma(const glm::vec3& axis, float amount) {
  pos_ += amount * axis;
  set_dirty();
}

glm::vec3 Transform::local_right() const { return rot_ * glm::vec3(1, 0, 0); }

glm::vec3 Transform::local_front() const { return rot_ * glm::vec3(0, 0, -1); }

glm::vec3 Transform::local_up() const { return rot_ * glm::vec3(0, 1, 0); }

glm::vec3 Transform::right() const {
  glm::vec3 local = local_right();

  return parent_ ? parent_.value().get().local_to_world_matrix() * glm::vec4(local, 0.0F) : local;
}

glm::vec3 Transform::front() const {
  glm::vec3 local = local_front();
  return parent_ ? parent_.value().get().local_to_world_matrix() * glm::vec4(local, 0.0F) : local;
}

glm::vec3 Transform::up() const {
  glm::vec3 local = local_up();
  return parent_ ? parent_.value().get().local_to_world_matrix() * glm::vec4(local, 0.0F) : local;
}

}  // namespace kEn
