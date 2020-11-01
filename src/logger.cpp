#include "logger.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/ostream_sink.h"


namespace {

	const std::string STANDARD_LOGGER_NAME{ "otplog" };

}

spdlog::logger& standard_logger(){
	auto raw_ptr = spdlog::get(STANDARD_LOGGER_NAME).get();
	internal_error::assert_true(raw_ptr != nullptr, "The standard logger has not been registered.");
	return *raw_ptr;
}

void init_logger(){
	auto sink_std_cout = std::make_shared<spdlog::sinks::ostream_sink_mt>(std::cout);
	auto standard_logger = std::make_shared<spdlog::logger>(STANDARD_LOGGER_NAME, sink_std_cout);
	standard_logger->set_level(spdlog::level::debug);
	spdlog::register_logger(standard_logger);
}


