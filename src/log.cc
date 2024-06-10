#include <iostream>

// spdlog switch might be defined there
#include "log.h"

// spdlog
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

// init_spdlog set default to async logger with multi sink (stdout, rotating_file)
bool init_log(SView name, const LogSettings & setting) {
	try {
		auto log_dir = setting.dir.size() > 0 ? setting.dir : "./logs";

		// use multi sink: stdout, rotating_file
		auto stdout_sink   = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
			std::format("{}/{}.log", log_dir, name), setting.rotate_size,
			setting.rotate_count, false);

		spdlog::sinks_init_list sink_list = {stdout_sink, rotating_sink};

		std::shared_ptr<spdlog::logger> logger;

		// speed test:
		// 	async_nb stdout > sync file > async_nb file > async file >
		// 	sync stdout > async_stdout
		if (setting.sync)
			logger = std::make_shared<spdlog::logger>("main", sink_list);

		else {
			// queue with max 4k items 1 backing thread
			spdlog::init_thread_pool(4096, 1);
			logger = std::make_shared<spdlog::async_logger>(
				"main", sink_list, spdlog::thread_pool(),
				spdlog::async_overflow_policy::block);
		}

		// set log level for every sink
		stdout_sink->set_level(spdlog::level::from_str(setting.level_term));
		rotating_sink->set_level(spdlog::level::from_str(setting.level_file));

		// set global log level and pattern
		logger->set_level(spdlog::level::trace);
		logger->set_pattern(setting.pattern);

		// Flush all *registered* loggers using a worker thread at interval
		spdlog::flush_every(std::chrono::seconds(setting.flush_interval));

		spdlog::set_default_logger(logger);

	} catch (const spdlog::spdlog_ex & ex) {
		// Exceptions will only be thrown upon failed logger or sink construction
		std::cout << "Log initialization failed: " << ex.what() << std::endl;
		return false;
	}

	// SPDLOG_LEVEL=info,mylogger=trace ./example
	// spdlog::cfg::load_env_levels();
	return true;
}
