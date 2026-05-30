#pragma once

#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn::util {

/**
 * @brief Visitor helper that merges multiple callable objects into one overload set.
 *
 * Constructs an object that inherits all operator() overloads from each base,
 * enabling multi-lambda @c std::visit without explicit visitor structs.
 *
 * @code
 * std::visit(overloaded{
 *     [](int x)   { ... },
 *     [](float x) { ... },
 * }, variant_value);
 * @endcode
 *
 * @tparam Ts Callable types whose operator() overloads are merged.
 */
template <class... Ts>
struct overloaded : Ts... {  // NOLINT(readability-identifier-naming)
  using Ts::operator()...;
};

}  // namespace kEn::util
