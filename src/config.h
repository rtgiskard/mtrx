#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H

#include <string>
#include <string_view>

#include "utils/log.h"

namespace mtrx {

using SView = std::string_view;

enum Operation {
	OP_RUN,
	OP_TEST,
	OP_DUMP,
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

	utils::LogSettings log;

  public:
	bool load(SView path);
	bool dump(SView path = "");
	bool verify();

	static Config fromArgs(int argc, char ** argv);
};
} // namespace mtrx

#endif
