#pragma once

#include "kEn/core.h"
#include "kEn/window.h"

#include <GLFW/glfw3.h>

namespace kEn
{
	class windows_window : public window
	{
	public:
		windows_window(const window_properties& properties);
		virtual ~windows_window() override;

		void on_update() override;

		inline unsigned int width() const override { return data_.width; }
		inline unsigned int height() const override { return data_.height; }

		inline void set_event_callback(const callback_t& callback) override { data_.callback = callback; }

		void set_vsync(bool enabled) override;
		bool vsync() const override;

		VIRTUAL_FIVE(windows_window);
	private:
		GLFWwindow* window_;

		struct data
		{
			std::string title;
			unsigned int width, height;

			bool vsync;

			callback_t callback;
		};

		data data_;
	};
}
