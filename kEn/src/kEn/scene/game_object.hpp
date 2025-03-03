#pragma once
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/scene/component.hpp>
#include <optional>

namespace kEn {

class GameObject {
 public:
  explicit GameObject(glm::vec3 pos = glm::vec3(), glm::quat rot = {1, 0, 0, 0}, glm::vec3 scale = {1, 1, 1});
  ~GameObject();

  GameObject& add_child(GameObject& child);
  GameObject& add_children(std::initializer_list<std::reference_wrapper<GameObject>> children);
  GameObject& add_component(std::shared_ptr<GameComponent> to_add);
  GameObject& add_components(std::initializer_list<std::shared_ptr<GameComponent>> components);

  void render(Shader& shader) const;
  void render_all(Shader& shader) const;
  void update(double delta);
  void update_all(double delta);
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
