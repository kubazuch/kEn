#include <kEn.h>

#include "kEn/event/event.h"
#include "kEn/event/key_events.h"


class sandbox : public kEn::application
{
public:
	sandbox() {	srand(time(NULL)); }

	//void run() override
	//{
	//	uint32_t c = 0; 

	//	kEn::event_dispatcher event_bus;
	//	event_bus.subscribe<kEn::key_pressed_event>(on_key_pressed_event);

	//	kEn::key_pressed_event event(kEn::key::backslash, false);
	//	event_bus.dispatch(event);

	//	/*while(running_)
	//	{
	//		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
	//		
	//		KEN_CORE_TRACE("Cycle no.: {0}", c);

	//		if (fizz_buzz_event event(c); !event_bus.dispatch(event))
	//			KEN_CORE_ERROR("Fizzbuzz event wasn't handled");

	//		c++;
	//	}*/
	//}


	static bool on_key_pressed_event(kEn::key_pressed_event& event)
	{
		KEN_TRACE("{0}", event);
		KEN_TRACE("{0}", kEn::mouse::name_of(kEn::mouse::button_left));
		return true;
	}
};

kEn::application* kEn::create_application()
{
	return new sandbox();
}
