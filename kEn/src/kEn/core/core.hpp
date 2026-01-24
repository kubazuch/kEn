// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#pragma once

#ifdef _KEN_DEBUG
#define KEN_ENABLE_ASSERTS
#endif

#define KEN_EXPAND(X) X
#define KEN_SEMICOLON(X) \
  do {                   \
    X                    \
  } while (0)
#define KEN_STRINGIFY(X) #X

#define VIRTUAL_FIVE(class)                         \
  class(const class&)                     = delete; \
  class(class&&)                          = delete; \
  class& operator=(const class&) noexcept = delete; \
  class& operator=(class&&) noexcept      = delete

#define EVAL0(...) __VA_ARGS__
#define EVAL1(...) EVAL0(EVAL0(EVAL0(__VA_ARGS__)))
#define EVAL2(...) EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL3(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL4(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL(...) EVAL4(EVAL4(EVAL4(__VA_ARGS__)))

#define MAP_OUT
#define MAP_END(...)
#define MAP_GET_END() 0, MAP_END
#define MAP_NEXT0(item, next, ...) next MAP_OUT
#define MAP_NEXT1(item, next) MAP_NEXT0(item, next, 0)
#define MAP_NEXT(item, next) MAP_NEXT1(MAP_GET_END item, next)

#define MAP0(f, x, peek, ...) f(x) MAP_NEXT(peek, MAP1)(f, peek, __VA_ARGS__)
#define MAP1(f, x, peek, ...) f(x) MAP_NEXT(peek, MAP0)(f, peek, __VA_ARGS__)
#define MAP(f, ...) EVAL(MAP1(f, __VA_ARGS__, (), 0))

template <class>
inline constexpr bool always_false_v = false;  // NOLINT

// NOLINTEND(cppcoreguidelines-macro-usage)