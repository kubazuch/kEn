#pragma once

#include <optional>
#include <span>
#include <vector>

#include <mEn/features/mat_decompose.hpp>
#include <mEn/features/type_ptr.hpp>

#include <kEn/core/assert.hpp>
#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

class GameObject;

class Transform {
 public:
  explicit Transform(mEn::Vec3 pos = mEn::Vec3(), mEn::Quat rot = {1, 0, 0, 0}, mEn::Vec3 scale = {1, 1, 1});
  ~Transform();

  // Parenting
  [[nodiscard]] bool has_parent() const noexcept { return parent_ != nullptr; }
  [[nodiscard]] Transform* parent() noexcept { return parent_; }
  [[nodiscard]] const Transform* parent() const noexcept { return parent_; }

  [[nodiscard]] Transform& get_parent() {
    KEN_CORE_ASSERT(has_parent(), "Transform has no parent");
    return *parent_;
  }
  [[nodiscard]] const Transform& get_parent() const {
    KEN_CORE_ASSERT(has_parent(), "Transform has no parent");
    return *parent_;
  }

  [[nodiscard]] bool can_set_parent(const Transform& parent) const noexcept;
  bool try_set_parent(Transform& parent);
  void set_parent(Transform& parent);
  void unset_parent();

  [[nodiscard]] std::span<Transform* const> children() const noexcept;
  [[nodiscard]] std::span<Transform*> children() noexcept;

  // Ownership
  void set_owner(GameObject& owner) noexcept { owner_ = &owner; }
  void unset_owner() noexcept { owner_ = nullptr; }
  [[nodiscard]] bool has_owner() const noexcept { return owner_ != nullptr; }
  [[nodiscard]] GameObject* owner() noexcept { return owner_; }
  [[nodiscard]] const GameObject* owner() const noexcept { return owner_; }

  [[nodiscard]] GameObject& get_owner() {
    KEN_CORE_ASSERT(has_owner(), "Transform has no owner");
    return *owner_;
  }
  [[nodiscard]] const GameObject& get_owner() const {
    KEN_CORE_ASSERT(has_owner(), "Transform has no owner");
    return *owner_;
  }

  // Matrices
  [[nodiscard]] mEn::Mat4 local_to_parent_matrix() const;
  [[nodiscard]] mEn::Mat4 parent_to_local_matrix() const;
  [[nodiscard]] const mEn::Mat4& local_to_world_matrix() const;
  [[nodiscard]] const mEn::Mat4& world_to_local_matrix() const;

  void set_dirty();

  // Basic operations
  void translate_local(const mEn::Vec3& delta);
  void translate_world(const mEn::Vec3& delta);

  void rotate(const mEn::Vec3& axis, float angle);
  void rotate(const mEn::Quat& rotation);
  void rotate_local(const mEn::Quat& rotation);
  void look_at(const mEn::Vec3& point, const mEn::Vec3& up = mEn::Vec3(0, 1, 0));

  void reset();
  void set_local_transform(const mEn::Vec3& pos, const mEn::Quat& rot, const mEn::Vec3& scale);

  // Local-space access
  [[nodiscard]] const mEn::Vec3& local_pos() const noexcept { return pos_; }
  void set_local_pos(const mEn::Vec3& pos);

  [[nodiscard]] const mEn::Quat& local_rot() const noexcept { return rot_; }
  void set_local_rot(const mEn::Quat& rot);

  [[nodiscard]] const mEn::Vec3& local_scale() const noexcept { return scale_; }
  void set_local_scale(const mEn::Vec3& scale);
  void set_local_scale(float uniform_scale);

  // World-space derived values
  [[nodiscard]] mEn::Vec3 world_pos() const;
  void set_world_pos(const mEn::Vec3& world_pos);

  [[nodiscard]] mEn::Quat world_rot() const;
  void set_world_rot(const mEn::Quat& world_rot);

  [[nodiscard]] mEn::Vec3 world_scale() const;
  void set_world_scale(const mEn::Vec3& world_scale);
  void set_world_scale(float uniform_scale);

  [[nodiscard]] mEn::Vec3 local_front() const;
  [[nodiscard]] mEn::Vec3 local_right() const;
  [[nodiscard]] mEn::Vec3 local_up() const;

  [[nodiscard]] mEn::Vec3 world_front() const;
  [[nodiscard]] mEn::Vec3 world_right() const;
  [[nodiscard]] mEn::Vec3 world_up() const;

  DELETE_COPY_MOVE(Transform);

 public:
  class WorldMatrixEdit {
   public:
    mEn::Mat4& matrix() noexcept { return world_; }
    float* ptr() noexcept { return mEn::value_ptr(world_); }

    void set_active(bool using_gizmo) noexcept { hint_using_ = using_gizmo; }

    ~WorldMatrixEdit();

    DELETE_COPY_MOVE(WorldMatrixEdit);

   private:
    friend class Transform;

    explicit WorldMatrixEdit(Transform& t);

    bool changed_trs_approx();

    void commit_from_world();

    Transform& t_;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    mEn::Mat4 world_;
    mEn::Vec3 t0_;
    mEn::Quat r0_;
    mEn::Vec3 s0_;
    mEn::Vec3 t1_{};
    mEn::Quat r1_{1, 0, 0, 0};
    mEn::Vec3 s1_{1, 1, 1};
    bool decomposed_ = false;
    std::optional<bool> hint_using_;
  };

  [[nodiscard]] WorldMatrixEdit edit_world_matrix() { return WorldMatrixEdit(*this); }

  class LocalTRSEdit {
   public:
    float* pos_ptr() noexcept { return &pos.x; }
    float* scale_ptr() noexcept { return &scale.x; }
    float* rot_ptr() noexcept { return &rot.x; }

    void set_active(bool changed) noexcept { hint_changed_ = changed; }

    ~LocalTRSEdit();

    DELETE_COPY_MOVE(LocalTRSEdit);

    mEn::Vec3& pos;    // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    mEn::Quat& rot;    // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    mEn::Vec3& scale;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

   private:
    friend class Transform;

    explicit LocalTRSEdit(Transform& t)
        : pos(t.pos_), rot(t.rot_), scale(t.scale_), t_(t), pos0_(t.pos_), rot0_(t.rot_), scale0_(t.scale_) {}

    bool changed_approx() const noexcept { return pos != pos0_ || scale != scale0_ || rot != rot0_; }

    Transform& t_;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    mEn::Vec3 pos0_;
    mEn::Quat rot0_;
    mEn::Vec3 scale0_;
    std::optional<bool> hint_changed_;
  };

  [[nodiscard]] LocalTRSEdit edit_local_trs() { return LocalTRSEdit(*this); }

 private:
  void detach_from_parent_unsafe();

  Transform* parent_ = nullptr;
  std::vector<Transform*> children_;
  GameObject* owner_ = nullptr;

  mEn::Vec3 pos_{};
  mEn::Quat rot_{1, 0, 0, 0};
  mEn::Vec3 scale_{1, 1, 1};

  mutable bool dirty_          = true;
  mutable mEn::Mat4 model_mat_ = mEn::Mat4(1);

  mutable bool inverse_dirty_      = true;
  mutable mEn::Mat4 inv_model_mat_ = mEn::Mat4(1);
};

}  // namespace kEn
