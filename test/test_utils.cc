#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include "doctest.h"

#include <cstddef>
#include <cstdint>
#include <array>
#include <string>

#include "../src/utils/crypto.h"
#include "../src/utils/misc.h"

auto utils = mtrx::utils::KeyUtils(mtrx::utils::HashParams());

TEST_CASE("genRandBytes") {
	std::array<uint8_t, 28> hash;

	utils.seed();
	utils.genRandBytes(hash.data(), hash.size());
}

TEST_CASE("KeyUtils: hash and verify") {
	// provide hash buffer and a sample key
	std::array<uint8_t, 32> hash;
	std::string             key = "key to hash and verify";

	// initial hash should not match the key
	CHECK(!utils.verifyHash(hash.data(), hash.size(), key));

	// update hash with respect to key
	utils.hashFromKey(hash.data(), hash.size(), key);

	// verify (hash, key), should match now
	CHECK(utils.verifyHash(hash.data(), hash.size(), key));

	// compare with external reference (update this if the default params changed):
	// echo -n "key to hash and verify" | argon2 "0xSalty.~#@&" -id -t 4 -m 10 -p 2 -l 32
	auto ref_hex = "0c0bfec1c6faad5a6f3527d8dd46d1786164fbbf9a442caf05884ac0d6b0c737";
	CHECK(ref_hex == mtrx::utils::hexFromBytes(hash.data(), hash.size()));
}

TEST_CASE("hexFromBytes") {
	uint8_t buf[] = {0x00, 0x02, 0xc2, 0xef};
	CHECK(mtrx::utils::hexFromBytes(buf, 0) == "");
	CHECK(mtrx::utils::hexFromBytes(buf, 1) == "00");
	CHECK(mtrx::utils::hexFromBytes(buf, 2) == "0002");
	CHECK(mtrx::utils::hexFromBytes(buf, 4) == "0002c2ef");

	auto buf2 = mtrx::utils::make_bytes(0x00, 0x02, 0xc2, 0xef);
	CHECK(mtrx::utils::hexFromBytes(buf2.data(), buf2.size()) == "0002c2ef");
}

TEST_CASE("hexToBytes") {
	uint8_t     buf[20];
	std::string str;

	CHECK(mtrx::utils::hexToBytes("", buf, 0));
	CHECK(!mtrx::utils::hexToBytes("", buf, 1));

	str = "0123456789abcdef";
	CHECK(mtrx::utils::hexToBytes(str, buf, str.size() / 2));
	CHECK(buf[0] == 0x01);
	CHECK(buf[1] == 0x23);
	CHECK(buf[2] == 0x45);

	str = "0123456789abcdefgh";
	CHECK(!mtrx::utils::hexToBytes(str, buf, str.size() / 2));

	str = "010";
	CHECK(!mtrx::utils::hexToBytes(str, buf, str.size() / 2));

	str = "az";
	CHECK(!mtrx::utils::hexToBytes(str, buf, str.size() / 2));

	str = "0 24";
	CHECK(!mtrx::utils::hexToBytes(str, buf, str.size() / 2));

	str = "0eFf";
	CHECK(mtrx::utils::hexToBytes(str, buf, str.size() / 2));
	CHECK(buf[0] == 0x0e);
	CHECK(buf[1] == 0xff);
}

TEST_CASE("rand hex convert") {
	uint8_t buf[40];

	mtrx::utils::KeyUtils::genRandBytes(buf, sizeof(buf));
	auto str = mtrx::utils::hexFromBytes(buf, sizeof(buf));

	CHECK(mtrx::utils::hexToBytes(str, buf, sizeof(buf)));
	CHECK(mtrx::utils::hexFromBytes(buf, sizeof(buf)) == str);

	CHECK(mtrx::utils::hexToBytes(str, (char *)buf, sizeof(buf)));
	CHECK(mtrx::utils::hexFromBytes((char *)buf, sizeof(buf)) == str);

	CHECK(mtrx::utils::hexToBytes(str, (std::byte *)buf, sizeof(buf)));
	CHECK(mtrx::utils::hexFromBytes((std::byte *)buf, sizeof(buf)) == str);
}
