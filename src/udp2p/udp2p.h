#ifndef INCLUDE_UDP2P_UDP2P_H
#define INCLUDE_UDP2P_UDP2P_H

#include <cstdint>
#include <memory>
#include <string>

#include <asio/io_context.hpp>
#include <spdlog/spdlog.h>

#include "udp2p/peer.h"
#include "udp2p/server.h"

namespace mtrx {
namespace udp2p {

enum class Udp2p_Mode {
	UDP2P_MODE_PEER,   // peer only
	UDP2P_MODE_SERVER, // server only
	UDP2P_MODE_HYBRID  // peer + server
};

class Config {
  public:
	Udp2p_Mode  mode;
	std::string server_addr;
	int         server_port;
	uint64_t    peer_id; // peer to connect
};

class Udp2p {
  public:
	Udp2p(Config & config) : config_(config) {
		logger_ = spdlog::default_logger()->clone("udp2p");
	}

	void run();
	void stop();

  private:
	Config &         config_;
	asio::io_context ctx_;

	std::unique_ptr<Server> server_ptr_ = nullptr;
	std::unique_ptr<Peer>   peer_ptr_   = nullptr;

	std::shared_ptr<spdlog::logger> logger_;
};

}; // namespace udp2p
}; // namespace mtrx

#endif
