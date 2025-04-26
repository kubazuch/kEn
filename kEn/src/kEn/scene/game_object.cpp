#include <kEn/renderer/renderer.hpp>
#include <kEn/scene/game_object.hpp>
#include <kEn/scene/mesh/obj_model.hpp>
#include <kenpch.hpp>
#include <string_view>

namespace kEn {

IdRegistry<GameObject> GameObject::game_object_registry_(GameObject::kMaxGameObjects);

GameObject::GameObject(mEn::Vec3 pos, mEn::Quat rot, mEn::Vec3 scale, std::string_view name)
    : transform_(pos, rot, scale), id_(game_object_registry_), name_(name) {}

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

void GameObject::render(Shader& shader, double alpha) const {
  for (const auto& component : components_) {
    component->render(shader, alpha);
  }
}

void GameObject::render_all(Shader& shader, double alpha) const {
  render(shader, alpha);

  for (const auto child : children_) {
    child.get().render_all(shader, alpha);
  }
}

void GameObject::imgui() {
  for (const auto& component : components_) {
    component->imgui();
  }
}

void GameObject::imgui_all() {
  imgui();

  for (const auto& child : children_) {
    child.get().imgui_all();
  }
}

void GameObject::update(duration_t delta, duration_t time) {
  for (auto& component : components_) {
    component->update(delta, time);
  }
}

void GameObject::update_all(duration_t delta, duration_t time) {
  update(delta, time);

  for (const auto child : children_) {
    child.get().update_all(delta, time);
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
