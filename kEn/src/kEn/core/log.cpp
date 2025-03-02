#include <kenpch.hpp>
#include <kEn/core/log.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace kEn
{
	std::shared_ptr<spdlog::logger> log::core_logger_;
	std::shared_ptr<spdlog::logger> log::client_logger_;

	void log::init()
	{
		spdlog::set_pattern("%^[%T] [%n/%l]: %v%$");
		core_logger_ = spdlog::stdout_color_mt("kEn");
		core_logger_->set_level(spdlog::level::trace);

		client_logger_ = spdlog::stdout_color_mt("Game");
		client_logger_->set_level(spdlog::level::trace);
	}
}
