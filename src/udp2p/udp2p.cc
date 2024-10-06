#include <cstring>
#include <memory>
#include <thread>

#include <asio/signal_set.hpp>
#include <asio/detail/chrono.hpp>
#include <asio/steady_timer.hpp>

#include "udp2p.h"

namespace mtrx {
namespace udp2p {

void Udp2p::run() {
	logger_->info("run mode: {}", static_cast<int>(config_.mode));

	switch (config_.mode) {
	case Udp2p_Mode::UDP2P_MODE_PEER:
		peer_ptr_ = std::make_unique<Peer>(ctx_);
		break;
	case Udp2p_Mode::UDP2P_MODE_SERVER:
		server_ptr_ = std::make_unique<Server>(ctx_);
		break;
	case Udp2p_Mode::UDP2P_MODE_HYBRID:
		peer_ptr_   = std::make_unique<Peer>(ctx_);
		server_ptr_ = std::make_unique<Server>(ctx_);
		break;
	}

	// hold sub threads to join at the end
	std::vector<std::thread> threads;
	if (peer_ptr_)
		threads.push_back(std::thread([this]() { peer_ptr_->run(); }));
	if (server_ptr_)
		threads.push_back(std::thread(
			[this]() { server_ptr_->serv(config_.server_addr, config_.server_port); }));

	// handle signals
	asio::signal_set signals(ctx_, SIGINT, SIGTERM);
	signals.async_wait([&](const asio::error_code & ec, int sig_num) {
		if (!ec) {
			logger_->warn("get signal: {}({})", sig_num, strsignal(sig_num));
			stop();
		} else
			logger_->warn("signal ec: {}, {}", ec.value(), ec.message());
	});

	// run the event loop
	ctx_.run();

	// wait sub threads
	for (auto & t : threads)
		t.join();
}

void Udp2p::stop() {
	logger_->info("stop ..");
	// notify peer/server via the stop
	ctx_.stop();
}

}; // namespace udp2p
}; // namespace mtrx
