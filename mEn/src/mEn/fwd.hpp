#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <mEn/config.hpp>

namespace mEn {

using length_t = std::size_t;

template <typename T>
concept Scalar = std::is_arithmetic_v<T>;

template <typename T>
concept Floating = std::is_floating_point_v<T>;

template <typename T>
concept Integral = std::is_integral_v<T>;

template <length_t L, typename T>
struct vec;

template <length_t N, typename T>
struct mat;

template <typename T>
struct qua;

template <typename>
inline constexpr bool is_vec_v = false;  // NOLINT(readability-identifier-naming)

template <length_t L, typename T>
inline constexpr bool is_vec_v<vec<L, T>> = true;  // NOLINT(readability-identifier-naming)

template <typename T>
concept VecType = is_vec_v<T>;

template <typename T>
concept VecOrScalar = Scalar<T> || VecType<T>;

template <typename T>
struct scalar_of {  // NOLINT(readability-identifier-naming)
  using type = T;
};

template <length_t L, typename T>
struct scalar_of<vec<L, T>> {
  using type = T;
};

template <typename T>
using scalar_of_t = typename scalar_of<T>::type;

template <typename T>
struct bool_result {  // NOLINT(readability-identifier-naming)
  using type = bool;
};

template <length_t L, typename T>
struct bool_result<vec<L, T>> {
  using type = vec<L, bool>;
};

template <typename T>
using bool_result_t = typename bool_result<T>::type;

using default_scalar_t = config::default_scalar_t;

using Vec2 = vec<2, default_scalar_t>;
using Vec3 = vec<3, default_scalar_t>;
using Vec4 = vec<4, default_scalar_t>;

using Mat3 = mat<3, default_scalar_t>;
using Mat4 = mat<4, default_scalar_t>;

using Quat = qua<default_scalar_t>;

using IVec2 = vec<2, std::int32_t>;
using IVec3 = vec<3, std::int32_t>;
using IVec4 = vec<4, std::int32_t>;

using UVec2 = vec<2, std::uint32_t>;
using UVec3 = vec<3, std::uint32_t>;
using UVec4 = vec<4, std::uint32_t>;

}  // namespace mEn
