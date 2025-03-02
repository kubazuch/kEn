#include <kEn/scene/component.hpp>
#include <kEn/scene/game_object.hpp>
#include <kenpch.hpp>

namespace kEn {

const kEn::transform& game_component::transform() const { return parent_.value().get().transform(); }

kEn::transform& game_component::transform() { return parent_.value().get().transform(); }

}  // namespace kEn
