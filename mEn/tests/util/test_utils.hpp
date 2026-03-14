#pragma once

#include <glm/glm.hpp>

#define EX_PP_EXPAND(x) x
#define EX_SELECT(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, NAME, ...) NAME

// Dispatcher
#define EX(T, ...)                                                                                                 \
  EX_PP_EXPAND(EX_SELECT(__VA_ARGS__, EX_MAT_4, EX_INVALID, EX_INVALID, EX_INVALID, EX_INVALID, EX_INVALID,        \
                         EX_INVALID, EX_MAT_3, EX_INVALID, EX_INVALID, EX_INVALID, EX_INVALID, EX_VEC_4, EX_VEC_3, \
                         EX_VEC_2, EX_INVALID)(T, __VA_ARGS__))

// Supported arities
#define EX_VEC_2(T, a, b) glm::vec<2, T>(T{a}, T{b})

#define EX_VEC_3(T, a, b, c) glm::vec<3, T>(T{a}, T{b}, T{c})

#define EX_VEC_4(T, a, b, c, d) glm::vec<4, T>(T{a}, T{b}, T{c}, T{d})

#define EX_MAT_3(T, ...) glm::mat<3, 3, T>(__VA_ARGS__)

#define EX_MAT_4(T, ...) glm::mat<4, 4, T>(__VA_ARGS__)

#define EX_INVALID(T, ...) static_assert(sizeof(T) == 0, "EX supports only 2, 3, 4, 9, or 16 arguments")