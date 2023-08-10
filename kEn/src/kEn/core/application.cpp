#include "kenpch.h"
#include "application.h"
#include <GLFW/glfw3.h>

#include "assert.h"
#include "imgui.h"
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
		double previous = glfwGetTime(); //TODO: platform
		double lag = 0.0;
		double time = 0.0;

		//TODO: TEMP
		int frames = 0, ticks = 0;
		int FPS = 0, TPS = 0;
		double frame_counter = 0.0;

		bool vsync = window_->vsync();

		int tps = 120;

		//TODO: CLEANUP, cause this is hack city
		while (running_)
		{
			double current = glfwGetTime(); //TODO: platform
			double delta = current - previous;
			previous = current;

			lag += delta;
			frame_counter += delta;

			double mspt = 1.0 / tps;

			while(lag >= mspt)
			{
				if (vsync != window_->vsync())
					window_->set_vsync(vsync);

				for (layer* layer : layer_stack_)
					layer->on_update(mspt, time);
				
				lag -= mspt;
				time += mspt;
				++ticks;
			}

			for (layer* layer : layer_stack_)
				layer->on_render();

			++frames;
			if (frame_counter >= 1.0)
			{
				FPS = frames;
				TPS = ticks;
				frames = 0;
				ticks = 0;
				frame_counter = 0;
			}

			imgui_layer_->begin();
			for (layer* layer : layer_stack_)
				layer->on_imgui();

			ImGui::Begin("DEBUG");
			ImGui::Text("FPS: %d", FPS);
			ImGui::Text("TPS: %d", TPS);
			ImGui::Checkbox("VSync", &vsync);
			ImGui::SliderInt("Tickrate", &tps, 1, 240);
			ImGui::End();

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
