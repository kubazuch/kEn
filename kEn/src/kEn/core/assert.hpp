#pragma once

/** @file
 *  @ingroup ken
 */

// https://github.com/TheCherno/Hazel/blob/93af298aa9007f50bc908edb84812b3af8df2409/Hazel/src/Hazel/Core/Assert.h
// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#ifdef KEN_DEBUG_BUILD
#define KEN_ENABLE_ASSERTS
#endif

// std::unreachable() is used inside KEN_UNREACHABLE() macro expansions
// NOLINTNEXTLINE(unused-includes)
#include <utility>

#ifdef KEN_ENABLE_ASSERTS
#include <kEn/core/core.hpp>
#include <kEn/core/log.hpp>

#define KEN_INT_ASSERT_IMPL(type, check, msg, ...) \
  if (!(check)) {                                  \
    KEN##type##ERROR(msg, __VA_ARGS__);            \
    KEN_DEBUGBREAK();                              \
  }
#define KEN_INT_ASSERT_MSG(type, check, ...) KEN_INT_ASSERT_IMPL(type, check, "Assert failed: {0}", __VA_ARGS__)
#define KEN_INT_ASSERT_NO_MSG(type, check) KEN_INT_ASSERT_IMPL(type, check, "Assert '{0}' failed", #check)

#define KEN_INT_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define KEN_INT_ASSERT_GET_MACRO(...) \
  KEN_EXPAND(KEN_INT_ASSERT_GET_MACRO_NAME(__VA_ARGS__, KEN_INT_ASSERT_MSG, KEN_INT_ASSERT_NO_MSG))

/**
 * @brief Assertion macro that checks a condition. If the condition is false, it logs to CLIENT logger and triggers a
 * debug breakpoint.
 *
 * @param check    The condition to be checked. Should be an expression that yields a boolean value.
 * @param message  Optional. An error message to be included in the error log when the assertion fails. When
 * omitted, the message will be: "Assert '[condition]' failed"
 * @param ...      Optional variadic arguments that can be used for further formatting the error message (if using
 * the message parameter).
 *
 * Example usage:
 * @code
 * int divisor = 0;
 * KEN_ASSERT(divisor != 0, "Divide by zero error!");
 * int result = dividend / divisor;
 * @endcode
 */
#define KEN_ASSERT(...) KEN_STMT(KEN_INT_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))

/**
 * @brief Assertion macro that checks a condition. If the condition is false, it logs to CORE logger and triggers a
 * debug breakpoint.
 *
 * @param check    The condition to be checked. Should be an expression that yields a boolean value.
 * @param message  Optional. An error message to be included in the error log when the assertion fails. When
 * omitted, the message will be: "Assert '[condition]' failed"
 * @param ...      Optional variadic arguments that can be used for further formatting the error message (if using
 * the message parameter).
 *
 * Example usage:
 * @code
 * int divisor = 0;
 * KEN_CORE_ASSERT(divisor != 0, "Divide by zero error!");
 * int result = dividend / divisor;
 * @endcode
 */
#define KEN_CORE_ASSERT(...) KEN_STMT(KEN_INT_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))

/**
 * @brief Marks a code path as unreachable. In debug builds, fires a core assertion and triggers a debugbreak before
 * calling std::unreachable(). In release builds, calls std::unreachable() directly.
 */
#define KEN_UNREACHABLE() KEN_STMT(KEN_CORE_ASSERT(false, "Unreachable code reached"); std::unreachable();)

#else
// In release: the condition is never evaluated; [[assume]] donates it to the optimizer as an axiom.
#define KEN_ASSERT(check, ...) [[assume(check)]]
#define KEN_CORE_ASSERT(check, ...) [[assume(check)]]

/** @copydoc KEN_UNREACHABLE */
#define KEN_UNREACHABLE() std::unreachable()
#endif
// NOLINTEND(cppcoreguidelines-macro-usage)
