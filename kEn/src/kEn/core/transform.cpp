#include <kEn/core/transform.hpp>
#include <kEn/scene/game_object.hpp>
#include <kenpch.hpp>
#include <mEn.hpp>

namespace kEn {

Transform::Transform(mEn::Vec3 pos, mEn::Quat rot, mEn::Vec3 scale)
    : pos_(pos), rot_(rot), scale_(scale), dirty_(true), inverse_dirty_(true) {}

Transform::~Transform() {
  for (const auto child : children_) {
    child.get().parent_.reset();
  }

  if (parent_.has_value()) {
    unset_parent();
  }
}

void Transform::set_parent(Transform& parent) {
  if (parent_.has_value()) {
    std::erase_if(parent_.value().get().children_, [this](auto ref) { return std::addressof(ref.get()) == this; });
  }

  parent_ = parent;

  parent.children_.emplace_back(*this);

  set_dirty();
}

void Transform::unset_parent() {
  if (parent_.has_value()) {
    std::erase_if(parent_.value().get().children_, [this](auto ref) { return std::addressof(ref.get()) == this; });
  }

  parent_.reset();
}

mEn::Mat4 Transform::local_to_parent_matrix() const {
  return mEn::translate(mEn::Mat4(1.0F), pos_) * mEn::mat4_cast(rot_) * mEn::scale(mEn::Mat4(1.0F), scale_);
}

mEn::Mat4& Transform::local_to_world_matrix() const {
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

mEn::Mat4 Transform::world_to_local_matrix() const {
  if (!inverse_dirty_) {
    return inv_model_mat_;
  }
  if (dirty_) {
    local_to_world_matrix();
  }

  inv_model_mat_ = mEn::inverse(model_mat_);
  inverse_dirty_ = false;
  return inv_model_mat_;
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

  if (owner_.has_value()) {
    owner_->get().on_transform_changed();
  }
}

void Transform::rotate(const mEn::Vec3& axis, float angle) {
  rot_ = mEn::rotate(rot_, angle, axis);
  set_dirty();
}

void Transform::rotate(const mEn::Quat& rotation) {
  rot_ = mEn::normalize(rotation * rot_);
  set_dirty();
}

void Transform::rotate_local(const mEn::Quat& rotation) {
  rot_ = mEn::normalize(rot_ * rotation);
  set_dirty();
}

void Transform::look_at(const mEn::Vec3& point, const mEn::Vec3& up) {
  const mEn::Vec3 local_point =
      parent_.has_value() ? mEn::Vec3(parent_.value().get().world_to_local_matrix() * mEn::Vec4(point, 1.F)) : point;
  mEn::Vec3 direction = local_point - pos_;
  direction           = mEn::normalize(direction);

  mEn::Vec3 front = local_front();
  if (std::abs(direction.x - front.x) > 1e-3F || std::abs(direction.y - front.y) > 1e-3F ||
      std::abs(direction.z - front.z) > 1e-3F) {
    if (std::abs(direction.x) > 1e-3F || std::abs(direction.z) > 1e-3F) {
      rot_ = mEn::quatLookAt(direction, up);
    } else {
      rot_ = mEn::quatLookAt(direction, local_up());
    }

    set_dirty();
  }
}

void Transform::fma(const mEn::Vec3& axis, float amount) {
  pos_ += amount * axis;
  set_dirty();
}

mEn::Vec3 Transform::local_right() const { return rot_ * mEn::Vec3(1, 0, 0); }

mEn::Vec3 Transform::local_front() const { return rot_ * mEn::Vec3(0, 0, -1); }

mEn::Vec3 Transform::local_up() const { return rot_ * mEn::Vec3(0, 1, 0); }

mEn::Vec3 Transform::right() const {
  mEn::Vec3 local = local_right();

  return parent_ ? mEn::Vec3(parent_.value().get().local_to_world_matrix() * mEn::Vec4(local, 0.0F)) : local;
}

mEn::Vec3 Transform::front() const {
  mEn::Vec3 local = local_front();
  return parent_ ? mEn::Vec3(parent_.value().get().local_to_world_matrix() * mEn::Vec4(local, 0.0F)) : local;
}

mEn::Vec3 Transform::up() const {
  mEn::Vec3 local = local_up();
  return parent_ ? mEn::Vec3(parent_.value().get().local_to_world_matrix() * mEn::Vec4(local, 0.0F)) : local;
}

}  // namespace kEn
