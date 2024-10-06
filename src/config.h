#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H

#include <string>
#include <string_view>

#include "udp2p/udp2p.h"
#include "utils/log.h"

namespace mtrx {

using SView = std::string_view;

enum class Operation {
	RUN,
	TEST,
	DUMP,
	UDP2P,
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
	udp2p::Config      udp2p;

  public:
	bool load(SView path);
	bool dump(SView path = "");
	bool verify();

	static Config fromArgs(int argc, char ** argv);
};
} // namespace mtrx

#endif
