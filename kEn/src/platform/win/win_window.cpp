#include "kenpch.h"
#include "win_window.h"

namespace kEn
{
	static bool GLFW_init = false;

	window* window::create(const window_properties& props)
	{
		return new windows_window(props);
	}

	windows_window::windows_window(const window_properties& properties)
	{
		data_.title = properties.title;
		data_.width = properties.width;
		data_.height = properties.height;

		KEN_CORE_INFO("Creating window {0} ({1} x {2})", properties.title, properties.width, properties.height);

		if (!GLFW_init)
		{

			// TODO: glfwTerminate()
			int status = glfwInit();
			KEN_CORE_ASSERT(status, "GLFW initialization failed!");

			GLFW_init = true;
		}

		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
		window_ = glfwCreateWindow(data_.width, data_.height, data_.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window_);
		glfwSetWindowUserPointer(window_, &data_);
		//set_vsync(true);
	}

	windows_window::~windows_window()
	{
		glfwDestroyWindow(window_);
	}

	void windows_window::on_update()
	{
		glfwPollEvents();
		glfwSwapBuffers(window_);
	}

	void windows_window::set_vsync(bool enabled)
	{
		glfwSwapInterval(enabled ? 1 : 0);
		data_.vsync = enabled;
	}

	bool windows_window::vsync() const
	{
		return data_.vsync;
	}
}
