#ifndef INCLUDE_UTILS_CRYPTO_H
#define INCLUDE_UTILS_CRYPTO_H

#include <climits>
#include <cstdint>
#include <random>
#include <span>
#include <string>
#include <vector>
#include <algorithm>

namespace mtrx {
namespace utils {

using RandomBytesEngine =
	std::independent_bits_engine<std::default_random_engine, CHAR_BIT, uint8_t>;

struct HashParams {
	std::string          family = "Argon2id"; // use Argon2id
	std::vector<uint8_t> salt   =             // req: salt.size() >= 8
		{0x4c, 0x0e, 0x56, 0x20, 0xe2, 0x70, 0x94, 0x74, 0x4e, 0x67, 0xec, 0xae};

	uint8_t t = 4;  // n-pass computation
	uint8_t m = 10; // 2^N KiB memory usage
	uint8_t p = 2;  // number of threads and lanes
};

class KeyUtils {
  public:
	KeyUtils(const HashParams & hash_params) : hash_params_(hash_params) {}

	static inline void seed() { rng_.seed(std::random_device()()); }

	static inline void genRandBytes(uint8_t * buf, const uint32_t size = 1) {
		std::generate(buf, buf + size, std::ref(rng_));
	}

	void hashKey(std::span<uint8_t> hash, const std::string & key);
	bool verifyHash(const std::span<uint8_t> hash, const std::string & key);

  private:
	HashParams hash_params_;

	static RandomBytesEngine rng_;
};

}; // namespace utils
}; // namespace mtrx

#endif
