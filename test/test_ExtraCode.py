#!/usr/bin/env python3
"""Tests for the extra code that is injected via the ExtraCode YAML directives"""

import pytest

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


def test_sim_calorimeter_hit_add_mc_particle_contribution():
    """MutableSimCalorimeterHit::addMCParticleContribution attaches the
    contribution and adds its energy to the energy of the hit"""
    contributions = edm4hep.CaloHitContributionCollection()
    for energy in (1.0, 2.0, 4.0):
        contributions.create().setEnergy(energy)

    hit = edm4hep.MutableSimCalorimeterHit()
    for contrib in contributions:
        hit.addMCParticleContribution(contrib)

    assert hit.getEnergy() == 7.0
    assert hit.getContributions().size() == 3
    assert [c.getEnergy() for c in hit.getContributions()] == [1.0, 2.0, 4.0]


def test_track_state_get_momentum():
    """TrackState::getMomentum computes the momentum from the track parameters
    and the (solenoidal) magnetic field along z"""
    bz = 2.0  # T
    omega = 1e-3  # 1 / mm, i.e. a radius of 1 m
    expected_pt = 2.998e-4 * bz / omega

    trackState = edm4hep.TrackState()
    trackState.omega = omega
    trackState.phi = 0.7
    trackState.tanLambda = 2.0

    mom = trackState.getMomentum(bz)
    utils = edm4hep.utils
    assert utils.magnitudeTransverse(mom) == pytest.approx(expected_pt, rel=1e-6)
    assert utils.magnitudeLongitudinal(mom) == pytest.approx(2 * expected_pt, rel=1e-6)
    assert utils.angleAzimuthal(mom) == pytest.approx(trackState.phi, rel=1e-6)

    # The sign of omega (i.e. the charge) does not affect the momentum
    trackState.omega = -omega
    assert trackState.getMomentum(bz) == mom
