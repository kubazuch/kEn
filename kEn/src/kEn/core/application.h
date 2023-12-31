#pragma once
#include "core.h"
#include "layer_stack.h"
#include "window.h"
#include "kEn/event/application_events.h"
#include "kEn/event/event.h"
#include "kEn/imgui/imgui_layer.h"

#define KEN_TIME_PER_UPDATE (1.0/120.0)

int main(int argc, char** argv);

namespace kEn {

	class application
	{
	public:
		application();
		virtual ~application() = default;
		
		void window_event_handler(base_event& e);

		void push_layer(layer* layer);
		void push_overlay(layer* overlay);

		window& main_window() const { return *window_; }
		static application& instance() { return *instance_; }

		VIRTUAL_FIVE(application);
	private:
		void run();
		void update(double delta);
		void render();

		bool on_window_close(window_close_event& e);
		bool on_window_resize(window_resize_event& e);

	private:
		std::unique_ptr<window> window_;
		std::unique_ptr<event_dispatcher> dispatcher_;
		imgui_layer* imgui_layer_;
		bool running_ = true;
		bool minimized_ = false;
		layer_stack layer_stack_;

		bool vsync_ = true;
		double time_ = 0.0;
		int fps_ = 0, tps_ = 0;
	private:
		static application* instance_;
		friend int ::main(int argc, char** argv);
	};

	// Client must define this!
	application* create_application();
}
