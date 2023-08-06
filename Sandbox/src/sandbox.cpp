#include <kEn.h>

#include "imgui/imgui.h"
#include "kEn/core/input.h"
#include "kEn/event/event.h"
#include "kEn/event/key_events.h"

#include "kEn/core/assert.h"

class fizzbuzz_layer : public kEn::layer
{
public:
	fizzbuzz_layer() : layer("FizzBuzz") {}

	void on_update() override
	{
	}

	void on_attach() override
	{
		KEN_ASSERT(true);
		_KEN_DEBUG("Attached!");
	}

	void on_detach() override
	{
		_KEN_DEBUG("Detached!");
	}

	void on_event(kEn::base_event& event) override
	{
		//KEN_TRACE("{0}", event);
	}

	void on_imgui() override
	{
		ImGui::Begin("Fizzbuzz!");
		ImGui::Text("Fizz or buzz? That is the question...");

		const auto pos = kEn::input::get_mouse_pos();
		ImGui::Text("Mouse pos: %.1f, %.1f", pos.x, pos.y);
		ImGui::End();
	}
};

class sandbox : public kEn::application
{
public:
	sandbox()
	{
		srand(time(NULL));
		push_layer(new fizzbuzz_layer());
	}

};

kEn::application* kEn::create_application()
{
	return new sandbox();
}
