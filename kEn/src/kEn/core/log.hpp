#pragma once

#include <kEn/core/core.hpp>

#pragma warning(push, 0)
#include <spdlog/common.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief Static logging facade backed by spdlog.
 *
 * Provides two named loggers: one for engine-internal (core) messages and one
 * for client (application) code. Both loggers are initialized on first use.
 * Prefer the KEN_CORE_* / KEN_* macros over calling these methods directly.
 */
class Log {
 public:
  /**
   * @brief Returns the engine-internal logger.
   * @return Reference to the spdlog logger used by KEN_CORE_* macros.
   */
  static spdlog::logger& core_logger();

  /**
   * @brief Returns the client/application logger.
   * @return Reference to the spdlog logger used by KEN_* macros.
   */
  static spdlog::logger& client_logger();
};

}  // namespace kEn

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

/** @brief Resolves to a populated `spdlog::source_loc` in debug builds, empty otherwise. */
#ifdef KEN_DEBUG_BUILD
#define KEN_SOURCE_LOC \
  spdlog::source_loc { __FILE__, __LINE__, __FUNCTION__ }
#else
#define KEN_SOURCE_LOC \
  spdlog::source_loc {}
#endif

/**
 * @defgroup core_log Engine logging macros (KEN_CORE_*)
 * Log through the engine-internal logger. Available at all build types except
 * KEN_CORE_DEBUG, which is stripped in non-debug builds.
 * @{
 */
#define KEN_CORE_TRACE(...) ::kEn::Log::core_logger().log(KEN_SOURCE_LOC, spdlog::level::trace, __VA_ARGS__)
#define KEN_CORE_INFO(...) ::kEn::Log::core_logger().log(KEN_SOURCE_LOC, spdlog::level::info, __VA_ARGS__)
#define KEN_CORE_WARN(...) ::kEn::Log::core_logger().log(KEN_SOURCE_LOC, spdlog::level::warn, __VA_ARGS__)
#define KEN_CORE_ERROR(...) ::kEn::Log::core_logger().log(KEN_SOURCE_LOC, spdlog::level::err, __VA_ARGS__)
#define KEN_CORE_CRITICAL(...) ::kEn::Log::core_logger().log(KEN_SOURCE_LOC, spdlog::level::critical, __VA_ARGS__)
/** @} */

/**
 * @defgroup client_log Client logging macros (KEN_*)
 * Log through the application/client logger. Available at all build types
 * except KEN_DEBUG, which is stripped in non-debug builds.
 * @{
 */
#define KEN_TRACE(...) ::kEn::Log::client_logger().log(KEN_SOURCE_LOC, spdlog::level::trace, __VA_ARGS__)
#define KEN_INFO(...) ::kEn::Log::client_logger().log(KEN_SOURCE_LOC, spdlog::level::info, __VA_ARGS__)
#define KEN_WARN(...) ::kEn::Log::client_logger().log(KEN_SOURCE_LOC, spdlog::level::warn, __VA_ARGS__)
#define KEN_ERROR(...) ::kEn::Log::client_logger().log(KEN_SOURCE_LOC, spdlog::level::err, __VA_ARGS__)
#define KEN_CRITICAL(...) ::kEn::Log::client_logger().log(KEN_SOURCE_LOC, spdlog::level::critical, __VA_ARGS__)
/** @} */

/**
 * @def KEN_CORE_DEBUG
 * @brief Engine debug log — compiled out in non-debug builds.
 * @ingroup core_log
 */
/**
 * @def KEN_DEBUG
 * @brief Client debug log — compiled out in non-debug builds.
 * @ingroup client_log
 */
#ifdef KEN_DEBUG_BUILD
#define KEN_CORE_DEBUG(...) ::kEn::Log::core_logger().log(KEN_SOURCE_LOC, spdlog::level::debug, __VA_ARGS__)
#define KEN_DEBUG(...) ::kEn::Log::client_logger().log(KEN_SOURCE_LOC, spdlog::level::debug, __VA_ARGS__)
#else
#define KEN_CORE_DEBUG(...)
#define KEN_DEBUG(...)
#endif
// NOLINTEND(cppcoreguidelines-macro-usage)
