#pragma once

#include "event.h"

namespace kEn
{
	class window_close_event : public event<window_close_event>
	{
	public:
		window_close_event() = default;

		const char* name() const override { return "WindowCloseEvent"; }
	};

	class window_resize_event : public event<window_resize_event>
	{
	public:
		window_resize_event(unsigned int width, unsigned int height) : width_(width), height_(height) {}

		unsigned int width() const { return width_; }
		unsigned int height() const { return height_; }

		const char* name() const override { return "WindowResizeEvent"; }
		std::string to_string() const override
		{
			std::stringstream ss;
			ss << name() << ": " << width_ << " x " << height_;
			return ss.str();
		}

	private:
		unsigned int width_, height_;
	};
}