#pragma once

#include <functional>
#include <optional>
#include <span>
#include <unordered_map>
#include <vector>

#include <mEn/features/mat_decompose.hpp>
#include <mEn/features/type_ptr.hpp>

#include <kEn/core/assert.hpp>
#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief Hierarchical 3D transform with dirty-flag world-matrix caching.
 *
 * Each Transform stores local position, rotation (unit quaternion), and scale
 * relative to its parent. World-space matrices are computed lazily and cached;
 * any mutation that changes the local TRS calls set_dirty(), which propagates
 * the invalidation down the child chain and notifies the owning GameObject.
 *
 * The rotation is kept normalized at all times. Non-uniform scale from a
 * parent is handled correctly for position and scale but may introduce shear
 * into the world matrix; set_world_rot() does not account for that shear.
 *
 * The class is non-copyable and non-movable (children hold raw pointers back
 * to their parent).
 */
class Transform {
 public:
  /**
   * @brief Constructs a Transform with the given local TRS.
   * @param pos   Local position (default: origin).
   * @param rot   Local rotation as a unit quaternion (default: identity).
   * @param scale Local scale (default: (1,1,1)).
   *
   * The rotation is normalized on construction.
   */
  explicit Transform(mEn::Vec3 pos = mEn::Vec3(), mEn::Quat rot = {1, 0, 0, 0}, mEn::Vec3 scale = {1, 1, 1});

  /**
   * @brief Destructor.  Detaches all children (they become root transforms)
   *        and removes this transform from its parent's child list.
   */
  ~Transform();

  /** @name Parenting */
  /** @{ */

  /** @brief Returns true if this transform has a parent. */
  [[nodiscard]] bool has_parent() const noexcept { return parent_ != nullptr; }

  /** @brief Returns a pointer to the parent, or nullptr if none. */
  [[nodiscard]] Transform* parent() noexcept { return parent_; }
  /** @copydoc parent() */
  [[nodiscard]] const Transform* parent() const noexcept { return parent_; }

  /**
   * @brief Returns a reference to the parent.
   * @pre has_parent() == true
   */
  [[nodiscard]] Transform& get_parent() {
    KEN_CORE_ASSERT(has_parent(), "Transform has no parent");
    return *parent_;
  }
  /** @copydoc get_parent() */
  [[nodiscard]] const Transform& get_parent() const {
    KEN_CORE_ASSERT(has_parent(), "Transform has no parent");
    return *parent_;
  }

  /**
   * @brief Returns true if @p parent can be set without creating a cycle or
   *        self-parenting.
   */
  [[nodiscard]] bool can_set_parent(const Transform& parent) const noexcept;

  /**
   * @brief Attempts to set @p parent.
   * @return true on success, false if the operation would create a cycle or
   *         self-parenting.  A no-op (returns true) when @p parent is already
   *         the current parent.
   */
  bool try_set_parent(Transform& parent);

  /**
   * @brief Sets @p parent unconditionally.
   * @pre can_set_parent(parent) == true  (asserted in debug builds)
   */
  void set_parent(Transform& parent);

  /**
   * @brief Removes the current parent, making this transform a root.
   *        No-op if already a root.  Marks the transform dirty.
   */
  void unset_parent();

  /** @brief Returns a read-only view of the direct children. */
  [[nodiscard]] std::span<Transform* const> children() const noexcept;
  /** @brief Returns a mutable view of the direct children. */
  [[nodiscard]] std::span<Transform*> children() noexcept;

  /** @} */

  /** @name Change notification */
  /** @{ */

  /**
   * @brief Registers a callback invoked whenever set_dirty() propagates to
   *        this transform.  Replaces any existing callback registered under
   *        the same @p key.
   * @param key      Unique subscriber identity (typically @c this of the caller).
   * @param callback Callable invoked on each dirty propagation.
   */
  void subscribe_on_changed(const void* key, std::function<void()> callback);
  /**
   * @brief Removes the callback registered under @p key.  No-op if @p key
   *        was never subscribed.
   */
  void unsubscribe_on_changed(const void* key) noexcept;

  /** @} */

  /** @name Matrices */
  /** @{ */

  /**
   * @brief Computes the local-to-parent matrix (TRS) each call; not cached.
   * @return T * R * S matrix transforming points from local space to parent
   *         space (or world space if this is a root transform).
   */
  [[nodiscard]] mEn::Mat4 local_to_parent_matrix() const;

  /**
   * @brief Computes the parent-to-local matrix each call; not cached.
   * @return Inverse of local_to_parent_matrix(), assuming no shear.
   */
  [[nodiscard]] mEn::Mat4 parent_to_local_matrix() const;

  /**
   * @brief Returns the cached local-to-world matrix, recomputing it if dirty.
   * @note The returned reference is invalidated by any call that marks the
   *       transform dirty (set_dirty(), set_local_pos(), etc.).
   */
  [[nodiscard]] const mEn::Mat4& local_to_world_matrix() const;

  /**
   * @brief Returns the cached world-to-local matrix, recomputing it if dirty.
   * @note Same invalidation rules as local_to_world_matrix().
   */
  [[nodiscard]] const mEn::Mat4& world_to_local_matrix() const;

  /**
   * @brief Marks this transform and all descendants as dirty and notifies the
   *        owning GameObject.  Idempotent: does nothing if already dirty.
   */
  void set_dirty();

  /** @} */

  /** @name Basic operations */
  /** @{ */

  /** @brief Translates by @p delta in local space (adds to local_pos()). */
  void translate_local(const mEn::Vec3& delta);

  /** @brief Translates by @p delta in world space. */
  void translate_world(const mEn::Vec3& delta);

  /**
   * @brief Rotates around @p axis by @p angle radians in world space.
   * @pre length(axis) > epsilon
   */
  void rotate(const mEn::Vec3& axis, float angle);

  /**
   * @brief Pre-multiplies the local rotation by @p rotation (world-space
   *        rotation applied on top of the current orientation).
   * @pre length(rotation) > epsilon
   */
  void rotate(const mEn::Quat& rotation);

  /**
   * @brief Post-multiplies the local rotation by @p rotation (local-space
   *        rotation applied after the current orientation).
   * @pre length(rotation) > epsilon
   */
  void rotate_local(const mEn::Quat& rotation);

  /**
   * @brief Orients the transform so that its local -Z axis points toward
   *        @p point in world space.
   *
   * Operates entirely in local/parent space: the target point and @p up vector
   * are converted to parent space before computing the quaternion, so the
   * result is stored as a local rotation.  No-ops when @p point coincides with
   * the current position or when the direction is already within tolerance.
   * Falls back to a safe up vector when @p up is nearly parallel to the
   * computed direction.
   *
   * @param point World-space target position.
   * @param up    World-space up hint (default: world Y).
   */
  void look_at(const mEn::Vec3& point, const mEn::Vec3& up = mEn::Vec3(0, 1, 0));

  /** @brief Resets local TRS to identity (origin, no rotation, unit scale). */
  void reset();

  /**
   * @brief Sets all three local TRS components atomically (single set_dirty()).
   * @param pos   New local position.
   * @param rot   New local rotation (normalized on assignment).
   * @param scale New local scale.
   */
  void set_local_transform(const mEn::Vec3& pos, const mEn::Quat& rot, const mEn::Vec3& scale);

  /** @} */

  /** @name Local-space access */
  /** @{ */

  /** @brief Returns the local position. */
  [[nodiscard]] const mEn::Vec3& local_pos() const noexcept { return pos_; }
  /** @brief Sets the local position and marks dirty. */
  void set_local_pos(const mEn::Vec3& pos);

  /** @brief Returns the local rotation (always a unit quaternion). */
  [[nodiscard]] const mEn::Quat& local_rot() const noexcept { return rot_; }
  /** @brief Sets the local rotation (normalized) and marks dirty. */
  void set_local_rot(const mEn::Quat& rot);

  /** @brief Returns the local scale. */
  [[nodiscard]] const mEn::Vec3& local_scale() const noexcept { return scale_; }
  /** @brief Sets the local scale and marks dirty. */
  void set_local_scale(const mEn::Vec3& scale);
  /** @brief Sets a uniform local scale and marks dirty. */
  void set_local_scale(float uniform_scale);

  /** @} */

  /** @name World-space derived values */
  /** @{ */

  /**
   * @brief Returns the world-space position.
   * Computed by transforming local_pos() through the parent's local-to-world
   * matrix.  O(depth) when the parent chain is dirty.
   */
  [[nodiscard]] mEn::Vec3 world_pos() const;
  /** @brief Sets the world-space position by converting to local space. */
  void set_world_pos(const mEn::Vec3& world_pos);

  /**
   * @brief Returns the world-space rotation as a unit quaternion.
   * Accumulated by multiplying parent world rotations up the hierarchy.
   */
  [[nodiscard]] mEn::Quat world_rot() const;
  /**
   * @brief Sets the world-space rotation by converting to local space.
   * @note Does not account for shear introduced by non-uniform parent scale.
   */
  void set_world_rot(const mEn::Quat& world_rot);

  /**
   * @brief Returns the world-space scale extracted from the world matrix
   *        column lengths.
   */
  [[nodiscard]] mEn::Vec3 world_scale() const;
  /**
   * @brief Sets the world-space scale by dividing by the parent world scale.
   * @pre All parent world scale components are non-zero (asserted in debug).
   */
  void set_world_scale(const mEn::Vec3& world_scale);
  /** @brief Sets a uniform world-space scale. */
  void set_world_scale(float uniform_scale);

  /** @brief Returns the local -Z axis in local space (forward direction). */
  [[nodiscard]] mEn::Vec3 local_front() const;
  /** @brief Returns the local +X axis in local space (right direction). */
  [[nodiscard]] mEn::Vec3 local_right() const;
  /** @brief Returns the local +Y axis in local space (up direction). */
  [[nodiscard]] mEn::Vec3 local_up() const;

  /** @brief Returns the local -Z axis transformed to world space. */
  [[nodiscard]] mEn::Vec3 world_front() const;
  /** @brief Returns the local +X axis transformed to world space. */
  [[nodiscard]] mEn::Vec3 world_right() const;
  /** @brief Returns the local +Y axis transformed to world space. */
  [[nodiscard]] mEn::Vec3 world_up() const;

  /** @} */

  DELETE_COPY_MOVE(Transform);

 public:
  /**
   * @brief RAII handle for editing the world matrix directly (e.g. via a gizmo).
   *
   * On construction the current world matrix is captured and decomposed into
   * T, R, S.  The caller can obtain a writable reference to the matrix via
   * matrix() / ptr() and modify it in-place.  On destruction, if a change is
   * detected (or set_active(true) was called), the new matrix is decomposed
   * and written back as world-space TRS via set_world_pos/rot/scale().
   *
   * set_active() is an optional hint to skip the decompose-based change
   * detection: pass true to force a commit, false to skip it entirely.
   *
   * Obtain via Transform::edit_world_matrix().
   */
  class WorldMatrixEdit {
   public:
    /** @brief Returns a mutable reference to the world matrix being edited. */
    mEn::Mat4& matrix() noexcept { return world_; }
    /** @brief Returns a raw float pointer to the world matrix (for OpenGL / a gizmo). */
    float* ptr() noexcept { return mEn::value_ptr(world_); }

    /**
     * @brief Provides a hint to skip expensive change detection on destruction.
     * @param using_gizmo  true  => always commit;
     *                     false => never commit (skip all work).
     * If not called, change detection is performed automatically.
     */
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

  /**
   * @brief Creates a WorldMatrixEdit scoped to this transform.
   * @return A RAII edit handle; commit happens on destruction.
   */
  [[nodiscard]] WorldMatrixEdit edit_world_matrix() { return WorldMatrixEdit(*this); }

  /**
   * @brief RAII handle for editing local TRS components directly (e.g. via
   *        ImGui drag widgets).
   *
   * Exposes direct references to the internal pos_, rot_, and scale_ fields.
   * On destruction, if the values have changed (or set_active(true) was
   * called), the rotation is normalized and set_dirty() is called.
   *
   * Obtain via Transform::edit_local_trs().
   */
  class LocalTRSEdit {
   public:
    /** @brief Returns a raw float pointer to pos (for ImGui::DragFloat3). */
    float* pos_ptr() noexcept { return &pos.x; }
    /** @brief Returns a raw float pointer to scale (for ImGui::DragFloat3). */
    float* scale_ptr() noexcept { return &scale.x; }
    /** @brief Returns a raw float pointer to rot.x (for ImGui::DragFloat4). */
    float* rot_ptr() noexcept { return &rot.x; }

    /**
     * @brief Provides a hint to skip component-wise change detection.
     * @param changed  true  => treat as changed and commit on destruction;
     *                 false => treat as unchanged and skip commit.
     */
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

  /**
   * @brief Creates a LocalTRSEdit scoped to this transform.
   * @return A RAII edit handle; dirty propagation happens on destruction.
   */
  [[nodiscard]] LocalTRSEdit edit_local_trs() { return LocalTRSEdit(*this); }

 private:
  void detach_from_parent_unsafe();

  Transform* parent_ = nullptr;
  std::vector<Transform*> children_;
  std::unordered_map<const void*, std::function<void()>> on_changed_reactors_;

  mEn::Vec3 pos_{};
  mEn::Quat rot_{1, 0, 0, 0};
  mEn::Vec3 scale_{1, 1, 1};

  mutable bool dirty_          = true;
  mutable mEn::Mat4 model_mat_ = mEn::Mat4(1);

  mutable bool inverse_dirty_      = true;
  mutable mEn::Mat4 inv_model_mat_ = mEn::Mat4(1);
};

}  // namespace kEn
