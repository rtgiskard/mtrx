#ifndef INCLUDE_UDP2P_MANAGER_H
#define INCLUDE_UDP2P_MANAGER_H

#include <map>
#include <string>

#include <asio.hpp>

namespace mtrx {
namespace udp2p {

struct EndPoint {
	asio::ip::address_v4 ip4;
	int                  port;
};

class AuthManager {
  public:
	AuthManager(){};
	~AuthManager(){};

	bool authUser(const std::string & id, const std::string & hash);
	bool updateUser(const std::string & id, const std::string & hash);
	bool rmUser(const std::string & id);

  private:
	std::map<std::string, std::string> userdb_;
};

class EndpointManager {
  public:
	EndpointManager(){};
	~EndpointManager(){};

	bool             updateEndpoint(const std::string & id, const EndPoint & endpoint);
	bool             rmEndpoint(const std::string & id);
	const EndPoint & getEndpointByID(const std::string & id);

  private:
	std::map<std::string, EndPoint> onlineMap_;
};

}; // namespace udp2p
}; // namespace mtrx

#endif
