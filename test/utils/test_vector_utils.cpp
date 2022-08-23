#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "edm4hep/utils/vector_utils.h"

#include "edm4hep/Vector2f.h"
#include "edm4hep/Vector2i.h"
#include "edm4hep/Vector3d.h"
#include "edm4hep/Vector3f.h"

#include <tuple>
#include <type_traits>

using AllVectorTypes = std::tuple<edm4hep::Vector3f, edm4hep::Vector3d, edm4hep::Vector2i, edm4hep::Vector2f>;

template <typename V>
constexpr V create();

template <>
constexpr edm4hep::Vector3f create() {
  return edm4hep::Vector3f{1.0f, 2.0f, 3.0f};
}

template <>
constexpr edm4hep::Vector3d create() {
  return edm4hep::Vector3d{1.0, 2.0, 3.0};
}

template <>
constexpr edm4hep::Vector2f create() {
  return edm4hep::Vector2f{1.0f, 2.0f};
}

template <>
constexpr edm4hep::Vector2i create() {
  return edm4hep::Vector2i{1, 2};
}

TEMPLATE_LIST_TEST_CASE("Vector uniform getters", "[vector_utils]", AllVectorTypes) {
  using namespace edm4hep;

  constexpr auto vector = create<TestType>();

  STATIC_REQUIRE(utils::vector_x(vector) == utils::ValueType<TestType>(1.0));
  STATIC_REQUIRE(utils::vector_y(vector) == utils::ValueType<TestType>(2.0));
  // 2D vectors fill z component with 0
  if constexpr (std::is_same_v<TestType, edm4hep::Vector2i> || std::is_same_v<TestType, edm4hep::Vector2f>) {
    STATIC_REQUIRE(utils::vector_z(vector) == utils::ValueType<TestType>(0.0));
  } else {
    STATIC_REQUIRE(utils::vector_z(vector) == utils::ValueType<TestType>(3.0));
  }
}

TEST_CASE("Vector ValueType", "[vector_utils]") {
  using namespace edm4hep;
  STATIC_REQUIRE(std::is_same_v<int32_t, utils::ValueType<Vector2i>>);
  STATIC_REQUIRE(std::is_same_v<float, utils::ValueType<Vector2f>>);
  STATIC_REQUIRE(std::is_same_v<float, utils::ValueType<Vector3f>>);
  STATIC_REQUIRE(std::is_same_v<double, utils::ValueType<Vector3d>>);
}

TEMPLATE_LIST_TEST_CASE("Vector operators", "[vector_utils]", AllVectorTypes) {
  using namespace edm4hep;

  constexpr auto vector1 = create<TestType>();
  constexpr auto vector2 = create<TestType>();

  // Some very basic tests to check addition and multiplication / division by a factor
  constexpr auto sumV = vector1 + vector2;
  STATIC_REQUIRE(sumV == 2 * vector1);
  STATIC_REQUIRE(sumV == vector1 * 2); // check that both orders of args work
  STATIC_REQUIRE(sumV / 2 == vector2);

  // check that subtraction works
  STATIC_REQUIRE(sumV - vector1 == vector2);

  // Vector product (depends again on whether it is 2D or 3D)
  if constexpr (std::is_same_v<TestType, edm4hep::Vector2i> || std::is_same_v<TestType, edm4hep::Vector2f>) {
    STATIC_REQUIRE(vector1 * vector2 == utils::ValueType<TestType>(5));
  } else {
    STATIC_REQUIRE(vector1 * vector2 == utils::ValueType<TestType>(14));
  }
}
