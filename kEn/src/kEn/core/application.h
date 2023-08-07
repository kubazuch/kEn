#pragma once
#include "core.h"
#include "layer_stack.h"
#include "window.h"
#include "kEn/event/application_events.h"
#include "kEn/event/event.h"
#include "kEn/imgui/imgui_layer.h"
#include "kEn/renderer/buffer.h"
#include "kEn/renderer/shader.h"
#include "kEn/renderer/vertex_array.h"

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

		bool on_window_close(window_close_event& e);
		bool on_window_resize(window_resize_event& e);

		std::unique_ptr<window> window_;
		std::unique_ptr<event_dispatcher> dispatcher_;
		imgui_layer* imgui_layer_;
		bool running_ = true;
		layer_stack layer_stack_;
		
	private:
		static application* instance_;
		friend int ::main(int argc, char** argv);
	};

	// Client must define this!
	application* create_application();
}
