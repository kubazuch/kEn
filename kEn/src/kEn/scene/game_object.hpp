#pragma once

#include <concepts>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <kEn/core/core.hpp>
#include <kEn/core/timestep.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/scene/component.hpp>
#include <kEn/scene/id_registry.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief Scene node that owns a Transform, a list of GameComponents, and a
 *        parent-child hierarchy.
 *
 * A GameObject is the fundamental building block of a scene.  Each object has:
 * - A unique numeric ID registered in a process-wide registry for O(1) lookup.
 * - A Transform that tracks local and world-space position, rotation, and scale.
 *   The transform hierarchy mirrors the scene hierarchy: reparenting a
 *   GameObject also reparents its Transform, so child transforms inherit the
 *   parent's world matrix automatically.
 * - An ordered list of GameComponent instances.  Components receive forwarded
 *   render, update, imgui, and event callbacks each frame.
 *
 * Hierarchy rules mirror those of Transform: cycles and self-parenting are
 * rejected.  Both hierarchies are always kept in sync.
 *
 * The class is non-copyable and non-movable because children and components
 * hold back-references into the owning object.
 */
class GameObject {
 public:
  /** @brief Maximum number of simultaneously live GameObjects. */
  constexpr static size_t kMaxGameObjects = 1U << 13;

  /** @name Construction */
  /** @{ */

  /**
   * @brief Constructs a GameObject with the given local TRS and registers it
   *        in the global lookup registry.
   * @param pos   Local position (default: origin).
   * @param rot   Local rotation as a unit quaternion (default: identity).
   * @param scale Local scale (default: (1,1,1)).
   * @param name  Display name used for logging and editor UI (default:
   *              "GameObject").
   */
  explicit GameObject(mEn::Vec3 pos = mEn::Vec3(), mEn::Quat rot = {1, 0, 0, 0}, mEn::Vec3 scale = {1, 1, 1},
                      std::string_view name = "GameObject");

  /**
   * @brief Destructor.  Calls on_detach() on all components, detaches all
   *        children (they become roots), and removes this object from its
   *        parent's child list.
   */
  ~GameObject();

  /** @} */

  /** @name Hierarchy */
  /** @{ */

  /**
   * @brief Returns true if @p child can be added without creating a cycle or
   *        self-parenting.
   */
  [[nodiscard]] bool can_add_child(const GameObject& child) const noexcept;

  /**
   * @brief Attempts to add @p child.
   * @return true on success, false if the operation would create a cycle or
   *         self-parenting.  If @p child already has a parent, it is reparented.
   */
  bool try_add_child(GameObject& child);

  /**
   * @brief Adds @p child unconditionally.
   * @pre can_add_child(child) == true (asserted in debug builds).
   *      If @p child already has a parent, it is reparented.
   * @return *this, for call chaining.
   */
  GameObject& add_child(GameObject& child);

  /**
   * @brief Adds multiple children, equivalent to calling add_child for each.
   * @return *this, for call chaining.
   */
  template <std::same_as<GameObject>... Ts>
  GameObject& add_children(Ts&... children) {
    (add_child(children), ...);
    return *this;
  }

  /**
   * @brief Removes @p child from this object's child list.
   * @pre @p child is a direct child of *this (asserted in debug builds).
   */
  void remove_child(GameObject& child);

  /**
   * @brief Attempts to set @p parent as this object's parent.
   * @return true on success, false if the operation would create a cycle or
   *         self-parenting.  If this object already has a parent, it is
   *         reparented.
   */
  bool try_set_parent(GameObject& parent);

  /**
   * @brief Sets @p parent unconditionally.
   * @pre parent.can_add_child(*this) == true (asserted in debug builds).
   *      If this object already has a parent, it is reparented.
   */
  void set_parent(GameObject& parent);

  /**
   * @brief Removes the current parent, making this object a root.
   *        No-op if already a root.
   */
  void unset_parent();

  /** @} */

  /** @name Components */
  /** @{ */

  /**
   * @brief Constructs a component of type @p T in-place and attaches it.
   *
   * Forwards @p args to T's constructor, wraps the result in a unique_ptr,
   * and transfers ownership to the internal component list.  Calls
   * T::on_attach() after successful insertion; rolls back ownership on
   * exception.
   *
   * @tparam T    Component type; must derive from GameComponent.
   * @tparam Args Constructor argument types.
   * @param  args Arguments forwarded to T's constructor.
   * @return      Reference to the newly attached component.
   */
  template <std::derived_from<GameComponent> T, typename... Args>
  T& emplace_component(Args&&... args) {
    return static_cast<T&>(add_component(std::make_unique<T>(std::forward<Args>(args)...)));
  }

  /**
   * @brief Attaches multiple pre-built components in one call.
   *
   * Each argument must be a std::unique_ptr<GameComponent>.
   *
   * @return *this, for call chaining.
   */
  template <typename... Args>
  GameObject& add_components(Args&&... components) {
    (add_component(std::forward<Args>(components)), ...);
    return *this;
  }

  /** @} */

  /** @name Identity */
  /** @{ */

  /** @brief Returns a typed ID view for this object. */
  [[nodiscard]] IdView<GameObject> id() const { return id_; }
  /** @brief Returns the raw numeric ID. */
  [[nodiscard]] uint32_t raw_id() const { return id_.raw_id(); }
  /** @brief Returns the display name. */
  [[nodiscard]] std::string_view name() const { return name_; }
  /** @brief Sets the display name. */
  void set_name(std::string_view name) { name_ = name; }

  /** @} */

  /** @name Frame phases */
  /** @{ */

  /**
   * @brief Renders all components, then optionally recurses into children.
   * @param shader    Shader program to use for this draw call.
   * @param alpha     Interpolation factor between the previous and current
   *                  simulation tick, used for smooth rendering.
   * @param recursive If true, recurses into child objects (default: true).
   */
  void render(Shader& shader, double alpha, bool recursive = true) const;

  /**
   * @brief Runs the ImGui widgets for all components, then optionally recurses.
   * @param recursive If true, recurses into child objects (default: true).
   */
  void imgui(bool recursive = true);

  /**
   * @brief Updates all components, then optionally recurses into children.
   * @param delta     Time elapsed since the last update tick.
   * @param time      Total elapsed simulation time.
   * @param recursive If true, recurses into child objects (default: true).
   */
  void update(Timestep delta, Timestep time, bool recursive = true);

  /**
   * @brief Dispatches @p event to components and optionally to children.
   *
   * Components are visited in insertion order; children are visited after all
   * components of the current object.  Propagation stops as soon as
   * event.handled is set to true.
   *
   * @param event     Event to dispatch.
   * @param recursive If true, recurses into child objects (default: true).
   */
  void on_event(BaseEvent& event, bool recursive = true);

  /** @} */

  /** @name Transform */
  /** @{ */

  /** @brief Returns the Transform owned by this object. */
  [[nodiscard]] kEn::Transform& transform() { return transform_; }
  /** @copydoc transform() */
  [[nodiscard]] const kEn::Transform& transform() const { return transform_; }

  /** @} */

  /** @name Registry lookup */
  /** @{ */

  /**
   * @brief Looks up a live GameObject by typed ID.
   * @return Pointer to the object, or nullptr if the ID is no longer valid.
   */
  [[nodiscard]] static GameObject* find_by_id(const IdView<GameObject>& id) {
    return game_object_registry_.get(id.handle());
  }

  /**
   * @brief Looks up a live GameObject by raw numeric index.
   * @return Pointer to the object, or nullptr if the index is unoccupied.
   */
  [[nodiscard]] static GameObject* find_by_id(uint32_t index) {
    const auto view = game_object_registry_.from_raw_id(index);
    return view ? view->get() : nullptr;
  }

  /** @} */

  DELETE_COPY_MOVE(GameObject);

 private:
  void on_transform_changed();
  GameComponent& add_component(std::unique_ptr<GameComponent> to_add);

  static Registry<GameObject> game_object_registry_;

  kEn::Transform transform_;

  Id<GameObject> id_;
  std::string name_;
  GameObject* parent_ = nullptr;
  std::vector<GameObject*> children_;
  std::vector<std::unique_ptr<GameComponent>> components_;
};

}  // namespace kEn
