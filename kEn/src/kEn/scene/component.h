#pragma once

#include "kEn/event/event.h"
#include "kEn/renderer/shader.h"

namespace kEn
{
	class game_object;

	class game_component
	{
	public:
		game_component() = default;
		virtual ~game_component() = default;
		[[nodiscard]] virtual std::shared_ptr<game_component> clone() const = 0;

		virtual void update(float delta) = 0;
		virtual void render(shader& shader) = 0;
		virtual void on_event(base_event& event) { dispatcher_.dispatch(event); }

		const kEn::transform& transform() const;
		kEn::transform& transform();

		VIRTUAL_FIVE(game_component);
	protected:
		std::optional<std::reference_wrapper<game_object>> parent_;
		kEn::event_dispatcher dispatcher_;
		friend game_object;
	};
}
