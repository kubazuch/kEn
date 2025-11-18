#include "log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace kEn {
std::shared_ptr<spdlog::logger> Log::core_logger_;
std::shared_ptr<spdlog::logger> Log::client_logger_;

void Log::init() {
  spdlog::set_pattern("%^[%T] [%n/%l]: %v%$");
  core_logger_ = spdlog::stdout_color_mt("kEn");
  core_logger_->set_level(spdlog::level::trace);

  client_logger_ = spdlog::stdout_color_mt("Game");
  client_logger_->set_level(spdlog::level::trace);
}
}  // namespace kEn
