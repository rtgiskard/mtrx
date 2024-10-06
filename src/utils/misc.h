#ifndef INCLUDE_UTILS_MISC_H
#define INCLUDE_UTILS_MISC_H

#include <cstddef>
#include <cstdint>
#include <string>

namespace mtrx {
namespace utils {

template <typename T>
concept Byte = sizeof(T) == 1;

template <Byte T>
std::string hexFromBytes(const T * bytes, const uint32_t size);

template <Byte T>
bool hexToBytes(const std::string & hex, T * bytes, const uint32_t size);

template <typename... Ts>
std::array<std::byte, sizeof...(Ts)> make_bytes(Ts &&... args) noexcept {
	return {std::byte(std::forward<Ts>(args))...};
}

}; // namespace utils
}; // namespace mtrx

#endif
