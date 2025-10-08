#pragma once
#include "functions.hpp"

namespace mEn {

template <typename T>
inline constexpr T min(T a, T b) {
  return (a < b) ? a : b;
}

template <typename T>
inline constexpr T max(T a, T b) {
  return (a < b) ? b : a;
}

template <typename T>
inline constexpr T clamp(T val, T minVal, T maxVal) {
  return min(max(val, minVal), maxVal);
}

template <typename T>
inline const T* value_ptr(const vec<2, T>& vec) {
  return &vec.x;
}

template <typename T>
inline T* value_ptr(vec<2, T>& vec) {
  return &vec.x;
}

template <typename T>
inline const T* value_ptr(const vec<3, T>& vec) {
  return &vec.x;
}

template <typename T>
inline T* value_ptr(vec<3, T>& vec) {
  return &vec.x;
}

template <typename T>
inline const T* value_ptr(const vec<4, T>& vec) {
  return &vec.x;
}

template <typename T>
inline T* value_ptr(vec<4, T>& vec) {
  return &vec.x;
}

template <typename T>
inline const T* value_ptr(const mat<3, T>& mat) {
  return &mat[0].x;
}

template <typename T>
inline T* value_ptr(mat<3, T>& mat) {
  return &mat[0].x;
}

template <typename T>
inline const T* value_ptr(const mat<4, T>& mat) {
  return &mat[0].x;
}

template <typename T>
inline T* value_ptr(mat<4, T>& mat) {
  return &mat[0].x;
}

inline const float* value_ptr(const Quat& quat) { return &quat.x; }

inline const float* value_ptr(Quat& quat) { return &quat.x; }

template <typename T>
inline constexpr T pi() {
  return static_cast<T>(3.14159265358979323846);
}

}  // namespace mEn