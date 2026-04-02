#include "edm4hep/TrackState.h"
#include <edm4hep/TrackCollection.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Track::getTrackState returns desired track state") {
  auto track = edm4hep::MutableTrack{};
  track.addToTrackStates(edm4hep::TrackState{.location = edm4hep::TrackState::AtIP});
  auto inputState = edm4hep::TrackState{.location = edm4hep::TrackState::AtCalorimeter, .D0 = 3.13f, .phi = 42.0f};
  track.addToTrackStates(inputState);

  auto stateAtCalo = track.getTrackState(edm4hep::TrackState::AtCalorimeter);
  REQUIRE(stateAtCalo.has_value());
  REQUIRE(stateAtCalo.value().D0 == inputState.D0);
  REQUIRE(stateAtCalo.value().phi == inputState.phi);
}

TEST_CASE("Track::getTrackState returns first found track state") {
  auto track = edm4hep::MutableTrack{};
  track.addToTrackStates(edm4hep::TrackState{.location = edm4hep::TrackState::AtIP});
  auto inputState = edm4hep::TrackState{.location = edm4hep::TrackState::AtCalorimeter, .D0 = 3.13f, .phi = 42.0f};
  track.addToTrackStates(inputState);
  track.addToTrackStates(edm4hep::TrackState{.location = edm4hep::TrackState::AtCalorimeter});

  auto firstStateAtCalo = track.getTrackState(edm4hep::TrackState::AtCalorimeter);
  REQUIRE(firstStateAtCalo.has_value());
  REQUIRE(firstStateAtCalo.value().D0 == inputState.D0);
  REQUIRE(firstStateAtCalo.value().phi == inputState.phi);
}

TEST_CASE("Track::getTrackState returns empty optional if no track state found") {
  auto track = edm4hep::MutableTrack{};
  track.addToTrackStates(edm4hep::TrackState{.location = edm4hep::TrackState::AtIP});
  track.addToTrackStates(edm4hep::TrackState{.location = edm4hep::TrackState::AtCalorimeter});

  REQUIRE_FALSE(track.getTrackState(edm4hep::TrackState::AtFirstHit).has_value());
}
