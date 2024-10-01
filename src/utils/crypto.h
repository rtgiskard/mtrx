#ifndef INCLUDE_UTILS_CRYPTO_H
#define INCLUDE_UTILS_CRYPTO_H

#include <cstdint>
#include <random>
#include <string>
#include <algorithm>

#include <argon2.h>

namespace mtrx {
namespace utils {

using RandomBytesEngine =
	std::independent_bits_engine<std::default_random_engine, CHAR_BIT, uint8_t>;

struct HashParams {
	std::string salt = "0xSalty.~#@&"; //

	uint8_t t_cost  = 4;  // n-pass computation
	uint8_t m_cost  = 10; // 2^N KiB memory usage
	uint8_t paralle = 2;  // number of threads and lanes

	argon2_type    hash_type = Argon2_id;
	argon2_version hash_ver  = ARGON2_VERSION_13;
};

class KeyUtils {
  public:
	KeyUtils(const HashParams & hash_params) : hash_params(hash_params) {}

	static inline void seed() { rng.seed(std::random_device()()); }

	static inline void genRandBytes(uint8_t * buf, const uint32_t size = 1) {
		std::generate(buf, buf + size, std::ref(rng));
	}

	bool hashFromKey(uint8_t * hash, const uint32_t hash_size, const std::string & key);
	bool verifyHash(const uint8_t * hash, const uint32_t hash_size, const std::string & key);

  private:
	argon2_context genHashContext(const uint8_t * hash, const uint32_t hash_size,
	                              const std::string & key);

	HashParams hash_params;

	static RandomBytesEngine rng;
};

}; // namespace utils
}; // namespace mtrx

#endif
