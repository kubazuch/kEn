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

#ifdef KEN_ENABLE_ASSERTS
#	define KEN_ASSERT(x, ...) { if(!(x)) { KEN_CRITICAL("Assert failed: {0}", __VA_ARGS__); __debugbreak(); } }
#	define KEN_CORE_ASSERT(x, ...) { if(!(x)) { KEN_CORE_CRITICAL("Assert failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#	define KEN_ASSERT(x, ...) 
#	define KEN_CORE_ASSERT(x, ...) 
#endif

#define VIRTUAL_FIVE(class)	class(const class&) = delete;\
							class(class&&) = delete;\
							class& operator=(const class&) noexcept = delete;\
							class& operator=(class&&) noexcept = delete
