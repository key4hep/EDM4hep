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

  bitField = utils::setBit(bitField, 3, false);
  REQUIRE_FALSE(utils::checkBit(bitField, 3));
}
