#include "game_object.hpp"

#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#ifdef KEN_DEBUG_BUILD
#include <algorithm>
#endif

#include <mEn/fwd.hpp>

#include <kEn/core/assert.hpp>
#include <kEn/core/log.hpp>
#include <kEn/core/timestep.hpp>
#include <kEn/event/event.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/scene/component.hpp>
#include <kEn/scene/id_registry.hpp>

namespace kEn {

Registry<GameObject> GameObject::game_object_registry_(GameObject::kMaxGameObjects);

GameObject::GameObject(mEn::Vec3 pos, mEn::Quat rot, mEn::Vec3 scale, std::string_view name)
    : transform_(pos, rot, scale), id_(game_object_registry_), name_(name) {
  game_object_registry_.bind(id_.handle(), this);
  transform_.set_on_changed([this] { on_transform_changed(); });
}

GameObject::~GameObject() {
  for (const auto& component : components_) {
    component->on_detach();
  }

  for (auto* child : children_) {
    child->parent_ = nullptr;
  }

  if (parent_ != nullptr) {
    std::erase_if(parent_->children_, [this](const auto* ptr) { return ptr == this; });
  }

  KEN_CORE_DEBUG("GameObject {} with id {} destroyed", name_, id_.raw_id());
}

bool GameObject::can_add_child(const GameObject& child) const noexcept {
  return child.transform_.can_set_parent(transform_);
}

bool GameObject::try_add_child(GameObject& child) {
  if (!can_add_child(child)) {
    return false;
  }

  if (child.parent_ != nullptr) {
    std::erase(child.parent_->children_, &child);
  }

  child.parent_ = this;
  children_.push_back(&child);
  child.transform_.set_parent(transform_);
  return true;
}

GameObject& GameObject::add_child(GameObject& child) {
  const bool ok = try_add_child(child);
  KEN_CORE_ASSERT(ok, "add_child would create a cycle or self-parenting");
  return *this;
}

void GameObject::remove_child(GameObject& child) {
  KEN_CORE_ASSERT(child.parent_ == this, "remove_child: object is not a direct child");
  std::erase(children_, &child);
  child.parent_ = nullptr;
  child.transform_.unset_parent();
}

bool GameObject::try_set_parent(GameObject& parent) { return parent.try_add_child(*this); }

void GameObject::set_parent(GameObject& parent) { parent.add_child(*this); }

void GameObject::unset_parent() {
  if (parent_ == nullptr) {
    return;
  }
  parent_->remove_child(*this);
}

GameComponent& GameObject::add_component(std::unique_ptr<GameComponent> to_add) {
  KEN_CORE_ASSERT(to_add != nullptr);
  const auto* raw = to_add.get();
  KEN_CORE_ASSERT(!std::ranges::contains(components_, raw, [](const auto& c) { return c.get(); }));

  to_add->parent_ = *this;
  components_.push_back(std::move(to_add));
  try {
    components_.back()->on_attach();
  } catch (...) {
    components_.back()->parent_.reset();
    components_.pop_back();
    throw;
  }

  return *components_.back();
}

void GameObject::render(Shader& shader, double alpha, bool recursive) const {
  for (const auto& component : components_) {
    component->render(shader, alpha);
  }

  if (recursive) {
    for (const auto* child : children_) {
      child->render(shader, alpha);
    }
  }
}

void GameObject::imgui(bool recursive) {
  for (const auto& component : components_) {
    component->imgui();
  }

  if (recursive) {
    for (auto* child : children_) {
      child->imgui();
    }
  }
}

void GameObject::update(Timestep delta, Timestep time, bool recursive) {
  for (auto& component : components_) {
    component->update(delta, time);
  }

  if (recursive) {
    for (auto* child : children_) {
      child->update(delta, time);
    }
  }
}

void GameObject::on_event(BaseEvent& event, bool recursive) {
  for (const auto& component : components_) {
    component->on_event(event);
    if (event.handled) {
      return;
    }
  }

  if (recursive) {
    for (auto* child : children_) {
      child->on_event(event);
      if (event.handled) {
        return;
      }
    }
  }
}

void GameObject::on_transform_changed() {
  for (const auto& component : components_) {
    component->on_transform_changed();
  }
}

}  // namespace kEn
