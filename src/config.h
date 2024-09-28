#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H

#include <string>
#include <string_view>

namespace mtrx {

using SView = std::string_view;

enum Operation {
	OP_RUN,
	OP_TEST,
	OP_DUMP,
};

struct LogSettings {
	std::string dir        = "logs";
	std::string level_term = "info";
	std::string level_file = "trace";
	std::string pattern    = "%Y%m%d_%H%M%S.%e %^%L%$ %n: %v";

	uint rotate_size    = 4 * 1024 * 1024;
	uint rotate_count   = 4;
	uint flush_interval = 1;
	bool sync           = true;
};

class Config {
  public:
	// from args
	std::string path_load;
	std::string path_dump;
	Operation   op;

	// from config
	std::string name;
	std::string version;

	LogSettings log;

  public:
	bool load(SView path);
	bool dump(SView path = "");
	bool verify();

	static Config fromArgs(int argc, char ** argv);
};
} // namespace mtrx

#endif
