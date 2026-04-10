/**
 * @file camera.hpp
 * @brief Camera components for orthographic and perspective projection.
 */
#pragma once

#include <mEn/fwd.hpp>
#include <mEn/vec3.hpp>

#include <kEn/event/application_events.hpp>
#include <kEn/scene/component.hpp>

namespace kEn {

/**
 * @brief Abstract base camera component.
 *
 * Stores a projection matrix and derives the view matrix from the owning
 * object's world-to-local transform. Subclasses must implement
 * @ref on_window_resize to recompute the projection when the window is resized.
 */
class Camera : public GameComponent {
 public:
  /** @brief Constructs a camera with the given projection matrix. */
  explicit Camera(mEn::Mat4 projection_matrix) : projection_matrix_(std::move(projection_matrix)) {}
  ~Camera() override = default;

  /** @brief Returns the projection matrix. */
  [[nodiscard]] mEn::Mat4 projection_matrix() const { return projection_matrix_; }

  /** @brief Returns the view matrix derived from the owning transform's world-to-local matrix. */
  [[nodiscard]] mEn::Mat4 view_matrix() const { return transform().world_to_local_matrix(); }

  /** @brief Returns the combined view-projection matrix (projection * view). */
  [[nodiscard]] mEn::Mat4 view_projection_matrix() const { return projection_matrix_ * view_matrix(); }

  [[nodiscard]] std::unique_ptr<GameComponent> clone() const override = 0;

 protected:
  /** @brief Replaces the stored projection matrix. */
  void set_projection(mEn::Mat4 projection_matrix) { projection_matrix_ = std::move(projection_matrix); }

 private:
  /**
   * @brief Handles window resize events to recompute the projection.
   * @param event The resize event.
   * @return @c false to allow further event propagation.
   */
  virtual bool on_window_resize(const WindowResizeEvent& event) = 0;

  mEn::Mat4 projection_matrix_;
};

/**
 * @brief Orthographic camera component.
 *
 * Uses an orthographic projection with fixed near/far planes of -1 and 1.
 * On window resize the left/right bounds are adjusted to maintain the new
 * aspect ratio while the bottom/top bounds are kept unchanged.
 */
class OrthographicCamera : public Camera {
 public:
  /**
   * @brief Constructs an orthographic camera.
   * @param left   Left clipping plane coordinate.
   * @param right  Right clipping plane coordinate.
   * @param bottom Bottom clipping plane coordinate.
   * @param top    Top clipping plane coordinate.
   */
  OrthographicCamera(float left, float right, float bottom, float top);

  /**
   * @brief Updates the orthographic projection bounds.
   * @param left   Left clipping plane coordinate.
   * @param right  Right clipping plane coordinate.
   * @param bottom Bottom clipping plane coordinate.
   * @param top    Top clipping plane coordinate.
   */
  void set_projection(float left, float right, float bottom, float top);

  [[nodiscard]] std::unique_ptr<GameComponent> clone() const override;

 private:
  bool on_window_resize(const WindowResizeEvent& event) override;

  float left_, right_, bottom_, top_;
};

/**
 * @brief Perspective camera component.
 *
 * Uses a perspective projection. On window resize the aspect ratio is
 * recomputed automatically while the field of view and clip planes are kept.
 */
class PerspectiveCamera : public Camera {
 public:
  /**
   * @brief Constructs a perspective camera.
   * @param fov    Vertical field of view in radians.
   * @param aspect Aspect ratio (width / height).
   * @param z_near Near clipping plane distance.
   * @param z_far  Far clipping plane distance.
   */
  PerspectiveCamera(float fov, float aspect, float z_near, float z_far);

  /**
   * @brief Updates the perspective projection parameters.
   * @param fov    Vertical field of view in radians.
   * @param aspect Aspect ratio (width / height).
   * @param z_near Near clipping plane distance.
   * @param z_far  Far clipping plane distance.
   */
  void set_projection(float fov, float aspect, float z_near, float z_far);

  [[nodiscard]] std::unique_ptr<GameComponent> clone() const override;

 private:
  bool on_window_resize(const WindowResizeEvent& event) override;

  float fov_, aspect_, z_near_, z_far_;
};

/**
 * @brief Camera component that can switch between perspective and orthographic projection.
 *
 * In perspective mode the projection is a standard @ref mEn::perspective matrix.
 * In orthographic mode the half-extents are derived from the camera's distance to
 * @ref focal_target() and the field of view, so that switching modes preserves the
 * apparent size of geometry at the focal distance.
 *
 * Call @ref recalculate_projection() after moving the camera while in
 * orthographic mode to keep the bounds in sync with the new position.
 */
class DynamicCamera : public Camera {
 public:
  /**
   * @brief Constructs a dynamic camera.
   * @param orthographic  Initial projection mode.
   * @param fov           Vertical field of view in radians (used in both modes).
   * @param aspect        Aspect ratio (width / height).
   * @param z_near        Near clipping plane distance.
   * @param z_far         Far clipping plane distance.
   */
  DynamicCamera(bool orthographic, float fov, float aspect, float z_near, float z_far);

  /** @brief Returns @c true when the camera is in orthographic mode. */
  [[nodiscard]] bool is_orthographic() const { return is_orthographic_; }
  /** @brief Returns the vertical field of view in radians. */
  [[nodiscard]] float fov() const { return fov_; }
  /** @brief Returns the aspect ratio. */
  [[nodiscard]] float aspect() const { return aspect_; }
  /** @brief Returns the near clipping plane distance. */
  [[nodiscard]] float z_near() const { return z_near_; }
  /** @brief Returns the far clipping plane distance. */
  [[nodiscard]] float z_far() const { return z_far_; }
  /**
   * @brief Returns the focal target used to compute the orthographic half-extents.
   *
   * The focal length is the distance from the camera's world position to this
   * point.  Defaults to the world origin.
   */
  [[nodiscard]] const mEn::Vec3& focal_target() const { return focal_target_; }
  /**
   * @brief Sets the focal target and rebuilds the projection when in orthographic mode.
   * @param target  World-space point the camera orbits / looks at.
   */
  void set_focal_target(const mEn::Vec3& target);

  /** @brief Switches between orthographic and perspective projection and rebuilds the matrix. */
  void set_orthographic(bool orthographic);
  /** @brief Sets the vertical field of view (radians) and rebuilds the matrix. */
  void set_fov(float fov);
  /** @brief Sets the aspect ratio and rebuilds the matrix. */
  void set_aspect(float aspect);
  /** @brief Sets the near clipping plane distance and rebuilds the matrix. */
  void set_z_near(float z_near);
  /** @brief Sets the far clipping plane distance and rebuilds the matrix. */
  void set_z_far(float z_far);

  /**
   * @brief Recomputes the projection matrix from the current parameters.
   *
   * In orthographic mode the half-extents are derived from the camera's
   * distance to the world origin, so this should be called after the camera
   * is moved.
   */
  void recalculate_projection();

  [[nodiscard]] std::unique_ptr<GameComponent> clone() const override;

 private:
  bool on_window_resize(const WindowResizeEvent& event) override;

  bool is_orthographic_;
  float fov_, aspect_, z_near_, z_far_;
  mEn::Vec3 focal_target_{0.0F, 0.0F, 0.0F};
};

}  // namespace kEn
