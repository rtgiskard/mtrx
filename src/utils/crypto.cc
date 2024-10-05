#include <cstdint>
#include <string>
#include <memory>
#include <argon2.h>

#include "crypto.h"

namespace mtrx {
namespace utils {

argon2_context KeyUtils::genHashContext(const uint8_t * hash, const uint32_t hash_size,
                                        const std::string & key) {
	return argon2_context{
		const_cast<uint8_t *>(hash), hash_size,
		const_cast<uint8_t *>(reinterpret_cast<const uint8_t *>(key.c_str())),
		static_cast<uint32_t>(key.size()),
		const_cast<uint8_t *>(reinterpret_cast<const uint8_t *>(hash_params.salt.c_str())),
		static_cast<uint32_t>(hash_params.salt.size()), // salt
		nullptr, 0, nullptr, 0,                         // optional secret and associated data
		hash_params.t_cost, 1u << hash_params.m_cost, hash_params.paralle,
		hash_params.paralle, hash_params.hash_ver, // algorithm version
		nullptr, nullptr, // custom memory allocation / deallocation functions
		// by default only internal memory is cleared (pwd is not wiped)
		ARGON2_DEFAULT_FLAGS};
}

bool KeyUtils::hashFromKey(uint8_t * hash, const uint32_t hash_size,
                           const std::string & key) {
	auto ctx = genHashContext(hash, hash_size, key);
	auto rc  = argon2_ctx(&ctx, hash_params.hash_type);
	return (rc == ARGON2_OK);
}

bool KeyUtils::verifyHash(const uint8_t * hash, const uint32_t hash_size,
                          const std::string & key) {
	// create a tmp hash buffer
	std::unique_ptr<uint8_t[]> tmp_hash(new uint8_t[hash_size]);

	// create context with the tmp_hash
	auto ctx = genHashContext(tmp_hash.get(), hash_size, key);

	// verify: calc hash for key, write to tmp_hash, compare(tmp_hash, hash)
	auto rc =
		argon2_verify_ctx(&ctx, reinterpret_cast<const char *>(hash), hash_params.hash_type);
	return (rc == ARGON2_OK);
}

RandomBytesEngine KeyUtils::rng;

}; // namespace utils
}; // namespace mtrx
