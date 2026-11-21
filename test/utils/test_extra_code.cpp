#include "edm4hep/CaloHitContributionCollection.h"
#include "edm4hep/MutableSimCalorimeterHit.h"
#include "edm4hep/MutableTrack.h"
#include "edm4hep/SimCalorimeterHit.h"
#include "edm4hep/Track.h"
#include "edm4hep/TrackState.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <numbers>
#include <optional>

namespace {
// Two helper concepts that we can use to statically check whether indexing into
// getTrackStates (plural!) compiles or not. We want this to work with integer
// types but we want this to fail with the TrackState constants. We assert this
// in the tests below
template <typename T>
concept IndexableWithLocation = requires(const T& t) { t.getTrackStates(edm4hep::TrackState::AtIP); };

template <typename T>
concept IndexableWithInteger = requires(const T& t) { t.getTrackStates(0); };
} // namespace

TEST_CASE("Track::getTrackState") {
  // Populate a track with track states
  auto mutableTrack = edm4hep::MutableTrack{};
  mutableTrack.addToTrackStates(edm4hep::TrackState{.location = edm4hep::TrackState::AtIP});
  auto inputState = edm4hep::TrackState{.location = edm4hep::TrackState::AtCalorimeter, .D0 = 3.13f, .phi = 42.0f};
  mutableTrack.addToTrackStates(inputState);
  mutableTrack.addToTrackStates(edm4hep::TrackState{.location = edm4hep::TrackState::AtCalorimeter, .D0 = 99.f});

  SECTION("returns desired track state via MutableTrack") {
    auto state = mutableTrack.getTrackState(edm4hep::TrackState::AtCalorimeter);
    REQUIRE(state.has_value());
    REQUIRE(state.value().D0 == inputState.D0);
    REQUIRE(state.value().phi == inputState.phi);
  }

  SECTION("returns desired track state via const Track") {
    const edm4hep::Track track = mutableTrack;
    auto state = track.getTrackState(edm4hep::TrackState::AtCalorimeter);
    REQUIRE(state.has_value());
    REQUIRE(state.value().D0 == inputState.D0);
    REQUIRE(state.value().phi == inputState.phi);
  }

  SECTION("returns first matching track state when multiple match") {
    auto state = mutableTrack.getTrackState(edm4hep::TrackState::AtCalorimeter);
    REQUIRE(state.has_value());
    REQUIRE(state.value().D0 == inputState.D0); // first added, not the duplicate
  }

  SECTION("returns empty optional when no track state matches") {
    REQUIRE_FALSE(mutableTrack.getTrackState(edm4hep::TrackState::AtFirstHit).has_value());
  }

  SECTION("No confusing overloads") {
    // indexing with ints should work
    STATIC_REQUIRE(IndexableWithInteger<edm4hep::Track>);
    STATIC_REQUIRE(IndexableWithInteger<edm4hep::MutableTrack>);
    // but using the track states should not
    STATIC_REQUIRE(!IndexableWithLocation<edm4hep::Track>);
    STATIC_REQUIRE(!IndexableWithLocation<edm4hep::MutableTrack>);
  }
}

TEST_CASE("SimCalorimeterHit::addMCParticleContribution") {
  // The energies are powers of two to have exact sums
  auto contributions = edm4hep::CaloHitContributionCollection{};
  for (const auto energy : {1.f, 2.f, 4.f}) {
    contributions.create().setEnergy(energy);
  }

  SECTION("sums up the energies of the contributions") {
    auto mutableHit = edm4hep::MutableSimCalorimeterHit{};
    for (const auto contrib : contributions) {
      mutableHit.addMCParticleContribution(contrib);
    }

    const edm4hep::SimCalorimeterHit hit = mutableHit;
    REQUIRE(hit.getEnergy() == 7.f);
    REQUIRE(hit.getContributions().size() == 3);
    for (size_t i = 0; i < contributions.size(); ++i) {
      REQUIRE(hit.getContributions()[i] == contributions[i]);
    }
  }

  SECTION("adds to the energy that is already present") {
    auto mutableHit = edm4hep::MutableSimCalorimeterHit{};
    mutableHit.setEnergy(8.f);
    mutableHit.addMCParticleContribution(contributions[0]);

    REQUIRE(mutableHit.getEnergy() == 9.f);
    REQUIRE(mutableHit.getContributions().size() == 1);
  }

  SECTION("addToContributions leaves the energy untouched") {
    auto mutableHit = edm4hep::MutableSimCalorimeterHit{};
    mutableHit.addToContributions(contributions[0]);

    REQUIRE(mutableHit.getEnergy() == 0.f);
    REQUIRE(mutableHit.getContributions().size() == 1);
  }
}

TEST_CASE("TrackState::getMomentum") {
  // A track with a radius of 1 m in a 2 T field has a pT of ~0.6 GeV
  constexpr double Bz = 2.0;
  constexpr float omega = 1e-3f; // 1 / mm
  constexpr double expectedPt = 2.998e-4 * Bz / omega;

  SECTION("momentum points along x for phi = 0 and tanLambda = 0") {
    const auto state = edm4hep::TrackState{.phi = 0.f, .omega = omega, .tanLambda = 0.f};
    const auto mom = state.getMomentum(Bz);
    REQUIRE(mom.x == Catch::Approx(expectedPt));
    REQUIRE(mom.y == Catch::Approx(0.0).margin(1e-6));
    REQUIRE(mom.z == Catch::Approx(0.0).margin(1e-6));
  }

  SECTION("phi and tanLambda determine the direction") {
    const auto state = edm4hep::TrackState{.phi = std::numbers::pi_v<float> / 2.f, .omega = omega, .tanLambda = 2.f};
    const auto mom = state.getMomentum(Bz);
    REQUIRE(mom.x == Catch::Approx(0.0).margin(1e-6));
    REQUIRE(mom.y == Catch::Approx(expectedPt));
    REQUIRE(mom.z == Catch::Approx(2 * expectedPt));
  }

  SECTION("transverse momentum is independent of phi") {
    const auto state = edm4hep::TrackState{.phi = 0.7f, .omega = omega, .tanLambda = 0.5f};
    const auto mom = state.getMomentum(Bz);
    REQUIRE(std::hypot(mom.x, mom.y) == Catch::Approx(expectedPt));
    REQUIRE(mom.z == Catch::Approx(0.5 * expectedPt));
  }

  SECTION("sign of omega (charge) does not affect the momentum") {
    const auto posState = edm4hep::TrackState{.phi = 0.3f, .omega = omega, .tanLambda = 1.f};
    const auto negState = edm4hep::TrackState{.phi = 0.3f, .omega = -omega, .tanLambda = 1.f};
    const auto posMom = posState.getMomentum(Bz);
    const auto negMom = negState.getMomentum(Bz);
    REQUIRE(posMom.x == Catch::Approx(negMom.x));
    REQUIRE(posMom.y == Catch::Approx(negMom.y));
    REQUIRE(posMom.z == Catch::Approx(negMom.z));
  }

  SECTION("momentum scales linearly with the field and inversely with omega") {
    const auto state = edm4hep::TrackState{.phi = 0.f, .omega = omega, .tanLambda = 0.f};
    const auto halfCurvState = edm4hep::TrackState{.phi = 0.f, .omega = omega / 2.f, .tanLambda = 0.f};
    REQUIRE(state.getMomentum(2 * Bz).x == Catch::Approx(2 * expectedPt));
    REQUIRE(halfCurvState.getMomentum(Bz).x == Catch::Approx(2 * expectedPt));
  }
}
