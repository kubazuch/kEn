#pragma once

#ifdef KEN_PLATFORM_WIN
#	ifdef KEN_DLL
#		define KEN_API __declspec(dllexport)
#		define KEN_TEMPLATE 
#	else
#		define KEN_API __declspec(dllimport)
#		define KEN_TEMPLATE extern
#	endif
#else
#	error Windows only!
#endif
