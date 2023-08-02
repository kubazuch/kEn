#pragma once

#include "kenpch.h"
#include "core.h"
#include "kEn/event/event.h"

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

	class window
	{
	public:
		using handler_t = std::function<void(base_event&)>;

		window() = default;
		virtual ~window() = default;

		virtual void on_update() = 0;

		virtual unsigned int width() const = 0;
		virtual unsigned int height() const = 0;

		virtual void set_event_handler(const handler_t& handler) = 0;
		virtual void set_vsync(bool enabled) = 0;
		virtual bool vsync() const = 0;

		static window* create(const window_properties& props = window_properties());

		VIRTUAL_FIVE(window);
	};
}