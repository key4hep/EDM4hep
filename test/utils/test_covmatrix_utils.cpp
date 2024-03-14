#include "edm4hep/Constants.h"
#include "edm4hep/TrackState.h"
#include "edm4hep/utils/cov_matrix_utils.h"

#include <catch2/catch_test_macros.hpp>

#include <stdexcept>

TEST_CASE("CovarianceMatrix indexing", "[cov_matrix_utils]") {
  using namespace edm4hep::utils;

  STATIC_REQUIRE(get_cov_dim(21) == 6);
  STATIC_REQUIRE(get_cov_dim(1) == 1);
  REQUIRE_THROWS_AS(get_cov_dim(14), std::invalid_argument);

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

TEST_CASE("TrackState covariance", "[cov_matrix_utils]") {
  auto trackState = edm4hep::TrackState{};

  trackState.setCovMatrix(1.23f, edm4hep::TrackParams::d0, edm4hep::TrackParams::phi);
  // We know the expected index in this case
  REQUIRE(trackState.covMatrix.values[1] == 1.23f);
  REQUIRE(trackState.getCovMatrix(edm4hep::TrackParams::time, edm4hep::TrackParams::omega) == 0);
}
