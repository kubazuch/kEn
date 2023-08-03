#include "kenpch.h"
#include "application.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "kEn/event/application_events.h"

#ifdef KEN_PLATFORM_WIN
#	include "platform/win/win_window.h"
#endif

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
		dispatcher_->subscribe<window_close_event>([this](auto& event) { return on_window_close(event); });

		imgui_layer_ = new imgui_layer();
		push_overlay(imgui_layer_);
	}

	application_cleanup::~application_cleanup()
	{
#ifdef KEN_PLATFORM_WIN
		windows_window::api_terminate();
#endif
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
			glClearColor(0.19f, 0.65f, 0.32f, 0.01f);
			glClear(GL_COLOR_BUFFER_BIT);

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
}