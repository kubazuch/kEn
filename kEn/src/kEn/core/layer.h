#pragma once
#include "kEn/event/event.h"

namespace kEn
{
	class layer
	{
	public:
		layer(const std::string& name = "Layer");
		virtual ~layer() = default;

		virtual void on_attach() = 0;
		virtual void on_detach() = 0;
		virtual void on_update() = 0;
		virtual void on_event(base_event& event) = 0;

		const std::string& name() const { return debug_name_; }

		VIRTUAL_FIVE(layer);
	private:
		std::string debug_name_;
	};
}

