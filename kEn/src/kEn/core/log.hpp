#pragma once

#include <kEn/core/core.hpp>

#pragma warning(push, 0)
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace kEn {

class Log {
 public:
  static void init();

  inline static std::shared_ptr<spdlog::logger>& core_logger() { return core_logger_; }
  inline static std::shared_ptr<spdlog::logger>& client_logger() { return client_logger_; }

 private:
  static std::shared_ptr<spdlog::logger> core_logger_;
  static std::shared_ptr<spdlog::logger> client_logger_;
};
}  // namespace kEn

// Core macros
#define KEN_CORE_TRACE(...) ::kEn::Log::core_logger()->trace(__VA_ARGS__)
#define KEN_CORE_INFO(...) ::kEn::Log::core_logger()->info(__VA_ARGS__)
#define KEN_CORE_WARN(...) ::kEn::Log::core_logger()->warn(__VA_ARGS__)
#define KEN_CORE_ERROR(...) ::kEn::Log::core_logger()->error(__VA_ARGS__)
#define KEN_CORE_CRITICAL(...) ::kEn::Log::core_logger()->critical(__VA_ARGS__)

// Client macros
#define KEN_TRACE(...) ::kEn::Log::client_logger()->trace(__VA_ARGS__)
#define KEN_INFO(...) ::kEn::Log::client_logger()->info(__VA_ARGS__)
#define KEN_WARN(...) ::kEn::Log::client_logger()->warn(__VA_ARGS__)
#define KEN_ERROR(...) ::kEn::Log::client_logger()->error(__VA_ARGS__)
#define KEN_CRITICAL(...) ::kEn::Log::client_logger()->critical(__VA_ARGS__)

// Debug
#ifdef _KEN_DEBUG
#define KEN_CORE_DEBUG(...) ::kEn::Log::core_logger()->debug(__VA_ARGS__)
#define KEN_DEBUG(...) ::kEn::Log::client_logger()->debug(__VA_ARGS__)
#else
#define KEN_CORE_DEBUG(...)
#define KEN_DEBUG(...)
#endif
