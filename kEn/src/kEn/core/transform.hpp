#pragma once

#include <mEn.hpp>
#include <optional>
#include <vector>

namespace kEn {

class Transform {
 public:
  explicit Transform(mEn::Vec3 pos = mEn::Vec3(), mEn::Quat rot = {1, 0, 0, 0}, mEn::Vec3 scale = {1, 1, 1});
  ~Transform();

  void set_parent(Transform& parent);
  void unset_parent();
  const Transform& get_parent() const { return *parent_; }

  mEn::Mat4 local_to_parent_matrix() const;
  mEn::Mat4& local_to_world_matrix() const;
  mEn::Mat4 world_to_local_matrix() const;
  void mark_dirty() const;

  void rotate(const mEn::Vec3& axis, float angle);
  void rotate(const mEn::Quat& rotation);
  void rotate_local(const mEn::Quat& rotation);
  void look_at(const mEn::Vec3& point, const mEn::Vec3& up = mEn::Vec3(0, 1, 0));
  void fma(const mEn::Vec3& axis, float amount);

  const mEn::Vec3& local_pos() const { return pos_; }
  mEn::Vec3& local_pos() { return pos_; }
  mEn::Vec3 pos() const {
    return parent_.has_value() ? mEn::Vec3(parent_.value().get().local_to_world_matrix() * mEn::Vec4(pos_, 1.F)) : pos_;
  }
  void set_local_pos(const mEn::Vec3& pos) {
    pos_ = pos;
    set_dirty();
  }

  const mEn::Quat& local_rot() const { return rot_; }
  mEn::Quat& local_rot() { return rot_; }
  mEn::Quat rot() const { return parent_.has_value() ? parent_.value().get().rot() * rot_ : rot_; }
  void set_local_rot(const mEn::Quat& rot) {
    rot_ = rot;
    set_dirty();
  }

  const mEn::Vec3& local_scale() const { return scale_; }
  mEn::Vec3& local_scale() { return scale_; }
  void set_local_scale(const mEn::Vec3& scale) {
    scale_ = scale;
    set_dirty();
  }
  void set_local_scale(float scale) {
    scale_ = mEn::Vec3(scale);
    set_dirty();
  }

  mEn::Vec3 local_front() const;
  mEn::Vec3 local_right() const;
  mEn::Vec3 local_up() const;

  mEn::Vec3 front() const;
  mEn::Vec3 right() const;
  mEn::Vec3 up() const;

  void set_dirty();

  Transform(const Transform&)                     = delete;
  Transform(Transform&&)                          = delete;
  Transform& operator=(const Transform&) noexcept = delete;
  Transform& operator=(Transform&&) noexcept      = delete;

 private:
  std::optional<std::reference_wrapper<Transform>> parent_;
  std::vector<std::reference_wrapper<Transform>> children_;

  mEn::Vec3 pos_;
  mEn::Quat rot_;
  mEn::Vec3 scale_;

  mutable bool dirty_          = false;
  mutable mEn::Mat4 model_mat_ = mEn::Mat4(1);

  mutable bool inverse_dirty_      = false;
  mutable mEn::Mat4 inv_model_mat_ = mEn::Mat4(1);
};

}  // namespace kEn
