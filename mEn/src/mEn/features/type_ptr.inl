#include <mEn/mat3.hpp>
#include <mEn/mat4.hpp>
#include <mEn/quat.hpp>
#include <mEn/vec2.hpp>
#include <mEn/vec3.hpp>
#include <mEn/vec4.hpp>

namespace mEn {

template <typename T>
MEN_FORCE_INLINE constexpr const T* value_ptr(const vec<2, T>& v) noexcept {
  return &(v.x);
}

template <typename T>
MEN_FORCE_INLINE constexpr T* value_ptr(vec<2, T>& v) noexcept {
  return &(v.x);
}

template <typename T>
MEN_FORCE_INLINE constexpr const T* value_ptr(const vec<3, T>& v) noexcept {
  return &(v.x);
}

template <typename T>
MEN_FORCE_INLINE constexpr T* value_ptr(vec<3, T>& v) noexcept {
  return &(v.x);
}

template <typename T>
MEN_FORCE_INLINE constexpr const T* value_ptr(const vec<4, T>& v) noexcept {
  return &(v.x);
}

template <typename T>
MEN_FORCE_INLINE constexpr T* value_ptr(vec<4, T>& v) noexcept {
  return &(v.x);
}

template <typename T>
MEN_FORCE_INLINE constexpr const T* value_ptr(const mat<3, T>& m) noexcept {
  return &(m[0].x);
}

template <typename T>
MEN_FORCE_INLINE constexpr T* value_ptr(mat<3, T>& m) noexcept {
  return &(m[0].x);
}

template <typename T>
MEN_FORCE_INLINE constexpr const T* value_ptr(const mat<4, T>& m) noexcept {
  return &(m[0].x);
}

template <typename T>
MEN_FORCE_INLINE constexpr T* value_ptr(mat<4, T>& m) noexcept {
  return &(m[0].x);
}

template <typename T>
MEN_FORCE_INLINE constexpr const T* value_ptr(const qua<T>& q) noexcept {
  return &(q[0]);
}

template <typename T>
MEN_FORCE_INLINE constexpr T* value_ptr(qua<T>& q) noexcept {
  return &(q[0]);
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<2, T> make_vec2(const T* ptr) noexcept {
  return vec<2, T>{ptr[0], ptr[1]};
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<3, T> make_vec3(const T* ptr) noexcept {
  return vec<3, T>{ptr[0], ptr[1], ptr[2]};
}

template <typename T>
MEN_FORCE_INLINE constexpr vec<4, T> make_vec4(const T* ptr) noexcept {
  return vec<4, T>{ptr[0], ptr[1], ptr[2], ptr[3]};
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<3, T> make_mat3(const T* ptr) noexcept {
  return mat<3, T>{ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7], ptr[8]};
}

template <typename T>
MEN_FORCE_INLINE constexpr mat<4, T> make_mat4(const T* ptr) noexcept {
  return mat<4, T>{ptr[0], ptr[1], ptr[2],  ptr[3],  ptr[4],  ptr[5],  ptr[6],  ptr[7],
                   ptr[8], ptr[9], ptr[10], ptr[11], ptr[12], ptr[13], ptr[14], ptr[15]};
}

template <typename T>
MEN_FORCE_INLINE constexpr qua<T> make_quat(const T* ptr) noexcept {
  // Memory layout from value_ptr is [x, y, z, w]; constructor is qua(w, x, y, z).
  return qua<T>{ptr[3], ptr[0], ptr[1], ptr[2]};
}

}  // namespace mEn