#include <kEn/scene/component.hpp>
#include <kEn/scene/game_object.hpp>
#include <kenpch.hpp>

namespace kEn {

const kEn::Transform& GameComponent::transform() const { return parent_.value().get().transform(); }

kEn::Transform& GameComponent::transform() { return parent_.value().get().transform(); }

}  // namespace kEn
