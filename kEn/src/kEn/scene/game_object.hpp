#pragma once

#include <concepts>
#include <cstddef>
#include <functional>
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

namespace kEn {

class GameObject {
 public:
  constexpr static size_t kMaxGameObjects = 1U << 13;

  explicit GameObject(mEn::Vec3 pos = mEn::Vec3(), mEn::Quat rot = {1, 0, 0, 0}, mEn::Vec3 scale = {1, 1, 1},
                      std::string_view name = "GameObject");
  ~GameObject();

  [[nodiscard]] bool can_add_child(const GameObject& child) const noexcept;

  bool try_add_child(GameObject& child);

  GameObject& add_child(GameObject& child);

  GameObject& add_children(std::initializer_list<std::reference_wrapper<GameObject>> children);

  GameObject& add_component(std::shared_ptr<GameComponent> to_add);
  GameObject& add_components(std::initializer_list<std::shared_ptr<GameComponent>> components);
  void remove_child(GameObject& child);

  bool try_set_parent(GameObject& parent);

  void set_parent(GameObject& parent);

  void unset_parent();

  template <std::derived_from<GameComponent> T, typename... Args>
  T& emplace_component(Args&&... args) {
    return static_cast<T&>(add_component(std::make_unique<T>(std::forward<Args>(args)...)));
  }

  template <typename... Args>
  GameObject& add_components(Args&&... components) {
    (add_component(std::forward<Args>(components)), ...);
    return *this;
  }

  [[nodiscard]] IdView<GameObject> id() const { return id_; }
  [[nodiscard]] uint32_t raw_id() const { return id_.raw_id(); }
  [[nodiscard]] std::string_view name() const { return name_; }
  void set_name(std::string_view name) { name_ = name; }

  void render(Shader& shader, double alpha, bool recursive = true) const;
  void imgui(bool recursive = true);
  void update(Timestep delta, Timestep time, bool recursive = true);
  void on_event(BaseEvent& event, bool recursive = true);

  [[nodiscard]] kEn::Transform& transform() { return transform_; }
  [[nodiscard]] const kEn::Transform& transform() const { return transform_; }

  [[nodiscard]] static GameObject* find_by_id(const IdView<GameObject>& id) {
    return game_object_registry_.get(id.handle());
  }

  [[nodiscard]] static GameObject* find_by_id(uint32_t index) {
    const auto view = game_object_registry_.from_raw_id(index);
    return view ? view->get() : nullptr;
  }

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
