#pragma once

#include <concepts>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

#include <mEn/config.hpp>
#include <mEn/fwd.hpp>

namespace mEn {

namespace detail {
// NOLINTBEGIN(readability-identifier-naming)
template <class T>
using lane_arg_t = std::add_lvalue_reference_t<std::add_const_t<T>>;

// Unary: out[i] = f(v[i])
template <auto Func, length_t L, class T>
  requires std::invocable<decltype(Func), lane_arg_t<T>>
[[nodiscard]] MEN_FORCE_INLINE constexpr auto map(const vec<L, T>& v) noexcept(
    noexcept(std::invoke(Func, std::declval<lane_arg_t<T>>())))
    -> vec<L, std::remove_cvref_t<std::invoke_result_t<decltype(Func), lane_arg_t<T>>>> {
  using R = std::remove_cvref_t<std::invoke_result_t<decltype(Func), lane_arg_t<T>>>;

  return [&]<std::size_t... I>(std::index_sequence<I...>) -> vec<L, R> {  // NOLINT(*-named-parameter)
    return vec<L, R>{std::invoke(Func, v[I])...};
  }(std::make_index_sequence<static_cast<std::size_t>(L)>{});
}

template <class T>
struct is_vec : std::false_type {};

template <length_t L, class T>
struct is_vec<vec<L, T>> : std::true_type {};

template <class T>
inline constexpr bool is_vec_v = is_vec<std::remove_cvref_t<T>>::value;

template <class T>
struct vec_traits;

template <length_t L, class T>
struct vec_traits<vec<L, T>> {
  static constexpr length_t len = L;
  using value_type              = T;
};

template <class... Ts>
struct first_vec_clean;

template <length_t L, class T, class... Rest>
struct first_vec_clean<::mEn::vec<L, T>, Rest...> {
  using type = ::mEn::vec<L, T>;
};

template <class First, class... Rest>
struct first_vec_clean<First, Rest...> : first_vec_clean<Rest...> {};

template <class... Args>
using first_vec_t = first_vec_clean<std::remove_cvref_t<Args>...>::type;

template <class X, length_t L, bool = is_vec_v<X>>
struct len_ok : std::true_type {};

template <class X, length_t L>
struct len_ok<X, L, true> : std::bool_constant<(vec_traits<std::remove_cvref_t<X>>::len == L)> {};

template <class... Args>
  requires(is_vec_v<Args> || ...)
struct common_length {
  using V = first_vec_t<Args...>;
  static_assert(is_vec_v<V>, "zip/reduce: at least one argument must be a vec");

  static constexpr length_t value = vec_traits<V>::len;
  static_assert((len_ok<Args, value>::value && ...), "zip: all vec arguments must have the same length");
};

template <std::size_t I, class T>
MEN_FORCE_INLINE constexpr decltype(auto) pick(const T& x) noexcept {
  if constexpr (is_vec_v<T>) {
    return x[I];
  } else {
    return x;
  }
}

template <class T>
using pick0_t = decltype(pick<0>(std::declval<const T&>()));

// Multinary (vec/scalar, ...): out[i] = f(a[i], s, ...)
template <auto Func, class... Args>
  requires(is_vec_v<Args> || ...) && std::invocable<decltype(Func), pick0_t<Args>...>
[[nodiscard]] MEN_FORCE_INLINE constexpr auto zip(const Args&... args) noexcept(noexcept(std::invoke(Func,
                                                                                                     pick<0>(args)...)))
    -> vec<common_length<Args...>::value, std::remove_cvref_t<std::invoke_result_t<decltype(Func), pick0_t<Args>...>>> {
  static constexpr length_t L = common_length<Args...>::value;

  using R = std::remove_cvref_t<std::invoke_result_t<decltype(Func), pick0_t<Args>...>>;

  return [&]<std::size_t... I>(std::index_sequence<I...>) -> vec<L, R> {
    auto lane = [&]<std::size_t J>() -> R { return std::invoke(Func, pick<J>(args)...); };

    // clang-format off
    return vec<L, R>{lane.template operator()<I>()...};
    // clang-format on
  }(std::make_index_sequence<static_cast<std::size_t>(L)>{});
}

// Fold: result = f(f(...f(init, v[0]), v[1]...), v[L-1])
template <auto Func, length_t L, class T, class Acc>
  requires std::invocable<decltype(Func), Acc, lane_arg_t<T>>
[[nodiscard]] MEN_FORCE_INLINE constexpr auto reduce(const vec<L, T>& v, Acc init) noexcept(
    noexcept(std::invoke(Func, std::declval<Acc>(), std::declval<lane_arg_t<T>>()))) -> Acc {
  return [&]<std::size_t... I>(std::index_sequence<I...>) -> Acc {
    ((init = std::invoke(Func, std::move(init), v[I])), ...);
    return init;
  }(std::make_index_sequence<static_cast<std::size_t>(L)>{});
}

// NOLINTEND(readability-identifier-naming)
}  // namespace detail

}  // namespace mEn
