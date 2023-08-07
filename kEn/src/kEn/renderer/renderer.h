#pragma once
#include "vertex_array.h"

namespace kEn
{

	class renderer
	{
	public:
		static void begin_scene();
		static void end_scene();

		static void submit(const std::shared_ptr<vertex_array>& vertex_array);
	};
}
