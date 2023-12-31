#pragma once

#include "core.h"

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

namespace kEn {

	class log
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& core_logger() { return core_logger_; }
		inline static std::shared_ptr<spdlog::logger>& client_logger() { return client_logger_; }

	private:
		static std::shared_ptr<spdlog::logger> core_logger_;
		static std::shared_ptr<spdlog::logger> client_logger_;
	};
}

// Core macros
#define KEN_CORE_TRACE(...)    ::kEn::log::core_logger()->trace(__VA_ARGS__)
#define KEN_CORE_INFO(...)     ::kEn::log::core_logger()->info(__VA_ARGS__)
#define KEN_CORE_WARN(...)     ::kEn::log::core_logger()->warn(__VA_ARGS__)
#define KEN_CORE_ERROR(...)    ::kEn::log::core_logger()->error(__VA_ARGS__)
#define KEN_CORE_CRITICAL(...) ::kEn::log::core_logger()->critical(__VA_ARGS__)

// Client macros
#define KEN_TRACE(...)    ::kEn::log::client_logger()->trace(__VA_ARGS__)
#define KEN_INFO(...)     ::kEn::log::client_logger()->info(__VA_ARGS__)
#define KEN_WARN(...)     ::kEn::log::client_logger()->warn(__VA_ARGS__)
#define KEN_ERROR(...)    ::kEn::log::client_logger()->error(__VA_ARGS__)
#define KEN_CRITICAL(...) ::kEn::log::client_logger()->critical(__VA_ARGS__)

// Debug
#ifdef _KEN_DEBUG
#	define KEN_CORE_DEBUG(...) ::kEn::log::core_logger()->debug(__VA_ARGS__)
#	define KEN_DEBUG(...)      ::kEn::log::client_logger()->debug(__VA_ARGS__)
#else
#	define KEN_CORE_DEBUG(...)
#	define KEN_DEBUG(...)
#endif
