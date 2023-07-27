#pragma once

#include "kenpch.h"
#include "core.h"
#include "event/event.h"

namespace kEn
{
	struct window_properties
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		window_properties(std::string title = "kEngine", unsigned int width = 1280, unsigned int height = 720)
			: title(std::move(title)), width(width), height(height)
		{}
	};

	class KEN_API window
	{
	public:
		using callback_t = std::function<void(base_event&)>;

		window() = default;
		virtual ~window() = default;

		virtual void on_update() = 0;

		virtual unsigned int width() const = 0;
		virtual unsigned int height() const = 0;

		virtual void set_event_callback(const callback_t& callback) = 0;
		virtual void set_vsync(bool enabled) = 0;
		virtual bool vsync() const = 0;

		static window* create(const window_properties& props = window_properties());

		VIRTUAL_FIVE(window);
	};
}