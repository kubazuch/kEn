#include <kEn/renderer/renderer.hpp>
#include <kEn/scene/game_object.hpp>
#include <kEn/scene/mesh/obj_model.hpp>
#include <kenpch.hpp>

namespace kEn {
GameObject::GameObject(glm::vec3 pos, glm::quat rot, glm::vec3 scale) : transform_(pos, rot, scale) {}

GameObject::~GameObject() {
  for (const auto child : children_) {
    child.get().parent_.reset();
  }

  if (parent_.has_value()) {
    std::erase_if(parent_.value().get().children_, [this](auto ref) { return std::addressof(ref.get()) == this; });
  }
}

GameObject& GameObject::add_child(GameObject& child) {
  if (child.parent_.has_value()) {
    std::erase_if(child.parent_.value().get().children_,
                  [this](auto ref) { return std::addressof(ref.get()) == this; });
  }

  child.parent_ = *this;
  child.transform().set_parent(transform_);

  children_.emplace_back(child);

  return *this;
}

GameObject& GameObject::add_children(std::initializer_list<std::reference_wrapper<GameObject>> children) {
  for (auto child : children) {
    add_child(child);
  }

  return *this;
}

GameObject& GameObject::add_component(std::shared_ptr<GameComponent> to_add) {
  to_add->parent_ = *this;

  components_.push_back(std::move(to_add));

  return *this;
}

GameObject& GameObject::add_components(std::initializer_list<std::shared_ptr<GameComponent>> components) {
  for (const auto& component : components) {
    add_component(component);
  }

  return *this;
}

void GameObject::render(Shader& shader) const {
  for (const auto& component : components_) {
    component->render(shader);
  }
}

void GameObject::render_all(Shader& shader) const {
  render(shader);

  for (const auto child : children_) {
    child.get().render_all(shader);
  }
}

void GameObject::update(double delta) {
  for (auto& component : components_) {
    component->update(delta);
  }
}

void GameObject::update_all(double delta) {
  update(delta);

  for (const auto child : children_) {
    child.get().update_all(delta);
  }
}

void GameObject::on_event(BaseEvent& event) {
  for (const auto& component : components_) {
    component->on_event(event);
    if (event.handled) {
      return;
    }
  }

  for (const auto& child : children_) {
    child.get().on_event(event);
    if (event.handled) {
      return;
    }
  }
}
}  // namespace kEn
