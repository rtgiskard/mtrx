#include <spdlog/spdlog.h>
#include <string>

#include "manager.h"

namespace mtrx {
namespace udp2p {

auto logger = spdlog::default_logger()->clone("udp2p");

bool AuthManager::updateUser(const std::string & id, const std::string & hash) {
	if (userdb_.contains(id))
		logger->info("update user: {}", id);
	else
		logger->info("add user: {}", id);

	userdb_[id] = hash;
	return true;
}

bool AuthManager::rmUser(const std::string & id) {
	auto it = userdb_.find(id);
	if (it == userdb_.end()) {
		logger->error("no such user: {}", id);
		return false;
	}

	userdb_.erase(it);
	return true;
}

bool AuthManager::authUser(const std::string & id, const std::string & hash) {
	auto it = userdb_.find(id);
	if (it == userdb_.end()) {
		logger->error("no such user: {}", id);
		return false;
	}
	return it->second == hash;
}

bool EndpointManager::updateEndpoint(const std::string & id, const EndPoint & endpoint) {
	if (onlineMap_.contains(id))
		logger->info("update endpoint: {}", id);
	else
		logger->info("add endpoint: {}", id);

	onlineMap_[id] = endpoint;
	return true;
}

bool EndpointManager::rmEndpoint(const std::string & id) {
	auto it = onlineMap_.find(id);
	if (it == onlineMap_.end()) {
		logger->error("no such endpoint: {}", id);
		return false;
	}

	onlineMap_.erase(it);
	return true;
};

const EndPoint & EndpointManager::getEndpointByID(const std::string & id) {

	auto it = onlineMap_.find(id);
	if (it == onlineMap_.end()) {
		logger->warn("no such endpoint: {}", id);
		static const EndPoint endpoint_zero{};
		return endpoint_zero;
	}

	return onlineMap_[id];
}

} // namespace udp2p
} // namespace mtrx
