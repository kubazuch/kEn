#pragma once

// Copied from "glfw3.h"
#define MOUSE_CODES(X)		\
	X(button_last,		7)	\
	X(button_left,		0)	\
	X(button_right,		1)	\
	X(button_middle,	2)	\
							\
	X(button0,			0)	\
	X(button1,			1)	\
	X(button2,			2)	\
	X(button3,			3)	\
	X(button4,			4)	\
	X(button5,			5)	\
	X(button6,			6)	\
	X(button7,			7)	


namespace kEn
{
	using mouse_code = uint16_t;

	namespace mouse
	{
#		define ENUM_ENTRY(id, code)		id = (code),
#		define MAP_ENTRY(id, code)		{code, #id},

		enum : mouse_code
		{
			MOUSE_CODES(ENUM_ENTRY)
		};

		const std::unordered_map<mouse_code, const char*> name_map {
			MOUSE_CODES(MAP_ENTRY)
		};

		inline const char* name_of(const mouse_code key)
		{
			return name_map.at(key);
		}
	}
}

#undef MOUSE_CODES
#undef ENUM_ENTRY
#undef MAP_ENTRY