#pragma once

#include <chrono>

#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/** Internal time representation. Nanosecond precision; not part of the public update API. */
using duration_t = std::chrono::nanoseconds;

/**
 * Thin wrapper around a time duration passed to Layer::on_update and GameComponent::update.
 *
 * Decouples the engine update API from the internal chrono precision: callers express time
 * as seconds or milliseconds without depending on std::chrono::nanoseconds directly.
 *
 * @ingroup ken
 */
class Timestep {
 public:
  /** Constructs a Timestep from an internal duration. */
  explicit Timestep(duration_t d) : duration_(d) {}

  /** @return Elapsed time in seconds. */
  [[nodiscard]] double seconds() const { return std::chrono::duration<double>(duration_).count(); }

  /** @return Elapsed time in milliseconds. */
  [[nodiscard]] double milliseconds() const { return std::chrono::duration<double, std::milli>(duration_).count(); }

  /** @return The underlying duration, for code that still requires the raw chrono type. */
  [[nodiscard]] duration_t duration() const { return duration_; }

 private:
  duration_t duration_;
};

}  // namespace kEn
