#pragma once
#include "core.h"
#include "window.h"

namespace kEn {

	class KEN_API application
	{
	public:
		application();
		virtual ~application();

		virtual void run();

		VIRTUAL_FIVE(application);
	protected:
		std::unique_ptr<window> window_;
		bool running_ = true;
	};

	application* create_application();
}
