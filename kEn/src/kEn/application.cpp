#include "kenpch.h"
#include "application.h"
#include <GLFW/glfw3.h>

namespace kEn
{
	application::application()
	{
		window_ = std::unique_ptr<window>(window::create());
	}

	application::~application()
	{
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
}
