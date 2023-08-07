#pragma once

#ifdef _KEN_DEBUG
#	define KEN_ENABLE_ASSERTS
#endif

#define KEN_EXPAND(X) X
#define KEN_SEMICOLON(X) do {X} while(0)
#define KEN_STRINGIFY(X) #X

#define VIRTUAL_FIVE(class)	class(const class&) = delete;\
							class(class&&) = delete;\
							class& operator=(const class&) noexcept = delete;\
							class& operator=(class&&) noexcept = delete