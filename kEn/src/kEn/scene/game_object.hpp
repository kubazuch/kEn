#pragma once
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/scene/component.hpp>
#include <optional>

namespace kEn {

class game_object {
 public:
  game_object(glm::vec3 pos = glm::vec3(), glm::quat rot = {1, 0, 0, 0}, glm::vec3 scale = {1, 1, 1});
  ~game_object();

  game_object& add_child(game_object& child);
  game_object& add_children(std::initializer_list<std::reference_wrapper<game_object>> children);
  game_object& add_component(std::shared_ptr<game_component> to_add);
  game_object& add_components(std::initializer_list<std::shared_ptr<game_component>> components);

  void render(shader& shader) const;
  void render_all(shader& shader) const;
  void update(double delta);
  void update_all(double delta);
  void on_event(base_event& event);

  kEn::transform& transform() { return transform_; }
  const kEn::transform& transform() const { return transform_; }

 protected:
  kEn::transform transform_;

 private:
  std::optional<std::reference_wrapper<game_object>> parent_;
  std::vector<std::reference_wrapper<game_object>> children_;
  std::vector<std::shared_ptr<game_component>> components_;
};

}  // namespace kEn
