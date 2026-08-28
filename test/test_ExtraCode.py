#!/usr/bin/env python3

import edm4hep

# TrackState locations. Make sure that the enumerator names are available in
# python, both via the enum and directly from the TrackState
assert edm4hep.TrackState.Location.AtIP == 1
assert edm4hep.TrackState.AtIP == 1
assert edm4hep.TrackState.Location.AtVertex == edm4hep.TrackState.AtVertex

# The Location enum is unscoped, so the constants can be assigned to and compared
# with the location member directly
trackState = edm4hep.TrackState()
assert trackState.location == edm4hep.TrackState.AtOther
trackState.location = edm4hep.TrackState.AtVertex
assert trackState.location == 5
assert trackState.location == edm4hep.TrackState.AtVertex

# Track::getTrackState returns a std::optional.track = edm4hep.MutableTrack()
# NOTE: when using this in python it is important to bind the optional to a
# variable as trying to use value() on a temporary will result in a dangling
# reference otherwise
track = edm4hep.MutableTrack()
track.addToTrackStates(trackState)
trackStateAtVertex = track.getTrackState(edm4hep.TrackState.AtVertex)
assert trackStateAtVertex.has_value()
assert trackStateAtVertex.value().location == edm4hep.TrackState.AtVertex
assert not track.getTrackState(edm4hep.TrackState.AtIP).has_value()
