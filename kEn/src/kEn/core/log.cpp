#include "log.hpp"

#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>
#include <vector>

#ifdef TRACY_ENABLE
#include <spdlog/details/log_msg.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/base_sink.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mutex>

#include <tracy/Tracy.hpp>  // NOLINT(misc-include-cleaner)
#endif

namespace kEn {

namespace {

#ifdef TRACY_ENABLE
// NOLINTBEGIN(misc-include-cleaner)
[[nodiscard]] tracy::MessageSeverity tracy_severity(spdlog::level::level_enum level) noexcept {
  switch (level) {
    case spdlog::level::trace:
      return tracy::MessageSeverity::Trace;
    case spdlog::level::debug:
      return tracy::MessageSeverity::Debug;
    case spdlog::level::warn:
      return tracy::MessageSeverity::Warning;
    case spdlog::level::err:
      return tracy::MessageSeverity::Error;
    case spdlog::level::critical:
      return tracy::MessageSeverity::Fatal;
    default:
      return tracy::MessageSeverity::Info;
  }
}

/** @brief Sends log records to the profiler as "[logger] message" with their severity; dropped while unavailable. */
class TracySink final : public spdlog::sinks::base_sink<std::mutex> {
 public:
  TracySink()
      : spdlog::sinks::base_sink<std::mutex>(
            std::make_unique<spdlog::pattern_formatter>("[%n] %v", spdlog::pattern_time_type::local, "")) {}

 protected:
  void sink_it_(const spdlog::details::log_msg& msg) override {
    if (!tracy::ProfilerAvailable()) {
      return;
    }

    spdlog::memory_buf_t text;
    formatter_->format(msg, text);
    const std::size_t size = std::min<std::size_t>(text.size(), std::numeric_limits<std::uint16_t>::max() - 1);
    tracy::Profiler::LogString(tracy::MessageSourceType::User, tracy_severity(msg.level), 0, 0, size, text.data());
  }

  void flush_() override {}
};
// NOLINTEND(misc-include-cleaner)
#endif

/** @brief A new console sink, followed by a new profiler sink with TRACY_ENABLE. */
std::vector<spdlog::sink_ptr> make_sinks() {
  const auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
#ifdef KEN_DEBUG_BUILD
  console->set_pattern("%^[%T][%n/%l][%s:%# %!]: %v%$");
#else
  console->set_pattern("%^[%T][%n/%l]: %v%$");
#endif

#ifdef TRACY_ENABLE
  return {console, std::make_shared<TracySink>()};
#else
  return {console};
#endif
}

}  // namespace

spdlog::logger& Log::core_logger() {
  // intentionally leaked (never destroyed): static destructors (e.g. Registry) may still log during exit
  static auto* const kInstance = [] {
    const auto sinks = make_sinks();
    auto* logger = new spdlog::logger("kEn", sinks.begin(), sinks.end());  // NOLINT(cppcoreguidelines-owning-memory)
#ifdef KEN_DEBUG_BUILD
    logger->set_level(spdlog::level::trace);
#else
    logger->set_level(spdlog::level::warn);
#endif

    return logger;
  }();
  return *kInstance;
}

spdlog::logger& Log::client_logger() {
  // intentionally leaked (never destroyed): static destructors (e.g. Registry) may still log during exit
  static auto* const kInstance = [] {
    const auto sinks = make_sinks();
    auto* logger = new spdlog::logger("Client", sinks.begin(), sinks.end());  // NOLINT(cppcoreguidelines-owning-memory)
#ifdef KEN_DEBUG_BUILD
    logger->set_level(spdlog::level::trace);
#else
    logger->set_level(spdlog::level::warn);
#endif

    return logger;
  }();
  return *kInstance;
}

}  // namespace kEn
