/**
 * @file camera.hpp
 * @brief Camera components for orthographic and perspective projection.
 */
#pragma once

#include <mEn/fwd.hpp>

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

}  // namespace kEn
