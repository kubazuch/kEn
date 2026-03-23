#include "transform.hpp"

#include <cstdlib>
#include <span>
#include <vector>

#include <mEn/constants.hpp>
#include <mEn/features/mat_decompose.hpp>
#include <mEn/functions/geometric.hpp>
#include <mEn/functions/matrix_transform.hpp>
#include <mEn/functions/quaternion_common.hpp>
#include <mEn/functions/quaternion_geometric.hpp>
#include <mEn/functions/quaternion_transform.hpp>
#include <mEn/functions/vector_relational.hpp>
#include <mEn/fwd.hpp>

#include <kEn/core/assert.hpp>
#include <kEn/scene/game_object.hpp>

namespace kEn {

Transform::Transform(mEn::Vec3 pos, mEn::Quat rot, mEn::Vec3 scale)
    : pos_(pos), rot_(mEn::normalize(rot)), scale_(scale) {}

Transform::~Transform() {
  owner_ = nullptr;

  for (auto* child : children_) {
    KEN_CORE_ASSERT(child != nullptr, "children_ invariant violated: null child in Transform destructor");
    child->parent_ = nullptr;
    child->set_dirty();
  }

  detach_from_parent_unsafe();
}

bool Transform::can_set_parent(const Transform& parent) const noexcept {
  if (&parent == this) {
    return false;
  }

  // Cycle check
  for (const auto* p = &parent; p != nullptr; p = p->parent_) {
    if (p == this) {
      return false;
    }
  }

  return true;
}

bool Transform::try_set_parent(Transform& parent) {
  if (!can_set_parent(parent)) {
    return false;
  }

  if (parent_ == &parent) {
    return true;  // already has this parent
  }

  detach_from_parent_unsafe();

  parent_ = &parent;
  parent.children_.push_back(this);

  set_dirty();
  return true;
}

void Transform::set_parent(Transform& parent) {
  const bool ok = try_set_parent(parent);
  KEN_CORE_ASSERT(ok, "Transform::set_parent would create a cycle or self-parenting");
  if (!ok) {
    KEN_UNREACHABLE();
  }
}

void Transform::detach_from_parent_unsafe() {
  if (parent_ == nullptr) {
    return;
  }

  std::erase(parent_->children_, this);
  parent_ = nullptr;
}

void Transform::unset_parent() {
  if (parent_ == nullptr) {
    return;
  }

  detach_from_parent_unsafe();
  set_dirty();
}

std::span<Transform* const> Transform::children() const noexcept { return {children_.data(), children_.size()}; }

std::span<Transform*> Transform::children() noexcept { return {children_.data(), children_.size()}; }

mEn::Mat4 Transform::local_to_parent_matrix() const {
  return mEn::translate(mEn::Mat4(1.0F), pos_) * static_cast<mEn::Mat4>(rot_) * mEn::scale(mEn::Mat4(1.0F), scale_);
}

mEn::Mat4 Transform::parent_to_local_matrix() const {
  return mEn::scale(mEn::Mat4(1.0F), 1.0F / scale_) * static_cast<mEn::Mat4>(mEn::conjugate(rot_)) *
         mEn::translate(mEn::Mat4(1.0F), -pos_);
}

const mEn::Mat4& Transform::local_to_world_matrix() const {
  if (!dirty_) {
    return model_mat_;
  }

  model_mat_ = local_to_parent_matrix();
  if (parent_ != nullptr) {
    model_mat_ = parent_->local_to_world_matrix() * model_mat_;
  }

  dirty_ = false;
  return model_mat_;
}

const mEn::Mat4& Transform::world_to_local_matrix() const {
  if (!inverse_dirty_) {
    return inv_model_mat_;
  }

  inv_model_mat_ = parent_to_local_matrix();
  if (parent_ != nullptr) {
    inv_model_mat_ = inv_model_mat_ * parent_->world_to_local_matrix();
  }

  inverse_dirty_ = false;
  return inv_model_mat_;
}

void Transform::set_dirty() {
  if (dirty_ && inverse_dirty_) {
    return;
  }

  dirty_         = true;
  inverse_dirty_ = true;

  for (auto* child : children_) {
    if (child != nullptr) {
      child->set_dirty();
    }
  }

  if (owner_ != nullptr) {
    owner_->on_transform_changed();
  }
}

void Transform::translate_local(const mEn::Vec3& delta) {
  pos_ += delta;
  set_dirty();
}

void Transform::translate_world(const mEn::Vec3& delta) { set_world_pos(world_pos() + delta); }

void Transform::rotate(const mEn::Vec3& axis, float angle) {
  KEN_CORE_ASSERT(mEn::length(axis) > mEn::kEpsilon<float>, "rotate: axis must be non-zero");
  rot_ = mEn::normalize(mEn::rotate(rot_, angle, axis));
  set_dirty();
}

void Transform::rotate(const mEn::Quat& rotation) {
  KEN_CORE_ASSERT(mEn::length(rotation) > mEn::kEpsilon<float>, "rotate: rotation must be non-zero");
  rot_ = mEn::normalize(rotation * rot_);
  set_dirty();
}

void Transform::rotate_local(const mEn::Quat& rotation) {
  KEN_CORE_ASSERT(mEn::length(rotation) > mEn::kEpsilon<float>, "rotate: rotation must be non-zero");
  rot_ = mEn::normalize(rot_ * rotation);
  set_dirty();
}

void Transform::look_at(const mEn::Vec3& point, const mEn::Vec3& up) {
  constexpr float kLookAtTol = 1e-3F;

  const mEn::Vec3 local_point =
      parent_ != nullptr ? mEn::Vec3(parent_->world_to_local_matrix() * mEn::Vec4(point, 1.F)) : point;

  mEn::Vec3 direction = local_point - pos_;
  if (mEn::length(direction) < mEn::kEpsilon<float>) {
    return;
  }
  direction = mEn::normalize(direction);

  const mEn::Vec3 current_front = local_front();
  if (mEn::all(mEn::near(direction, current_front, kLookAtTol))) {
    return;
  }

  mEn::Vec3 local_up_vec = up;
  if (parent_ != nullptr) {
    local_up_vec = mEn::normalize(mEn::conjugate(parent_->world_rot()) * up);
  }

  // Guard against up being parallel to direction (would produce NaN in quatLookAt)
  if (std::abs(mEn::dot(direction, local_up_vec)) > 1.0F - kLookAtTol) {
    local_up_vec = std::abs(direction.x) < 0.9F ? mEn::Vec3(1, 0, 0) : mEn::Vec3(0, 0, 1);
  }

  rot_ = mEn::quatLookAt(direction, local_up_vec);
  set_dirty();
}

void Transform::reset() {
  pos_   = mEn::Vec3{};
  rot_   = mEn::Quat{1, 0, 0, 0};
  scale_ = mEn::Vec3{1, 1, 1};
  set_dirty();
}

void Transform::set_local_transform(const mEn::Vec3& pos, const mEn::Quat& rot, const mEn::Vec3& scale) {
  pos_   = pos;
  rot_   = mEn::normalize(rot);
  scale_ = scale;
  set_dirty();
}

void Transform::set_local_pos(const mEn::Vec3& pos) {
  pos_ = pos;
  set_dirty();
}

void Transform::set_local_rot(const mEn::Quat& rot) {
  rot_ = mEn::normalize(rot);
  set_dirty();
}

void Transform::set_local_scale(const mEn::Vec3& scale) {
  scale_ = scale;
  set_dirty();
}

void Transform::set_local_scale(float uniform_scale) {
  scale_ = mEn::Vec3(uniform_scale);
  set_dirty();
}

mEn::Vec3 Transform::world_pos() const {
  return parent_ != nullptr ? mEn::Vec3(parent_->local_to_world_matrix() * mEn::Vec4(pos_, 1.F)) : pos_;
}

void Transform::set_world_pos(const mEn::Vec3& world_pos) {
  const mEn::Vec3 local =
      parent_ != nullptr ? mEn::Vec3(parent_->world_to_local_matrix() * mEn::Vec4(world_pos, 1.F)) : world_pos;
  set_local_pos(local);
}

mEn::Quat Transform::world_rot() const {
  return parent_ != nullptr ? mEn::normalize(parent_->world_rot() * rot_) : rot_;
}

void Transform::set_world_rot(const mEn::Quat& world_rot) {
  const mEn::Quat local =
      parent_ != nullptr ? mEn::normalize(mEn::conjugate(parent_->world_rot()) * world_rot) : mEn::normalize(world_rot);
  set_local_rot(local);
}

mEn::Vec3 Transform::world_scale() const {
  const mEn::Mat4& m = local_to_world_matrix();

  const float sx = mEn::length(mEn::Vec3(m[0]));
  const float sy = mEn::length(mEn::Vec3(m[1]));
  const float sz = mEn::length(mEn::Vec3(m[2]));

  return {sx, sy, sz};
}

void Transform::set_world_scale(const mEn::Vec3& desired_world_scale) {
  if (parent_ == nullptr) {
    set_local_scale(desired_world_scale);
    return;
  }

  const mEn::Vec3 parent_world_scale = parent_->world_scale();
  KEN_CORE_ASSERT(mEn::all(mEn::greaterThan(parent_world_scale, 0.F)),
                  "set_world_scale: parent has a zero scale component");
  set_local_scale(desired_world_scale / parent_world_scale);
}

void Transform::set_world_scale(float uniform_scale) { set_world_scale(mEn::Vec3(uniform_scale)); }

mEn::Vec3 Transform::local_right() const { return rot_ * mEn::Vec3(1, 0, 0); }

mEn::Vec3 Transform::local_front() const { return rot_ * mEn::Vec3(0, 0, -1); }

mEn::Vec3 Transform::local_up() const { return rot_ * mEn::Vec3(0, 1, 0); }

mEn::Vec3 Transform::world_right() const { return world_rot() * mEn::Vec3(1, 0, 0); }

mEn::Vec3 Transform::world_front() const { return world_rot() * mEn::Vec3(0, 0, -1); }

mEn::Vec3 Transform::world_up() const { return world_rot() * mEn::Vec3(0, 1, 0); }

Transform::WorldMatrixEdit::WorldMatrixEdit(Transform& t)
    : t_(t), world_(t.local_to_world_matrix()), t0_(), r0_(1, 0, 0, 0), s0_(1, 1, 1) {
  (void)mEn::decompose(world_, t0_, r0_, s0_);
}

Transform::WorldMatrixEdit::~WorldMatrixEdit() {
  // If caller told us gizmo isn't using it, skip all work.
  if (hint_using_.has_value() && !*hint_using_) {
    return;
  }

  // If caller told us gizmo is using it, commit without expensive compares.
  if (hint_using_.has_value() && *hint_using_) {
    commit_from_world();
    return;
  }

  // Fallback: do semantic change detection
  if (!changed_trs_approx()) {
    return;
  }
  commit_from_world();
}

bool Transform::WorldMatrixEdit::changed_trs_approx() {
  (void)mEn::decompose(world_, t1_, r1_, s1_);
  decomposed_ = true;
  return t0_ != t1_ || r0_ != r1_ || s0_ != s1_;
}

void Transform::WorldMatrixEdit::commit_from_world() {
  if (!decomposed_) {
    (void)mEn::decompose(world_, t1_, r1_, s1_);
  }
  t_.set_world_pos(t1_);
  t_.set_world_rot(r1_);
  t_.set_world_scale(s1_);
}

Transform::LocalTRSEdit::~LocalTRSEdit() {
  const bool changed = hint_changed_.has_value() ? *hint_changed_ : changed_approx();
  if (!changed) {
    return;
  }

  // keep invariants
  rot = mEn::normalize(rot);
  t_.set_dirty();
}

}  // namespace kEn
