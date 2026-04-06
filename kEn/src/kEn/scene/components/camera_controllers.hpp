#pragma once

#include <mEn/vec2.hpp>

#include <kEn/core/timestep.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/scene/component.hpp>

/**
 * @file
 * @ingroup ken
 */

namespace kEn {

/**
 * @brief First-person mouse-look camera controller.
 *
 * Tracks mouse delta each tick and integrates it into yaw and pitch angles,
 * then sets the owning object's local rotation accordingly.  Mouse capture is
 * toggled with F (enable) and Escape (disable).
 *
 * The component subscribes to @ref WindowResizeEvent to keep the cursor
 * lock-point centred after window resizes.  Subscription is deferred to
 * @ref on_attach so that the component can be constructed without a live
 * application.
 */
class FreeLookComponent : public GameComponent {
 public:
  /**
   * @brief Construct with the given mouse sensitivity.
   *
   * @param sensitivity  Scale applied to raw mouse delta before converting to
   *                     radians.  Larger values produce faster rotation.
   */
  explicit FreeLookComponent(float sensitivity);

  /**
   * @brief Cache the window centre and subscribe to resize events.
   *
   * Queries @ref Application::main_window for the initial window dimensions.
   * Called by @ref GameObject after the component is attached.
   */
  void on_attach() override;

  void update(Timestep delta, Timestep time) override;
  [[nodiscard]] std::unique_ptr<GameComponent> clone() const override;

  /** @brief Return the current mouse sensitivity. */
  [[nodiscard]] float sensitivity() const { return sensitivity_; }

  /** @brief Set the mouse sensitivity. */
  void set_sensitivity(float sensitivity) { sensitivity_ = sensitivity; }

 private:
  /** @brief Update the cursor lock-point when the window is resized. */
  bool on_window_resize(const kEn::WindowResizeEvent& event);

  float sensitivity_;
  float pitch_ = 0, yaw_ = 0;
  bool update_ = false;
  mEn::Vec2 window_center_;
};

/**
 * @brief Keyboard-driven translation controller.
 *
 * Reads arrow keys, WASD, Space/E (up), and Shift/Q (down) each tick and
 * moves the owning object along the resulting direction.  Holding Left Control
 * triples the movement speed.
 *
 * When @p world_y is @c true the vertical axis is always world Y; when
 * @c false, up and down follow the object's local up direction.
 */
class FreeMoveComponent : public GameComponent {
 public:
  /**
   * @brief Construct with the given speed and vertical-axis mode.
   *
   * @param speed    Movement units per second.
   * @param world_y  When @c true, Space/E and Shift/Q move along world Y.
   *                 When @c false, they move along the object's local up.
   */
  explicit FreeMoveComponent(float speed, bool world_y = true) : speed_(speed), world_y_(world_y) {}

  void update(Timestep delta, Timestep time) override;
  [[nodiscard]] std::unique_ptr<GameComponent> clone() const override;

  /** @brief Return the movement speed in units per second. */
  [[nodiscard]] float speed() const { return speed_; }

  /** @brief Set the movement speed in units per second. */
  void set_speed(float speed) { speed_ = speed; }

  /** @brief Return whether vertical movement uses world Y (@c true) or local up (@c false). */
  [[nodiscard]] bool world_y() const { return world_y_; }

  /** @brief Set the vertical-axis mode. */
  void set_world_y(bool world_y) { world_y_ = world_y; }

 private:
  float speed_;
  bool world_y_;
};

/**
 * @brief Rotates the owning object to face a fixed target each tick.
 *
 * Calls @ref Transform::look_at with the target's world position every update.
 * The target is stored as a non-owning raw pointer and must outlive this
 * component.
 *
 * @note @c clone() returns a new component pointing at the same target object.
 *       The caller must ensure the target remains valid for the lifetime of
 *       both the original and the clone.
 */
class LookAtComponent : public GameComponent {
 public:
  /**
   * @brief Construct with the given look-at target.
   *
   * @param target  The object to track.  Must outlive this component.
   */
  explicit LookAtComponent(const GameObject& target) : target_(&target) {}

  /**
   * @brief Change the look-at target.
   *
   * @param target  The new object to track.  Must outlive this component.
   */
  void set_target(const GameObject& target) { target_ = &target; }

  /** @brief Return the current look-at target. */
  [[nodiscard]] const GameObject& target() const { return *target_; }

  void update(Timestep delta, Timestep time) override;
  [[nodiscard]] std::unique_ptr<GameComponent> clone() const override;

 private:
  const GameObject* target_;
};

}  // namespace kEn
