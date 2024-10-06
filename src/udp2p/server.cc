#include <string>

#include <asio/detached.hpp>
#include <asio/awaitable.hpp>
#include <asio/use_awaitable.hpp>
#include <asio/co_spawn.hpp>
#include <asio/buffer.hpp>
#include <asio/ip/address.hpp>
#include <asio/ip/udp.hpp>

#include "server.h"
#include "utils/misc.h"

namespace mtrx {
namespace udp2p {

void Server::serv(const std::string & addr, uint port) {
	logger_->info("serv ..");

	auto endpoint = asio::ip::udp::endpoint(asio::ip::make_address(addr), port);
	auto socket   = asio::ip::udp::socket(ctx_, endpoint);

	asio::co_spawn(
		ctx_,
		[&socket, this]() -> asio::awaitable<void> {
			char                    buffer[1024];
			asio::ip::udp::endpoint remote_endpoint;
			while (true) {
				size_t n = co_await socket.async_receive_from(
					asio::buffer(buffer), remote_endpoint, asio::use_awaitable);

				logger_->debug("get: {}", utils::hexFromBytes(buffer, n));

				co_await socket.async_send_to(asio::buffer(buffer, n), remote_endpoint,
			                                  asio::use_awaitable);
			}
		},
		asio::detached);

	ctx_.run();
}

}; // namespace udp2p
}; // namespace mtrx
