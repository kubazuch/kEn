#pragma once

/**
 * @file component.hpp
 * @brief Abstract base class for all game-object components.
 * @ingroup ken
 */

#include <memory>

#include <kEn/core/assert.hpp>
#include <kEn/core/timestep.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/event/event.hpp>

namespace kEn {

class GameObject;
class Shader;

/**
 * @brief Abstract base for all components attached to a @ref GameObject.
 *
 * A component encapsulates one logical behaviour (rendering, input, physics,
 * etc.) and is owned exclusively by a @ref GameObject.  The owning object
 * calls @ref on_attach / @ref on_detach at the appropriate times via the
 * private @c attach_to / @c detach_from_parent helpers; components must not
 * manage their own lifetime.
 *
 * Subclasses override only the lifecycle hooks they need.  The event
 * dispatcher is available in @ref dispatcher_ to subscribe to engine events
 * from within @ref on_attach.
 *
 * @note Copy and move are deleted.  Components are always held through
 *       @c std::unique_ptr and transferred by moving the pointer.
 */
class GameComponent {
 public:
  GameComponent()          = default;
  virtual ~GameComponent() = default;

  /**
   * @brief Return a heap-allocated deep copy of this component.
   *
   * Used by @ref GameObject to duplicate a component when cloning a game
   * object.  The returned component is not yet attached to any object.
   */
  [[nodiscard]] virtual std::unique_ptr<GameComponent> clone() const = 0;

  /**
   * @brief Called immediately after this component is attached to its owner.
   *
   * Override to subscribe to events, allocate GPU resources, or cache
   * references to sibling components.  @ref parent() is valid when this is
   * called.
   */
  virtual void on_attach() {}

  /**
   * @brief Called immediately before this component is detached from its owner.
   *
   * Override to release resources or unsubscribe from events.  @ref parent()
   * is still valid when this is called.
   */
  virtual void on_detach() {}

  /**
   * @brief Fixed-timestep game logic update.
   *
   * @param delta  Time elapsed since the previous tick.
   * @param time   Total elapsed simulation time.
   */
  virtual void update(Timestep /*delta*/, Timestep /*time*/) {}

  /**
   * @brief Per-frame render callback.
   *
   * @param shader  The active shader for this render pass.
   * @param alpha   Interpolation factor in [0, 1] between the previous and
   *                current simulation tick.
   */
  virtual void render(Shader& /*shader*/, double /*alpha*/) {}

  /**
   * @brief Per-frame ImGui callback for debug/editor UI.
   */
  virtual void imgui() {}

  /**
   * @brief Dispatch @p event through the component's internal dispatcher.
   *
   * @return @c true if the event was handled and should stop propagating.
   */
  bool on_event(BaseEvent& event) { return dispatcher_.dispatch(event); }

  /**
   * @brief Return whether this component is currently attached to an owner.
   */
  [[nodiscard]] bool has_parent() const noexcept { return parent_ != nullptr; }

  /**
   * @brief Return the owning @ref GameObject.
   * @pre @ref has_parent() is @c true.
   */
  [[nodiscard]] const GameObject& parent() const {
    KEN_CORE_ASSERT(parent_ != nullptr);
    return *parent_;
  }

  /** @copydoc parent() const */
  [[nodiscard]] GameObject& parent() {
    KEN_CORE_ASSERT(parent_ != nullptr);
    return *parent_;
  }

  /**
   * @brief Return the world transform of the owning @ref GameObject.
   * @pre @ref has_parent() is @c true.
   */
  [[nodiscard]] const kEn::Transform& transform() const;

  /** @copydoc transform() const */
  [[nodiscard]] kEn::Transform& transform();

  DELETE_COPY_MOVE(GameComponent);

 protected:
  /** @brief Event dispatcher; subscribe to engine events from @ref on_attach. */
  kEn::EventDispatcher dispatcher_;

 private:
  void attach_to(GameObject& go);
  void detach_from_parent() noexcept;

  GameObject* parent_ = nullptr;

  friend GameObject;
};

}  // namespace kEn
