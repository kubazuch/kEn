#include "component.hpp"

#include <kEn/core/assert.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/scene/game_object.hpp>

namespace kEn {

const kEn::Transform& GameComponent::transform() const { return parent().transform(); }
kEn::Transform& GameComponent::transform() { return parent().transform(); }

void GameComponent::attach_to(GameObject& go) {
  KEN_CORE_ASSERT(parent_ == nullptr);
  parent_ = &go;
  on_attach();
}

void GameComponent::detach_from_parent() noexcept {
  KEN_CORE_ASSERT(parent_ != nullptr);
  on_detach();
  parent_ = nullptr;
}

}  // namespace kEn
