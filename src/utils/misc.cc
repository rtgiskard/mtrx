#include <format>
#include <sstream>
#include <string>

#include "misc.h"

namespace mtrx {
namespace utils {

std::string hexFromBytes(const uint8_t * bytes, const uint32_t size) {
	std::ostringstream oss;
	for (uint32_t i = 0; i < size; i++)
		oss << std::format("{:02x}", bytes[i]);
	return oss.str();
}

bool hexToBytes(const std::string & hex, uint8_t * bytes, const uint32_t size) {
	if (hex.size() != size * 2)
		return false;

	for (auto c : hex)
		if (!std::isxdigit(c))
			return false;

	for (uint32_t i = 0; i < size; i++)
		bytes[i] = std::stoul(hex.substr(i * 2, 2), nullptr, 16);

	return true;
}

}; // namespace utils
}; // namespace mtrx
