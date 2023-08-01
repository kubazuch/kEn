#include "kenpch.h"
#include "application.h"
#include <GLFW/glfw3.h>

#include "kEn/event/application_events.h"


#ifdef KEN_PLATFORM_WIN
#	include "platform/win/win_window.h"
#endif

namespace kEn
{
	application::application()
	{
		window_ = std::unique_ptr<window>(window::create());
		window_->set_event_handler([this](auto& event) { window_event_handler(event); });

		dispatcher_ = std::make_unique<event_dispatcher>();
		dispatcher_->subscribe<window_close_event>([this](auto& event) { return on_window_close(event); });
	}

	application::~application()
	{
#ifdef KEN_PLATFORM_WIN
		window_.reset();
		windows_window::api_terminate();
#endif
	}

	void application::run()
	{
		while (running_)
		{
			glClearColor(0.19f, 0.65f, 0.32f, 0.01f);
			glClear(GL_COLOR_BUFFER_BIT);
			window_->on_update();
		}
	}

	void application::window_event_handler(base_event& e)
	{
		KEN_CORE_INFO("{0}", e);
		dispatcher_->dispatch(e);
	}

	bool application::on_window_close(window_close_event& e)
	{
		running_ = false;
		return true;
	}
}
