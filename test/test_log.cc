#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include <doctest.h>

#include "../src/log.h"

#include <chrono>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>

void bench_logger(int howmany, std::shared_ptr<spdlog::logger> log) {
	using std::chrono::duration;
	using std::chrono::duration_cast;

	spdlog::stopwatch sw;

	for (auto i = 0; i < howmany; ++i)
		log->info("Hello logger: msg number {}", i);

	auto delta = duration_cast<duration<double>>(sw.elapsed()).count();

	auto logger_summary = spdlog::get("summary");
	logger_summary->info(spdlog::fmt_lib::format("{:<30} Elapsed: {:0.2f} secs {:>16L}/sec",
	                                             log->name(), delta, int(howmany / delta)));

	spdlog::drop(log->name());
}

TEST_CASE("spdlog_test") {
	mtrx::init_log("log_test", mtrx::LogSettings{});

	auto logger = spdlog::default_logger();

	logger->info("Welcome to spdlog version {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR,
	             SPDLOG_VER_PATCH);

	logger->info("log level: {}", spdlog::level::to_string_view(logger->level()));

	logger->warn("Easy padding in numbers like {:08d}", 12);
	logger->critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
	logger->info("Support for floats {:03.2f}", 1.23456);
	logger->info("Positional args are {1} {0}..", "too", "supported");
	logger->info("{:>8} aligned, {:<8} aligned", "right", "left");

	logger->debug("debug log");
	logger->trace("trace log");

	SPDLOG_TRACE("Some trace message.. {} ,{}", 1, 3.23);
	SPDLOG_DEBUG("Some debug message.. {} ,{}", 1, 3.23);
	SPDLOG_INFO("Some info message.. {} ,{}", 1, 3.23);
	SPDLOG_WARN("Some warn message.. {} ,{}", 1, 3.23);

	// Compiled time log level
	SPDLOG_LOGGER_TRACE(logger, "another trace message");
	SPDLOG_LOGGER_DEBUG(logger, "another debug message");
	SPDLOG_LOGGER_WARN(logger, "another warn message");
}

TEST_CASE("spdlog_bench" * doctest::skip(true)) {
	mtrx::init_log("log_bench", mtrx::LogSettings{});
	auto howmany = 200000;

	// store summary to file
	auto stdout_sink   = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
		"logs/bench_summary.log", 1024 * 1024 * 1, 2);
	spdlog::sinks_init_list sink_list = {stdout_sink, rotating_sink};
	auto logger_summary = std::make_shared<spdlog::logger>("summary", sink_list);

	stdout_sink->set_level(spdlog::level::debug);
	rotating_sink->set_level(spdlog::level::debug);
	logger_summary->set_level(spdlog::level::debug);

	// logger not created by the spdlog.h factory functions need to be registered
	spdlog::register_logger(logger_summary);
	logger_summary->info("");

	auto logger0 = spdlog::rotating_logger_mt(
		"rotating-off", "logs/bench_sync_rotating_off.log", 1024 * 1024 * 10, 5);
	logger0->set_level(spdlog::level::off);
	bench_logger(howmany * 20, logger0);

	auto logger1 = spdlog::rotating_logger_mt("rotating_mt", "logs/bench_sync_rotating.log",
	                                          1024 * 1024 * 10, 5);
	bench_logger(howmany, logger1);

	auto logger2 = spdlog::create_async<spdlog::sinks::rotating_file_sink_mt>(
		"async_rotating_mt", "logs/bench_async_rotating.log", 1024 * 1024 * 10, 5, false);
	bench_logger(howmany, logger2);

	auto logger3 = spdlog::create_async_nb<spdlog::sinks::rotating_file_sink_mt>(
		"asyncnb_rotating_mt", "logs/bench_asyncnb_rotating.log", 1024 * 1024 * 10, 5, false);
	bench_logger(howmany, logger3);

	auto logger4 = spdlog::stdout_color_mt("stdout");
	bench_logger(10000, logger4);

	auto logger5 = spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>("async_stdout");
	bench_logger(10000, logger5);

	auto logger6 =
		spdlog::create_async_nb<spdlog::sinks::stdout_color_sink_mt>("asyncnb_stdout");
	bench_logger(10000, logger6);
}
