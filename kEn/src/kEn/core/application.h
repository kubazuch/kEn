#pragma once
#include "core.h"
#include "window.h"
#include "kEn/event/application_events.h"
#include "kEn/event/event.h"

namespace kEn {

	class KEN_API application
	{
	public:
		application();
		virtual ~application();

		virtual void run();

		void window_event_handler(base_event& e);

		bool on_window_close(window_close_event& e);

		VIRTUAL_FIVE(application);
	protected:
		std::unique_ptr<window> window_;
		std::unique_ptr<event_dispatcher> dispatcher_;
		bool running_ = true;
	};

	application* create_application();
}
