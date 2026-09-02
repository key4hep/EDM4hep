#include "edm4hep/TrackState.h"
#include <edm4hep/TrackCollection.h>

#include <catch2/catch_test_macros.hpp>

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
