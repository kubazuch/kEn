#pragma once
#include "core.h"

namespace kEn {

	class KEN_API application
	{
	public:
		application();
		virtual ~application();

		virtual void run();
	};

	application* create_application();
}
