#include "log.hpp"

#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>

namespace kEn {

spdlog::logger& Log::core_logger() {
  // intentionally leaked (never destroyed): static destructors (e.g. Registry) may still log during exit
  static auto* const kInstance = [] {
    const auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
#ifdef KEN_DEBUG_BUILD
    sink->set_pattern("%^[%T][%n/%l][%s:%# %!]: %v%$");
#else
    sink->set_pattern("%^[%T][%n/%l]: %v%$");
#endif

    auto* logger = new spdlog::logger("kEn", sink);  // NOLINT(cppcoreguidelines-owning-memory)
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
    const auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
#ifdef KEN_DEBUG_BUILD
    sink->set_pattern("%^[%T][%n/%l][%s:%# %!]: %v%$");
#else
    sink->set_pattern("%^[%T][%n/%l]: %v%$");
#endif

    auto* logger = new spdlog::logger("Client", sink);  // NOLINT(cppcoreguidelines-owning-memory)
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
