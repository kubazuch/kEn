#pragma once

#ifdef KEN_PLATFORM_WIN
	#ifdef KEN_DLL
		#define KEN_API __declspec(dllexport)
	#else
		#define KEN_API __declspec(dllimport)
	#endif
#else
	#error Windows only!
#endif
