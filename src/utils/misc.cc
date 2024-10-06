#include <cstdint>
#include <format>
#include <sstream>
#include <string>

#include "misc.h"

namespace mtrx {
namespace utils {

template <Byte T>
std::string hexFromBytes(const T * bytes, const uint32_t size) {
	std::ostringstream oss;
	for (uint32_t i = 0; i < size; i++)
		oss << std::format("{:02x}", static_cast<uint8_t>(bytes[i]));
	return oss.str();
}

template <Byte T>
bool hexToBytes(const std::string & hex, T * bytes, const uint32_t size) {
	if (hex.size() != size * 2)
		return false;

	for (auto c : hex)
		if (!std::isxdigit(c))
			return false;

	for (uint32_t i = 0; i < size; i++)
		bytes[i] = static_cast<T>(std::stoul(hex.substr(i * 2, 2), nullptr, 16));

	return true;
}

template std::string hexFromBytes(const char * bytes, const uint32_t size);
template std::string hexFromBytes(const uint8_t * bytes, const uint32_t size);
template std::string hexFromBytes(const std::byte * bytes, const uint32_t size);

template bool hexToBytes(const std::string & hex, char * bytes, const uint32_t size);
template bool hexToBytes(const std::string & hex, uint8_t * bytes, const uint32_t size);
template bool hexToBytes(const std::string & hex, std::byte * bytes, const uint32_t size);

}; // namespace utils
}; // namespace mtrx
