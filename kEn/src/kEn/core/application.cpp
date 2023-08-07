#include "kenpch.h"
#include "application.h"
#include <GLFW/glfw3.h>

#include "assert.h"
#include "kEn/event/application_events.h"
#include "kEn/renderer/renderer.h"
#include "kEn/renderer/render_command.h"

namespace kEn
{
	application* application::instance_ = nullptr;

	application::application()
	{
		KEN_CORE_ASSERT(!instance_, "App already exists!");
		instance_ = this;

		window_ = std::unique_ptr<window>(window::create());
		window_->set_event_handler([this](auto& event) { window_event_handler(event); });

		dispatcher_ = std::make_unique<event_dispatcher>();
		dispatcher_->subscribe<window_close_event>(KEN_EVENT_SUBSCRIBER(on_window_close));
		dispatcher_->subscribe<window_resize_event>(KEN_EVENT_SUBSCRIBER(on_window_resize));

		render_command::init();

		imgui_layer_ = new imgui_layer();
		push_overlay(imgui_layer_);
	}

	void application::push_layer(layer* layer)
	{
		layer_stack_.push_layer(layer);
	}

	void application::push_overlay(layer* overlay)
	{
		layer_stack_.push_overlay(overlay);
	}

	void application::run()
	{
		while (running_)
		{
			for (layer* layer : layer_stack_)
				layer->on_update();

			imgui_layer_->begin();
			for (layer* layer : layer_stack_)
				layer->on_imgui();
			imgui_layer_->end();

			window_->on_update();
		}
	}

	void application::window_event_handler(base_event& e)
	{
		dispatcher_->dispatch(e);

		for (auto it = layer_stack_.rbegin(); it != layer_stack_.rend(); ++it)
		{
			if (e.handled)
				break;
			(*it)->on_event(e);
		}
	}

	bool application::on_window_close(window_close_event& e)
	{
		running_ = false;
		return true;
	}

	bool application::on_window_resize(window_resize_event& e)
	{
		render_command::set_viewport(0, 0, e.width(), e.height());
		return true;
	}
}
