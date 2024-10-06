#ifndef INCLUDE_UDP2P_SERVER_H
#define INCLUDE_UDP2P_SERVER_H

#include <asio/io_context.hpp>
#include <spdlog/spdlog.h>

#include "udp2p/manager.h"

namespace mtrx {
namespace udp2p {

class Server {
  public:
	Server(asio::io_context & ctx) : ctx_(ctx) {
		logger_ = spdlog::default_logger()->clone("server");
	};

	void serv(const std::string & addr, uint port);

  private:
	asio::io_context &              ctx_;
	AuthManager                     authKeeper_;
	std::shared_ptr<spdlog::logger> logger_;
};

class Relay {
  public:
	void serv();
};

}; // namespace udp2p
}; // namespace mtrx
#endif
