#pragma once
#include "core.h"
#include "layer_stack.h"
#include "window.h"
#include "kEn/event/application_events.h"
#include "kEn/event/event.h"
#include "kEn/imgui/imgui_layer.h"

namespace kEn {

	class application_cleanup
	{
	public:
		virtual ~application_cleanup();
	};

	class application : application_cleanup
	{
	public:
		application();
		virtual ~application() override {}

		virtual void run();

		void window_event_handler(base_event& e);

		void push_layer(layer* layer);
		void push_overlay(layer* overlay);

		window& main_window() const { return *window_; }
		static application& instance() { return *instance_; }

		VIRTUAL_FIVE(application);
	protected:	//TODO: Private
		bool on_window_close(window_close_event& e);

		std::unique_ptr<window> window_;
		std::unique_ptr<event_dispatcher> dispatcher_;
		imgui_layer* imgui_layer_;
		bool running_ = true;
		layer_stack layer_stack_;
	private:
		static application* instance_;
	};

	application* create_application();
}
