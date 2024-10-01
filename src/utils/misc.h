#ifndef INCLUDE_UTILS_MISC_H
#define INCLUDE_UTILS_MISC_H

#include <cstdint>
#include <string>

namespace mtrx {
namespace utils {

std::string hexFromBytes(const uint8_t * bytes, const uint32_t size);
bool        hexToBytes(const std::string & hex, uint8_t * bytes, const uint32_t size);

}; // namespace utils
}; // namespace mtrx

#endif
