#pragma once

/** @file
 *  @ingroup ken
 *
 *  Tracy profiler instrumentation. With the TRACY_ENABLE CMake option off every macro expands to nothing and Tracy
 *  is not needed. Scope macros open a zone that ends with the enclosing block; several may share a block if they are
 *  on different lines.
 */

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#ifdef TRACY_ENABLE
#include <memory>

#include <kEn/core/core.hpp>

#include <tracy/Tracy.hpp>

namespace tracy {
class GpuCtxScope;
}  // namespace tracy

namespace kEn::profile {

/** @brief GPU timer zone on the main graphics context, open for the object's lifetime. Use KEN_PROFILE_GPU_SCOPE. */
class GpuZone final {
 public:
  explicit GpuZone(const tracy::SourceLocationData* location);
  ~GpuZone();

  DELETE_COPY_MOVE(GpuZone);

 private:
  std::unique_ptr<tracy::GpuCtxScope> scope_;
};

}  // namespace kEn::profile

/** @brief CPU zone named after the enclosing function. */
#define KEN_PROFILE_FUNCTION() ZoneNamed(TracyConcat(ken_profile_zone_, TracyLine), true)

/** @brief CPU zone with a string literal name. */
#define KEN_PROFILE_SCOPE(name) ZoneNamedN(TracyConcat(ken_profile_zone_, TracyLine), name, true)

/** @brief CPU zone with a runtime name (null-terminated, copied). */
#define KEN_PROFILE_SCOPE_DYNAMIC(name) ZoneTransientN(TracyConcat(ken_profile_zone_, TracyLine), name, true)

/** @brief GPU timer zone with a string literal name; the main graphics context must be current. */
#define KEN_PROFILE_GPU_SCOPE(name)                                                             \
  static constexpr tracy::SourceLocationData TracyConcat(ken_profile_gpu_location_, TracyLine){ \
      name, TracyFunction, TracyFile, TracyLine, 0};                                            \
  const ::kEn::profile::GpuZone TracyConcat(ken_profile_gpu_zone_, TracyLine) {                 \
    &TracyConcat(ken_profile_gpu_location_, TracyLine)                                          \
  }

/** @brief Adds a sample to a plot; name is a string literal (plots are keyed by its address). */
#define KEN_PROFILE_PLOT(name, value) TracyPlot(name, value)

/** @brief Marks the end of a frame. */
#define KEN_PROFILE_FRAME() FrameMark

/** @brief Names the calling thread in the profiler. */
#define KEN_PROFILE_THREAD(name) tracy::SetThreadName(name)

#else
#define KEN_PROFILE_FUNCTION()
#define KEN_PROFILE_SCOPE(name)
#define KEN_PROFILE_SCOPE_DYNAMIC(name)
#define KEN_PROFILE_GPU_SCOPE(name)
#define KEN_PROFILE_PLOT(name, value)
#define KEN_PROFILE_FRAME()
#define KEN_PROFILE_THREAD(name)
#endif
// NOLINTEND(cppcoreguidelines-macro-usage)
