#include <asio/steady_timer.hpp>

#include "peer.h"

namespace mtrx {
namespace udp2p {

void Peer::run() {
	logger_->info("run ..");

	// TODO: do_sth()
	asio::steady_timer t(ctx_, asio::chrono::seconds(2));
	t.async_wait([&](const std::error_code &) { logger_->info("timer done"); });

	ctx_.run();
};

uint64_t Peer::connectServer() {
	return 0;
};

void Peer::disconnectServer() {};

}; // namespace udp2p
}; // namespace mtrx
