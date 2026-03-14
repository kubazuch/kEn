#pragma once

#include <mEn/fwd.hpp>

namespace mEn {

template <typename T>
[[nodiscard]] bool decompose(const mat<4, T>& model, vec<3, T>& translation, qua<T>& rotation,
                             vec<3, T>& scale) noexcept;

}  // namespace mEn

#include "mat_decompose.inl"
