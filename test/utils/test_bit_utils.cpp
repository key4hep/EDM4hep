#include "edm4hep/utils/bit_utils.h"

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <tuple>

// The integet types that we us as type fields in EDM4hep
using BitFieldTypes = std::tuple<int32_t, uint32_t, int64_t, int16_t, uint64_t>;

TEMPLATE_LIST_TEST_CASE("Bitfield utils set and get", "[bit_utils]", BitFieldTypes) {
  using namespace edm4hep;
  auto bitField = TestType{};

  for (auto i = 0u; i < sizeof(TestType) * 8; ++i) {
    REQUIRE_FALSE(utils::checkBit(bitField, i));
  }

  bitField = utils::setBit(bitField, 3, true);
  REQUIRE(utils::checkBit(bitField, 3));

  bitField = utils::setBit(bitField, 4, true);
  REQUIRE(utils::checkBit(bitField, 3));
  REQUIRE(utils::checkBit(bitField, 4));

  bitField = utils::setBit(bitField, 3, false);
  REQUIRE_FALSE(utils::checkBit(bitField, 3));
  REQUIRE(utils::checkBit(bitField, 4));

  if constexpr (sizeof(TestType) >= 8) {
    bitField = utils::setBit(bitField, 35, true);
    REQUIRE(utils::checkBit(bitField, 35));
    REQUIRE(bitField == TestType(1ULL << 4 | 1ULL << 35));
    bitField = utils::setBit(bitField, 35, false);
    REQUIRE_FALSE(utils::checkBit(bitField, 35));
    REQUIRE(bitField == TestType(1ULL << 4));
  }
}

TEMPLATE_LIST_TEST_CASE("Bitfield utils set multiple", "[bit_utils]", BitFieldTypes) {
  using namespace edm4hep;
  auto bitField = TestType{};
  bitField = utils::setBits(bitField, true, 3u, 4u, 7u);

  REQUIRE(utils::checkBit(bitField, 3));
  REQUIRE(utils::checkBit(bitField, 4));
  REQUIRE(utils::checkBit(bitField, 7));
  REQUIRE_FALSE(utils::checkBit(bitField, 1));
  REQUIRE_FALSE(utils::checkBit(bitField, 2));
  REQUIRE_FALSE(utils::checkBit(bitField, 5));
  REQUIRE_FALSE(utils::checkBit(bitField, 6));
  REQUIRE_FALSE(utils::checkBit(bitField, 8));
}

TEMPLATE_LIST_TEST_CASE("Bitfield utils check all ", "[bit_utils]", BitFieldTypes) {
  using namespace edm4hep;
  auto bitField = TestType{};
  bitField = utils::setBits(bitField, true, 3u, 4u, 7u);

  REQUIRE(utils::checkAllBits(bitField, 7u, 3u, 4u));
  REQUIRE(utils::checkAllBits(bitField, 3u, 4u));
  REQUIRE_FALSE(utils::checkAllBits(bitField, 2u, 3u, 4u, 7u));
  REQUIRE_FALSE(utils::checkAllBits(bitField, 2u, 3u, 4u));
}

TEMPLATE_LIST_TEST_CASE("Bitfield utils check any ", "[bit_utils]", BitFieldTypes) {
  using namespace edm4hep;
  auto bitField = TestType{};
  bitField = utils::setBits(bitField, true, 3u, 4u, 7u);

  REQUIRE(utils::checkAnyBits(bitField, 3u, 4u));
  REQUIRE(utils::checkAnyBits(bitField, 3u));
  REQUIRE(utils::checkAnyBits(bitField, 1u, 2u, 3u));
  REQUIRE_FALSE(utils::checkAnyBits(bitField, 1u, 2u, 6u, 8u));
}
