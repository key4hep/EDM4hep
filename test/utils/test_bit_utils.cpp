#include "edm4hep/utils/bit_utils.h"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <tuple>
#include <type_traits>

// The integer types that we use as type fields in EDM4hep
using BitFieldTypes = std::tuple<int32_t, uint32_t, int64_t, int16_t, uint64_t>;

namespace {
/// Helper to run a test body for every type in BitFieldTypes
template <typename F>
void forEachType(F&& f) {
  std::apply([&](auto... types) { (f(types), ...); }, BitFieldTypes{});
}
} // namespace

TEST_CASE("Bitfield utils", "[bit_utils]") {
  using namespace edm4hep;

  SECTION("set and get") {
    forEachType([](auto type) {
      using T = std::decay_t<decltype(type)>;
      auto bitField = T{};

      for (auto i = 0u; i < sizeof(T) * 8; ++i) {
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

      if constexpr (sizeof(T) >= 8) {
        bitField = utils::setBit(bitField, 35, true);
        REQUIRE(utils::checkBit(bitField, 35));
        REQUIRE(bitField == T(1ULL << 4 | 1ULL << 35));
        bitField = utils::setBit(bitField, 35, false);
        REQUIRE_FALSE(utils::checkBit(bitField, 35));
        REQUIRE(bitField == T(1ULL << 4));
      }
    });
  }

  SECTION("set multiple") {
    forEachType([](auto type) {
      using T = std::decay_t<decltype(type)>;
      auto bitField = T{};
      bitField = utils::setBits(bitField, true, 3u, 4u, 7u);

      REQUIRE(utils::checkBit(bitField, 3));
      REQUIRE(utils::checkBit(bitField, 4));
      REQUIRE(utils::checkBit(bitField, 7));
      REQUIRE_FALSE(utils::checkBit(bitField, 1));
      REQUIRE_FALSE(utils::checkBit(bitField, 2));
      REQUIRE_FALSE(utils::checkBit(bitField, 5));
      REQUIRE_FALSE(utils::checkBit(bitField, 6));
      REQUIRE_FALSE(utils::checkBit(bitField, 8));
    });
  }

  SECTION("check all") {
    forEachType([](auto type) {
      using T = std::decay_t<decltype(type)>;
      auto bitField = T{};
      bitField = utils::setBits(bitField, true, 3u, 4u, 7u);

      REQUIRE(utils::checkAllBits(bitField, 7u, 3u, 4u));
      REQUIRE(utils::checkAllBits(bitField, 3u, 4u));
      REQUIRE_FALSE(utils::checkAllBits(bitField, 2u, 3u, 4u, 7u));
      REQUIRE_FALSE(utils::checkAllBits(bitField, 2u, 3u, 4u));
    });
  }

  SECTION("check any") {
    forEachType([](auto type) {
      using T = std::decay_t<decltype(type)>;
      auto bitField = T{};
      bitField = utils::setBits(bitField, true, 3u, 4u, 7u);

      REQUIRE(utils::checkAnyBits(bitField, 3u, 4u));
      REQUIRE(utils::checkAnyBits(bitField, 3u));
      REQUIRE(utils::checkAnyBits(bitField, 1u, 2u, 3u));
      REQUIRE_FALSE(utils::checkAnyBits(bitField, 1u, 2u, 6u, 8u));
    });
  }
}
