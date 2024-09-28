#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <fmt/ranges.h>

#include <string>
#include <toml++/toml.h>
#include <spdlog/spdlog.h>
#include <argparse/argparse.hpp>

#include "const.h"
#include "config.h"

namespace mtrx {

bool Config::load(SView path) {

	toml::table tbl;

	if (!std::filesystem::is_regular_file(path))
		std::cout << "* config not find, using default" << std::endl;
	else
		try {
			tbl = toml::parse_file(path);
		} catch (const toml::parse_error & err) {
			std::cout << err << std::endl;
			return false;
		}

	name    = tbl["name"].value_or(PRJ_NAME);
	version = tbl["version"].value_or(PRJ_VERSION);

	path_load = path;

	// set with hard code default value(from the struct definition)
	log.dir        = tbl["log"]["dir"].value_or(log.dir);
	log.pattern    = tbl["log"]["pattern"].value_or(log.pattern);
	log.level_term = tbl["log"]["level_term"].value_or(log.level_term);
	log.level_file = tbl["log"]["level_file"].value_or(log.level_file);

	log.rotate_size    = tbl["log"]["rotate_size"].value_or(10 * 1024 * 1024);
	log.rotate_count   = tbl["log"]["rotate_count"].value_or(1);
	log.flush_interval = tbl["log"]["flush_interval"].value_or(1);
	log.sync           = tbl["log"]["sync"].value_or(true);

	return true;
}

bool Config::dump(SView path) {
	auto tbl_log = toml::table{
		{"dir",            log.dir           },
		{"level_term",     log.level_term    },
		{"level_file",     log.level_file    },
		{"rotate_size",    log.rotate_size   },
		{"rotate_count",   log.rotate_count  },
		{"flush_interval", log.flush_interval},
		{"sync",           log.sync          },
		{"pattern",        log.pattern       },
	};

	auto tbl = toml::table{
		{"name",    name   },
		{"version", version},
		{"log",     tbl_log},
	};

	const auto path_out = (path == "" ? path_dump : path);

	auto logger = spdlog::default_logger()->clone("config");
	logger->info("dump: {}", path_out);

	if (path_out == "-") {
		std::cout << "--------\n"
				  << "# config: " << path_load << "\n"
				  << tbl << "\n--------" << std::endl;
	} else {
		std::fstream s(path_out.data(), s.binary | s.trunc | s.out);
		if (!s.is_open()) {
			std::cerr << "* failed to open: " << path_out << '\n';
		} else {
			s << tbl;
			s.close();
		}
	}

	return true;
}

bool Config::verify() {
	auto logger = spdlog::default_logger()->clone("config");

	// TODO: config version verification

	return true;
}

Config Config::fromArgs(int argc, char ** argv) {
	argparse::ArgumentParser parser(PRJ_NAME.data(), PRJ_VERSION.data());

	auto verify_choice = [](SView value, const auto & choices, SView msg) {
		if (std::find(choices.begin(), choices.end(), value) != choices.end())
			return std::string(value);
		throw std::runtime_error(fmt::format("* {}: \"{}\" not in {}", msg, value, choices));
	};

	parser.add_argument("-c", "--config")
		.default_value("data/config.toml")
		.nargs(1)
		.metavar("CONFIGFILE")
		.help("load config file");

	parser.add_argument("--log-dir")
		.default_value("./logs")
		.nargs(1)
		.metavar("DIR")
		.help("directory to hold all logs");

	parser.add_argument("--log-level")
		.default_value("info")
		.nargs(1)
		.metavar("LEVEL")
		.help(fmt::format("log level for stdout: {}", CHOICE_LOG_LEVELS))
		.action([verify_choice](SView value) {
			return verify_choice(value, CHOICE_LOG_LEVELS, "--log-level");
		});

	parser.add_argument("-o", "--output")
		.default_value("-")
		.nargs(1)
		.metavar("OUTPUT")
		.help("dump config to the output, write stdout if '-'");

	parser.add_argument("op")
		.help(fmt::format("operations: {}", CHOICE_OPERATIONS))
		.action([verify_choice](SView value) {
			return verify_choice(value, CHOICE_OPERATIONS, "op");
		});

	try {
		parser.parse_args(argc, argv);
	} catch (const std::runtime_error & err) {
		std::cerr << err.what() << std::endl;
		std::cerr << "\n" << parser;
		std::exit(1);
	}

	Config config;

	// op is verified on arg parse
	config.op = std::unordered_map<SView, Operation>{
		{"dump", OP_DUMP},
		{"run",  OP_RUN },
		{"test", OP_TEST},
	}[parser.get<std::string>("op")];

	// load config
	config.load(parser.get<std::string>("-c"));
	config.path_dump = parser.get<std::string>("-o");

	// override config with args
	if (parser.is_used("--log-dir"))
		config.log.dir = parser.get<std::string>("--log-dir");
	if (parser.is_used("--log-level"))
		config.log.level_term = parser.get<std::string>("--log-level");

	return config;
}
} // namespace mtrx
