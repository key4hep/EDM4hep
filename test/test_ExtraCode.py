#!/usr/bin/env python3
"""Tests for the extra code that is injected via the ExtraCode YAML directives"""

import edm4hep


def test_track_state_location_enum_values():
    """Make sure that the enumerator names are available in python, both via
    the enum and directly from the TrackState"""
    assert edm4hep.TrackState.Location.AtIP == 1
    assert edm4hep.TrackState.AtIP == 1
    assert edm4hep.TrackState.Location.AtVertex == edm4hep.TrackState.AtVertex


def test_track_state_location_assignment():
    """The Location enum is unscoped, so the constants can be assigned to and
    compared with the location member directly"""
    trackState = edm4hep.TrackState()
    assert trackState.location == edm4hep.TrackState.AtOther
    trackState.location = edm4hep.TrackState.AtVertex
    assert trackState.location == 5
    assert trackState.location == edm4hep.TrackState.AtVertex


def test_track_get_track_state():
    """Track::getTrackState returns a std::optional.
    NOTE: when using this in python it is important to bind the optional to a
    variable, as trying to use value() on a temporary will result in a
    dangling reference otherwise"""
    trackState = edm4hep.TrackState()
    trackState.location = edm4hep.TrackState.AtVertex

    track = edm4hep.MutableTrack()
    track.addToTrackStates(trackState)

    trackStateAtVertex = track.getTrackState(edm4hep.TrackState.AtVertex)
    assert trackStateAtVertex.has_value()
    assert trackStateAtVertex.value().location == edm4hep.TrackState.AtVertex
    assert not track.getTrackState(edm4hep.TrackState.AtIP).has_value()
