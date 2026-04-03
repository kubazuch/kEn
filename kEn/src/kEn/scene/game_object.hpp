#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

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

  GameObject& add_child(GameObject& child);
  GameObject& add_children(std::initializer_list<std::reference_wrapper<GameObject>> children);
  GameObject& add_component(std::shared_ptr<GameComponent> to_add);
  GameObject& add_components(std::initializer_list<std::shared_ptr<GameComponent>> components);

  [[nodiscard]] IdView<GameObject> id() const { return id_; }
  [[nodiscard]] uint32_t raw_id() const { return id_.raw_id(); }
  [[nodiscard]] std::string_view name() const { return name_; }
  void set_name(std::string_view name) { name_ = name; }

  void render(Shader& shader, double alpha) const;
  void render_all(Shader& shader, double alpha) const;
  void imgui();
  void imgui_all();
  void update(Timestep delta, Timestep time);
  void update_all(Timestep delta, Timestep time);
  void on_event(BaseEvent& event);

  [[nodiscard]] kEn::Transform& transform() { return transform_; }
  [[nodiscard]] const kEn::Transform& transform() const { return transform_; }

  [[nodiscard]] static GameObject* find_by_id(const IdView<GameObject>& id) {
    return game_object_registry_.get(id.handle());
  }

  [[nodiscard]] static GameObject* find_by_id(uint32_t index) {
    const auto view = game_object_registry_.from_raw_id(index);
    return view ? view->get() : nullptr;
  }

 private:
  void on_transform_changed();

 protected:
  kEn::Transform transform_;

 private:
  static Registry<GameObject> game_object_registry_;

  Id<GameObject> id_;
  std::string name_;
  std::optional<std::reference_wrapper<GameObject>> parent_;
  std::vector<std::reference_wrapper<GameObject>> children_;
  std::vector<std::shared_ptr<GameComponent>> components_;

  friend class Transform;
};

}  // namespace kEn
