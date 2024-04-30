#include "edm4hep/Constants.h"
#include "edm4hep/CovMatrix3f.h"
#include "edm4hep/MutableTrackerHit3D.h"
#include "edm4hep/TrackState.h"
#include "edm4hep/TrackerHit3D.h"
#include "edm4hep/utils/cov_matrix_utils.h"

#include <catch2/catch_test_macros.hpp>

#include <stdexcept>

TEST_CASE("CovarianceMatrix indexing", "[cov_matrix_utils]") {
  using namespace edm4hep::utils::detail;

  STATIC_REQUIRE(get_cov_dim(21) == 6);
  STATIC_REQUIRE(get_cov_dim(1) == 1);
#if !__cpp_consteval
  // This will fail to compile if we have consteval
  REQUIRE_THROWS_AS(get_cov_dim(14), std::invalid_argument);
#endif

  // clang-format off
  // For better interpretability of the tests below, these are the indices of a
  // 2D matrix in lower triangular form together with the matrix indices
  //
  //   | 0  1  2  3  4  5
  // --+------------------
  // 0 | 0  1  3  6  10 15
  // 1 | 1  2  4  7  11 16
  // 2 | 3  4  5  8  12 17
  // 3 | 6  7  8  9  13 18
  // 4 | 10 11 12 13 14 19
  // 5 | 15 16 17 18 19 20
  // clang-format on

  // diagonal elements
  STATIC_REQUIRE(to_lower_tri(0, 0) == 0);
  STATIC_REQUIRE(to_lower_tri(1, 1) == 2);
  STATIC_REQUIRE(to_lower_tri(2, 2) == 5);
  STATIC_REQUIRE(to_lower_tri(3, 3) == 9);
  STATIC_REQUIRE(to_lower_tri(4, 4) == 14);
  STATIC_REQUIRE(to_lower_tri(5, 5) == 20);

  // some off diagonal elements
  STATIC_REQUIRE(to_lower_tri(1, 0) == 1);
  STATIC_REQUIRE(to_lower_tri(0, 1) == 1);
  STATIC_REQUIRE(to_lower_tri(0, 2) == 3);
  STATIC_REQUIRE(to_lower_tri(2, 0) == 3);
  STATIC_REQUIRE(to_lower_tri(2, 3) == 8);
  STATIC_REQUIRE(to_lower_tri(3, 2) == 8);
  STATIC_REQUIRE(to_lower_tri(5, 3) == 18);
  STATIC_REQUIRE(to_lower_tri(2, 5) == 17);
}

TEST_CASE("CovMatrixNf array access", "[cov_matrix_utils]") {
  // We use the 3D version here, but since the ExtraCode is effectively
  // duplicated for the others as well it shouldn't really matter
  auto covMatrix = edm4hep::CovMatrix3f{};

  covMatrix[3] = 3.14f;
  REQUIRE(covMatrix[3] == 3.14f);

  REQUIRE(covMatrix.data()[3] == 3.14f);
  covMatrix.data()[2] = 2.13f;
  REQUIRE(covMatrix[2] == 2.13f);

  float i = 0.f;
  for (auto& v : covMatrix) {
    v = i++;
  }
  i = 0.f;
  for (const auto& v : covMatrix) {
    REQUIRE(v == i++);
  }
}

TEST_CASE("CovMatrixNf enum access", "[cov_matrix_utils]") {
  enum class TestDims : uint32_t { a = 0, b, c };

  auto covMatrix = edm4hep::CovMatrix3f{};
  covMatrix.setValue(1.23f, TestDims::a, TestDims::c);
  REQUIRE(covMatrix.getValue(TestDims::a, TestDims::c) == 1.23f);
}

TEST_CASE("CovMatrixNf invalid enum access", "[cov_matrix_utils]") {
  // Invalid dimensions with too many elements to fit the 3D convariance matrix
  enum class InvalidDims : edm4hep::DimType { i = 0, j, k, l, m };

  auto covMatrix = edm4hep::CovMatrix3f{};
  REQUIRE_THROWS_AS(covMatrix.setValue(1.23f, InvalidDims::k, InvalidDims::l), std::invalid_argument);
  REQUIRE_THROWS_AS(covMatrix.getValue(InvalidDims::m, InvalidDims::i), std::invalid_argument);
}

TEST_CASE("CovMatrixNf equality operators", "[cov_matrix_utils]") {
  auto covMatrix = edm4hep::CovMatrix3f{};
  covMatrix[3] = 3.14f;
  covMatrix[2] = 2.13f;
  REQUIRE(covMatrix == std::array<float, 6>{0, 0, 2.13f, 3.14f, 0, 0});
  REQUIRE(covMatrix != std::array<float, 6>{});
}

TEST_CASE("TrackState covariance", "[cov_matrix_utils]") {
  auto trackState = edm4hep::TrackState{};

  trackState.setCovMatrix(1.23f, edm4hep::TrackParams::d0, edm4hep::TrackParams::phi);
  // We know the expected index in this case
  REQUIRE(trackState.covMatrix.values[1] == 1.23f);
  REQUIRE(trackState.getCovMatrix(edm4hep::TrackParams::time, edm4hep::TrackParams::omega) == 0);
}

TEST_CASE("TrackerHit3D covariance", "[cov_matrix_utils]") {
  auto trackerHit = edm4hep::MutableTrackerHit3D{};
  trackerHit.setCovMatrix(3.14f, edm4hep::Cartesian::x, edm4hep::Cartesian::z);
  REQUIRE(trackerHit.getCovMatrix(edm4hep::Cartesian::x, edm4hep::Cartesian::z) == 3.14f);
  // We can also use the expected index of (x, y)
  REQUIRE(trackerHit.getCovMatrix().values[3] == 3.14f);

  auto hit = edm4hep::TrackerHit3D(trackerHit);
  REQUIRE(hit.getCovMatrix(edm4hep::Cartesian::x, edm4hep::Cartesian::z) == 3.14f);

  trackerHit.setCovMatrix({1.f, 2.f, 3.f, 4.f, 5.f, 6.f});
  REQUIRE(trackerHit.getCovMatrix() == std::array{1.f, 2.f, 3.f, 4.f, 5.f, 6.f});

  const std::array arrValues = {6.f, 5.f, 4.f, 3.f, 2.f, 1.f};
  trackerHit.setCovMatrix(arrValues);
  REQUIRE(trackerHit.getCovMatrix() == std::array{6.f, 5.f, 4.f, 3.f, 2.f, 1.f});
}
