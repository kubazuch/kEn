#include "log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace kEn {

spdlog::logger& Log::core_logger() {
  static const auto kInstance = []() {
    auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    sink->set_pattern("%^[%T] [%n/%l]: %v%$");
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
    sink->set_pattern("%^[%T] [%n/%l]: %v%$");
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
