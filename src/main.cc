#include "const.h"
#include "config.h"
#include "log.h"

int main(int argc, char ** argv) {

	auto config = mtrx::Config::fromArgs(argc, argv);
	if (!mtrx::init_log(mtrx::PRJ_NAME, config.log))
		return 1;

	spdlog::info("== ver: {}, pid: {}", mtrx::PRJ_VERSION, getpid());

	if (!config.verify())
		return 2;

	switch (config.op) {
	case mtrx::OP_DUMP:
		config.dump();
		break;
	default:
		break;
	}

	return 0;
}
