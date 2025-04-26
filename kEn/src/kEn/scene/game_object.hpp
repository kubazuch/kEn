#pragma once
#include <cstddef>
#include <kEn/core/transform.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/scene/component.hpp>
#include <kEn/scene/id_registry.hpp>
#include <optional>
#include <string>
#include <string_view>

namespace kEn {
using duration_t   = std::chrono::nanoseconds;
using GameObjectId = Id<GameObject>;

class GameObject {
 public:
  constexpr static size_t kMaxGameObjects = 100;
  static IdRegistry<GameObject> game_object_registry_;

  explicit GameObject(mEn::Vec3 pos = mEn::Vec3(), mEn::Quat rot = {1, 0, 0, 0}, mEn::Vec3 scale = {1, 1, 1},
                      std::string_view name = "GameObject");
  ~GameObject();

  GameObject& add_child(GameObject& child);
  GameObject& add_children(std::initializer_list<std::reference_wrapper<GameObject>> children);
  GameObject& add_component(std::shared_ptr<GameComponent> to_add);
  GameObject& add_components(std::initializer_list<std::shared_ptr<GameComponent>> components);

  inline IdView<GameObjectId> id() const { return id_; }
  inline size_t raw_id() const { return id_.raw_id(); }
  inline std::string_view name() const { return name_; }
  inline void set_name(std::string_view name) { name_ = name; }

  void render(Shader& shader, double alpha) const;
  void render_all(Shader& shader, double alpha) const;
  void imgui();
  void imgui_all();
  void update(duration_t delta, duration_t time);
  void update_all(duration_t delta, duration_t time);
  void on_event(BaseEvent& event);

  kEn::Transform& transform() { return transform_; }
  const kEn::Transform& transform() const { return transform_; }

 protected:
  kEn::Transform transform_;

 public:
  bool torus = false;
  bool point = false;
  bool line = false;

 private:
  GameObjectId id_;
  std::string name_;
  std::optional<std::reference_wrapper<GameObject>> parent_;
  std::vector<std::reference_wrapper<GameObject>> children_;
  std::vector<std::shared_ptr<GameComponent>> components_;
};

}  // namespace kEn
