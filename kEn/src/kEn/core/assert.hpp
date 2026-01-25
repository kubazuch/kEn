#pragma once

// https://github.com/TheCherno/Hazel/blob/93af298aa9007f50bc908edb84812b3af8df2409/Hazel/src/Hazel/Core/Assert.h
// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#ifdef KEN_ENABLE_ASSERTS
// NOLINTNEXTLINE(unused-includes)
#include <filesystem>

#include <kEn/core/core.hpp>
#include <kEn/core/log.hpp>

#define KEN_INT_ASSERT_IMPL(type, check, msg, ...) \
  {                                                \
    if (!(check)) {                                \
      KEN##type##ERROR(msg, __VA_ARGS__);          \
      __debugbreak();                              \
    }                                              \
  }
#define KEN_INT_ASSERT_MSG(type, check, ...) KEN_INT_ASSERT_IMPL(type, check, "Assert failed: {0}", __VA_ARGS__)
#define KEN_INT_ASSERT_NO_MSG(type, check)                                   \
  KEN_INT_ASSERT_IMPL(type, check, "Assert '{0}' failed at {1}:{2}", #check, \
                      std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define KEN_INT_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define KEN_INT_ASSERT_GET_MACRO(...) \
  KEN_EXPAND(KEN_INT_ASSERT_GET_MACRO_NAME(__VA_ARGS__, KEN_INT_ASSERT_MSG, KEN_INT_ASSERT_NO_MSG))

/**
 * @ingroup ken
 * @brief Assertion macro that checks a condition. If the condition is false, it logs to CLIENT logger and triggers a
 * debug breakpoint.
 *
 * @param check		The condition to be checked. Should be an expression that yields a boolean value.
 * @param message	Optional. An error message to be included in the error log when the assertion fails. When
 * omitted, the message will be: "Assert '[condition]' failed at [filename]:[line]"
 * @param ...		Optional variadic arguments that can be used for further formatting the error message (if using
 * the message parameter).
 *
 * Example usage:
 * @code
 * int divisor = 0;
 * KEN_ASSERT(divisor != 0, "Divide by zero error!");
 * int result = dividend / divisor;
 * @endcode
 */
#define KEN_ASSERT(...) KEN_SEMICOLON(KEN_INT_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))

/**
 * @ingroup ken
 * @brief Assertion macro that checks a condition. If the condition is false, it logs to CORE logger and triggers a
 * debug breakpoint.
 *
 * @param check		The condition to be checked. Should be an expression that yields a boolean value.
 * @param message	Optional. An error message to be included in the error log when the assertion fails. When
 * omitted, the message will be: "Assert '[condition]' failed at [filename]:[line]"
 * @param ...		Optional variadic arguments that can be used for further formatting the error message (if using
 * the message parameter).
 *
 * Example usage:
 * @code
 * int divisor = 0;
 * KEN_CORE_ASSERT(divisor != 0, "Divide by zero error!");
 * int result = dividend / divisor;
 * @endcode
 */
#define KEN_CORE_ASSERT(...) KEN_SEMICOLON(KEN_INT_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
#define KEN_ASSERT(...)
#define KEN_CORE_ASSERT(...)
#endif
// NOLINTEND(cppcoreguidelines-macro-usage)
