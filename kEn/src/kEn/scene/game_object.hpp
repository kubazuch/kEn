#pragma once
#include <kEn/core/transform.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/scene/component.hpp>
#include <optional>

namespace kEn {
using duration_t = std::chrono::nanoseconds;

class GameObject {
 public:
  explicit GameObject(mEn::Vec3 pos = mEn::Vec3(), mEn::Quat rot = {1, 0, 0, 0}, mEn::Vec3 scale = {1, 1, 1});
  ~GameObject();

  GameObject& add_child(GameObject& child);
  GameObject& add_children(std::initializer_list<std::reference_wrapper<GameObject>> children);
  GameObject& add_component(std::shared_ptr<GameComponent> to_add);
  GameObject& add_components(std::initializer_list<std::shared_ptr<GameComponent>> components);

  void render(Shader& shader, double alpha) const;
  void render_all(Shader& shader, double alpha) const;
  void update(duration_t delta, duration_t time);
  void update_all(duration_t delta, duration_t time);
  void on_event(BaseEvent& event);

  kEn::Transform& transform() { return transform_; }
  const kEn::Transform& transform() const { return transform_; }

 protected:
  kEn::Transform transform_;

 private:
  std::optional<std::reference_wrapper<GameObject>> parent_;
  std::vector<std::reference_wrapper<GameObject>> children_;
  std::vector<std::shared_ptr<GameComponent>> components_;
};

}  // namespace kEn
