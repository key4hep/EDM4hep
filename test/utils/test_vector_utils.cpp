#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "edm4hep/utils/vector_utils.h"

#include "edm4hep/Vector2f.h"
#include "edm4hep/Vector2i.h"
#include "edm4hep/Vector3d.h"
#include "edm4hep/Vector3f.h"
#include "edm4hep/Vector4f.h"

#include <tuple>
#include <type_traits>
using Vector2And3Types = std::tuple<edm4hep::Vector3f, edm4hep::Vector3d, edm4hep::Vector2i, edm4hep::Vector2f>;
using AllVectorTypes =
    std::tuple<edm4hep::Vector4f, edm4hep::Vector3f, edm4hep::Vector3d, edm4hep::Vector2i, edm4hep::Vector2f>;

template <typename V>
constexpr V create();

template <>
constexpr edm4hep::Vector4f create() {
  return edm4hep::Vector4f{1.0f, 2.0f, 3.0f, 4.0f};
}

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
  } else if constexpr (std::is_same_v<TestType, edm4hep::Vector4f>) {
    STATIC_REQUIRE(utils::vector_t(vector) == utils::ValueType<TestType>(4.0));
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
  STATIC_REQUIRE(std::is_same_v<float, utils::ValueType<Vector4f>>);
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
  } else if constexpr (std::is_same_v<TestType, edm4hep::Vector3f> || std::is_same_v<TestType, edm4hep::Vector3d>) {
    STATIC_REQUIRE(vector1 * vector2 == utils::ValueType<TestType>(14));
  } else {
    STATIC_REQUIRE(vector1 * vector2 == utils::ValueType<TestType>(-2));
  }
}

TEMPLATE_LIST_TEST_CASE("Vector utility functionality", "[vector_utils]", Vector2And3Types) {
  const auto vector = create<TestType>();

  using namespace edm4hep;

  // Can only normalize vectors with floating point numbers
  if constexpr (!std::is_same_v<TestType, Vector2i>) {
    const auto normV = utils::normalizeVector(vector);
    REQUIRE(utils::magnitude(normV) == Catch::Approx(1));
    REQUIRE(utils::projection(normV, vector) == Catch::Approx(1));
  }

  // Small differences in expectations between 2D and 3D vectors for everything
  // that involves the z component
  if constexpr (std::is_same_v<TestType, Vector2i> || std::is_same_v<TestType, Vector2f>) {
    REQUIRE(utils::magnitude(vector) == Catch::Approx(std::sqrt(5)));

    const auto otherVec = TestType(3, 4);
    REQUIRE(utils::angleBetween(otherVec, vector) == Catch::Approx(0.17985349)); // acos(11 / (5 * sqrt(5)))
  } else {
    REQUIRE(utils::magnitude(vector) == Catch::Approx(std::sqrt(14)));
    REQUIRE(utils::magnitudeTransverse(vector) == Catch::Approx(std::sqrt(5)));
    REQUIRE(utils::magnitudeLongitudinal(vector) == utils::ValueType<TestType>(3));
    REQUIRE(utils::vectorTransverse(vector) == TestType(1.0, 2.0, 0.0));
    REQUIRE(utils::vectorLongitudinal(vector) == TestType(0.0, 0.0, 3.0));

    const auto spherVec = utils::sphericalToVector(1, 1, 1);
    // expected: (0.4546487134, 0.7080734183, 0.5403023059)
    REQUIRE(spherVec.x == Catch::Approx(0.4546487134));
    REQUIRE(spherVec.y == Catch::Approx(0.7080734183));
    REQUIRE(spherVec.z == Catch::Approx(0.5403023059));

    const auto otherVec = TestType(3, 4, 5);
    REQUIRE(utils::angleBetween(otherVec, vector) == Catch::Approx(0.1862387)); // acos(26 / (sqrt(14) * sqrt(50)))
  }
}
