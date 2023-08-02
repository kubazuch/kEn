#include <kEn.h>

#include "kEn/event/event.h"
#include "kEn/event/key_events.h"


class fizzbuzz_layer : public kEn::layer
{
public:
	fizzbuzz_layer() : layer("FizzBuzz") {}

	void on_update() override
	{
		KEN_INFO("FIZZ BUZZ!");
	}

	void on_attach() override
	{
		_KEN_DEBUG("Attached!");
	}
	void on_detach() override
	{
		_KEN_DEBUG("Detached!");
	}

	void on_event(kEn::base_event& event) override
	{
		KEN_TRACE("{0}", event);
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
