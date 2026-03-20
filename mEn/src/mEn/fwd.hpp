#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <mEn/config.hpp>

/// @file
/// @ingroup men

namespace mEn {

/// @brief Unsigned integer type used for vector/matrix dimension parameters.
using length_t = std::size_t;

/// @brief Satisfied by any arithmetic type (integral or floating-point).
template <typename T>
concept Scalar = std::is_arithmetic_v<T>;

/// @brief Satisfied by floating-point types only.
template <typename T>
concept Floating = std::is_floating_point_v<T>;

/// @brief Satisfied by integral types only.
template <typename T>
concept Integral = std::is_integral_v<T>;

/// @brief Primary template for a fixed-length vector of scalar @p T.
/// @tparam L Number of components.
/// @tparam T Component scalar type.
template <length_t L, typename T>
struct vec;

/// @brief Primary template for an NxN square matrix of scalar @p T.
/// @tparam N Row/column count.
/// @tparam T Component scalar type.
template <length_t N, typename T>
struct mat;

/// @brief Primary template for a quaternion of scalar @p T.
/// @tparam T Component scalar type.
template <typename T>
struct qua;  // NOLINT(readability-identifier-naming)

/// @brief Type trait: @c true for any @c vec<L,T> specialization, @c false otherwise.
template <typename>
inline constexpr bool is_vec_v = false;  // NOLINT(readability-identifier-naming)

template <length_t L, typename T>
inline constexpr bool is_vec_v<vec<L, T>> = true;  // NOLINT(readability-identifier-naming)

/// @brief Satisfied by any specialization of @c vec<L,T>.
template <typename T>
concept VecType = is_vec_v<T>;

/// @brief Satisfied by a @c Scalar or a @c VecType.
///
/// Used to constrain generic free functions that operate identically
/// on scalars and vectors (e.g. @c abs, @c sqrt, @c clamp).
template <typename T>
concept VecOrScalar = Scalar<T> || VecType<T>;

/// @brief Extracts the scalar component type from a @c VecOrScalar.
///
/// For a @c vec<L,T> yields @c T; for a plain scalar @c T yields @c T.
/// @tparam T A @c VecOrScalar type.
template <typename T>
struct scalar_of {  // NOLINT(readability-identifier-naming)
  using type = T;
};

template <length_t L, typename T>
struct scalar_of<vec<L, T>> {
  using type = T;
};

/// @brief Convenience alias for @c scalar_of<T>::type.
template <typename T>
using scalar_of_t = scalar_of<T>::type;

/// @brief Maps a @c VecOrScalar to its boolean counterpart.
///
/// For a @c vec<L,T> yields @c vec<L,bool>; for a plain scalar yields @c bool.
/// Used as the return type for component-wise relational functions.
/// @tparam T A @c VecOrScalar type.
template <typename T>
struct bool_result {  // NOLINT(readability-identifier-naming)
  using type = bool;
};

template <length_t L, typename T>
struct bool_result<vec<L, T>> {
  using type = vec<L, bool>;
};

/// @brief Convenience alias for @c bool_result<T>::type.
template <typename T>
using bool_result_t = bool_result<T>::type;

/// @brief The default scalar type, equal to @c config::default_scalar_t (@c float).
using default_scalar_t = config::default_scalar_t;

/// @name Default-precision vector aliases
/// @{
using Vec2 = vec<2, default_scalar_t>;  ///< 2-component float vector.
using Vec3 = vec<3, default_scalar_t>;  ///< 3-component float vector.
using Vec4 = vec<4, default_scalar_t>;  ///< 4-component float vector.
/// @}

/// @name Default-precision matrix aliases
/// @{
using Mat3 = mat<3, default_scalar_t>;  ///< 3x3 float matrix (column-major).
using Mat4 = mat<4, default_scalar_t>;  ///< 4x4 float matrix (column-major).
/// @}

/// @brief Default-precision quaternion alias.
using Quat = qua<default_scalar_t>;

/// @name Signed 32-bit integer vector aliases
/// @{
using IVec2 = vec<2, std::int32_t>;  ///< 2-component int32 vector.
using IVec3 = vec<3, std::int32_t>;  ///< 3-component int32 vector.
using IVec4 = vec<4, std::int32_t>;  ///< 4-component int32 vector.
/// @}

/// @name Unsigned 32-bit integer vector aliases
/// @{
using UVec2 = vec<2, std::uint32_t>;  ///< 2-component uint32 vector.
using UVec3 = vec<3, std::uint32_t>;  ///< 3-component uint32 vector.
using UVec4 = vec<4, std::uint32_t>;  ///< 4-component uint32 vector.
/// @}

}  // namespace mEn
