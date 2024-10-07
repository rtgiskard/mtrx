#include <botan/pwdhash.h>

#include "crypto.h"

namespace mtrx {
namespace utils {

void KeyUtils::hashKey(std::span<uint8_t> hash, const std::string & key) {
	auto hashFM = Botan::PasswordHashFamily::create(hash_params_.family);
	auto argon2 = hashFM->from_params(1 << hash_params_.m, hash_params_.t, hash_params_.p);
	argon2->hash(hash, key, hash_params_.salt);
}

bool KeyUtils::verifyHash(const std::span<uint8_t> hash, const std::string & key) {
	auto hash_tmp = std::vector<uint8_t>(hash.size());
	hashKey(hash_tmp, key);
	return std::equal(hash.cbegin(), hash.cend(), hash_tmp.cbegin());
}

RandomBytesEngine KeyUtils::rng_;

}; // namespace utils
}; // namespace mtrx
