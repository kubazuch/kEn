#include "opengl_profile.hpp"

#ifdef TRACY_ENABLE
#include <glad/gl.h>  // IWYU pragma: keep

#include <cstddef>
#include <cstdint>
#include <memory>

#include <kEn/core/profile.hpp>

#include <tracy/TracyOpenGL.hpp>
#endif

// NOLINTBEGIN(misc-include-cleaner)
namespace kEn::profile {

#ifdef TRACY_ENABLE
namespace {

constexpr const char* kDrawCallsPlot = "Draw calls";
constexpr const char* kUploadsPlot   = "GPU uploads";

struct FrameCounters {
  std::int64_t draw_calls   = 0;
  std::int64_t upload_bytes = 0;
};

// main (OpenGL) thread only
FrameCounters& frame_counters() {
  static FrameCounters counters;
  return counters;
}

}  // namespace

void count_draw() { ++frame_counters().draw_calls; }

void count_upload(std::size_t bytes) { frame_counters().upload_bytes += static_cast<std::int64_t>(bytes); }

GpuZone::GpuZone(const tracy::SourceLocationData* location)
    : scope_(std::make_unique<tracy::GpuCtxScope>(location, true)) {}

GpuZone::~GpuZone() = default;
#endif

void create_gpu_context() {
#ifdef TRACY_ENABLE
  TracyGpuContext;
  TracyGpuContextName("OpenGL", 6);
  TracyPlotConfig(kDrawCallsPlot, tracy::PlotFormatType::Number, true, true, 0);
  TracyPlotConfig(kUploadsPlot, tracy::PlotFormatType::Memory, true, true, 0);
#endif
}

void end_frame() {
#ifdef TRACY_ENABLE
  TracyGpuCollect;

  FrameCounters& counters = frame_counters();
  KEN_PROFILE_PLOT(kDrawCallsPlot, counters.draw_calls);
  KEN_PROFILE_PLOT(kUploadsPlot, counters.upload_bytes);
  counters = {};
#endif
}

}  // namespace kEn::profile
// NOLINTEND(misc-include-cleaner)
