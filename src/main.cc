#include <spdlog/spdlog.h>

#include "const.h"
#include "config.h"

#include "udp2p/udp2p.h"

int main(int argc, char ** argv) {

	auto config = mtrx::Config::fromArgs(argc, argv);
	if (!mtrx::utils::init_log(mtrx::PRJ_NAME, config.log))
		return 1;

	spdlog::info("== ver: {}, pid: {}", mtrx::PRJ_VERSION, getpid());

	if (!config.verify())
		return 2;

	switch (config.op) {
	case mtrx::OP_DUMP:
		config.dump();
		break;
	case mtrx::OP_UDP2P: {
		auto udp2p = mtrx::udp2p::Udp2p();
		udp2p.run();
		break;
	}
	default:
		break;
	}

	return 0;
}
