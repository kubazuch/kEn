#include "kenpch.h"
#include "win_window.h"
#include "kEn/event/application_events.h"
#include "kEn/event/key_events.h"
#include "kEn/event/mouse_events.h"

#include "kEn/core/assert.h"
#include "kEn/renderer/renderer_api.h"

namespace kEn
{
	static uint8_t GLFW_window_count = 0;

	static void api_error_callback(int error_code, const char* description)
	{
		KEN_CORE_ERROR("GLFW Error #{0}: {1}", error_code, description);
	}

	void windows_window::api_init()
	{
		const int status = glfwInit();
		KEN_CORE_ASSERT(status, "GLFW init failed!");

		glfwSetErrorCallback(api_error_callback);
	}

	void windows_window::api_shutdown()
	{
		glfwTerminate();
	}

	window* window::create(const window_properties& props)
	{
		return new windows_window(props);
	}

	windows_window::windows_window(const window_properties& properties)
	{
		data_.title = properties.title;
		data_.width = properties.width;
		data_.height = properties.height;

		KEN_CORE_DEBUG("Creating window {0} ({1} x {2})", properties.title, properties.width, properties.height);

		if (GLFW_window_count == 0)
			api_init();

#ifdef _KEN_DEBUG
		if(renderer_api::get_api() == renderer_api::api::opengl)
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
		glfwWindowHint(GLFW_SAMPLES, 16);

		window_ptr = glfwCreateWindow(static_cast<int>(data_.width), static_cast<int>(data_.height), data_.title.c_str(), nullptr, nullptr);
		++GLFW_window_count;

		context_ = graphics_context::create(window_ptr);
		context_->init();

		glfwSetWindowUserPointer(window_ptr, &data_);
		windows_window::set_vsync(true);

		set_glfw_callbacks();
	}

	void windows_window::set_glfw_callbacks() const
	{
		// events purr!
		glfwSetWindowCloseCallback(window_ptr, [](GLFWwindow* window)
		{
			const data& win_data = *static_cast<data*>(glfwGetWindowUserPointer(window));
			window_close_event event;
			win_data.handler(event);
		});

		glfwSetWindowSizeCallback(window_ptr, [](GLFWwindow* window, int width, int height)
		{
			data& win_data = *static_cast<data*>(glfwGetWindowUserPointer(window));
			win_data.width = width;
			win_data.height = height;

			window_resize_event event(width, height);
			win_data.handler(event);
		});

		glfwSetKeyCallback(window_ptr, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			const data& win_data = *static_cast<data*>(glfwGetWindowUserPointer(window));
			
			switch (action)
			{
			case GLFW_PRESS:
			{
				key_pressed_event event(static_cast<key_code>(key), false);
				win_data.handler(event);
				break;
			}
			case GLFW_REPEAT:
			{
				key_pressed_event event(static_cast<key_code>(key), true);
				win_data.handler(event);
				break;
			}
			case GLFW_RELEASE:
			{
				key_released_event event(static_cast<key_code>(key));
				win_data.handler(event);
				break;
			}
			}
		});

		glfwSetCharCallback(window_ptr, [](GLFWwindow* window, unsigned int keycode)
		{
			const data& win_data = *static_cast<data*>(glfwGetWindowUserPointer(window));
			key_typed_event event(static_cast<key_code>(keycode));
			win_data.handler(event);
		});

		glfwSetMouseButtonCallback(window_ptr, [](GLFWwindow* window, int button, int action, int mods)
		{
			const data& win_data = *static_cast<data*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				mouse_button_pressed_event event(static_cast<mouse_code>(button));
				win_data.handler(event);
				break;
			}
			case GLFW_RELEASE:
			{
				mouse_button_released_event event(static_cast<mouse_code>(button));
				win_data.handler(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(window_ptr, [](GLFWwindow* window, double x_offset, double y_offset)
		{
			const data& win_data = *static_cast<data*>(glfwGetWindowUserPointer(window));

			mouse_scroll_event event(static_cast<float>(x_offset), static_cast<float>(y_offset));
			win_data.handler(event);
		});

		glfwSetCursorPosCallback(window_ptr, [](GLFWwindow* window, double x, double y)
		{
			const data& win_data = *static_cast<data*>(glfwGetWindowUserPointer(window));

			mouse_move_event event(static_cast<float>(x), static_cast<float>(y));
			win_data.handler(event);
		});
	}

	windows_window::~windows_window()
	{
		glfwDestroyWindow(window_ptr);
		--GLFW_window_count;

		if (GLFW_window_count == 0)
			api_shutdown();
	}

	void windows_window::on_update()
	{
		glfwPollEvents();
		context_->swap_buffers();
	}

	void windows_window::set_vsync(const bool enabled)
	{
		glfwSwapInterval(enabled ? 1 : 0);
		data_.vsync = enabled;
	}

	bool windows_window::vsync() const
	{
		return data_.vsync;
	}
}
