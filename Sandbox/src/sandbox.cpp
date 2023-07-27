#include <kEn.h>

#include "kEn/event/event.h"

class fizz_buzz_event : public kEn::event<fizz_buzz_event>
{
public:
	fizz_buzz_event(uint32_t n) : num_{n} {}

	uint32_t num() const { return num_; }
	const char* name() const override { return "FizzBuzzEvent"; }
	std::string to_string() const override
	{
		std::stringstream ss;
		ss << name() << ": ";

		if (num_ % 3 == 0 && num_ % 5 == 0)
			ss << "FIZZ BUZZ";
		else if (num_ % 3 == 0)
			ss << "FIZZ";
		else if (num_ % 5 == 0)
			ss << "BUZZ";
		else
			ss << num_;

		return ss.str();
	}

private:
	uint32_t num_;
};

class sandbox : public kEn::application
{
public:
	sandbox() {	srand(time(NULL)); }

	//void run() override
	//{
	//	uint32_t c = 0; 

	//	kEn::event_dispatcher event_bus;
	//	event_bus.subscribe<fizz_buzz_event>(on_fizz_buzz_event);
	//	
	//	while(running_)
	//	{
	//		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
	//		
	//		KEN_CORE_TRACE("Cycle no.: {0}", c);

	//		if (fizz_buzz_event event(c); !event_bus.dispatch(event))
	//			KEN_CORE_ERROR("Fizzbuzz event wasn't handled");

	//		c++;
	//	}
	//}

	static bool on_fizz_buzz_event(const fizz_buzz_event& event)
	{
		KEN_INFO(event);
		return event.num() % 7 != 0;
	}
};

kEn::application* kEn::create_application()
{
	return new sandbox();
}
