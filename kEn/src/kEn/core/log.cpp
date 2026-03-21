#include "log.hpp"

#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>

namespace kEn {

spdlog::logger& Log::core_logger() {
  static const auto kInstance = []() {
    auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
#ifdef KEN_DEBUG_BUILD
    sink->set_pattern("%^[%T] [%n/%l] [%s:%# %!]: %v%$");
#else
    sink->set_pattern("%^[%T] [%n/%l]: %v%$");
#endif

    auto logger = std::make_shared<spdlog::logger>("kEn", sink);
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
  static const auto kInstance = []() {
    auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
#ifdef KEN_DEBUG_BUILD
    sink->set_pattern("%^[%T] [%n/%l] [%s:%# %!]: %v%$");
#else
    sink->set_pattern("%^[%T] [%n/%l]: %v%$");
#endif

    auto logger = std::make_shared<spdlog::logger>("Client", sink);
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
