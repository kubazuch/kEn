#include "kenpch.h"
#include "component.h"

#include "game_object.h"

namespace kEn
{
	const kEn::transform& game_component::transform() const
	{
		return parent_.value().get().transform();
	}

	kEn::transform& game_component::transform()
	{
		return parent_.value().get().transform();
	}


}
