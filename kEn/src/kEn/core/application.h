#pragma once
#include "core.h"
#include "layer_stack.h"
#include "window.h"
#include "kEn/event/application_events.h"
#include "kEn/event/event.h"

namespace kEn {

	class application
	{
	public:
		application();
		virtual ~application();

		virtual void run();

		void window_event_handler(base_event& e);

		void push_layer(layer* layer);
		void push_overlay(layer* overlay);

		VIRTUAL_FIVE(application);
	protected:	//TODO: Private
		bool on_window_close(window_close_event& e);

		std::unique_ptr<window> window_;
		std::unique_ptr<event_dispatcher> dispatcher_;
		bool running_ = true;
		layer_stack layer_stack_;
	};

	application* create_application();
}
