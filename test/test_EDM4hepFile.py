#!/usr/bin/env python3
"""Pytest module that can be used to check whether the file that has been
created by scripts/createEDM4hepFile.py has the expected contents
"""

import os
import re
import podio
import edm4hep
import pytest
from itertools import count

from edm4hep import __version__

from conftest import options

# For now simply copy these from createEDM4hepFile.py
FRAMES = 3
VECTORSIZE = 5
COUNT_START = 42  # Starting point for the counters

# Skip the test if an rntuple file has not been produced
if "rntuple" in options.inputfile and not os.path.isfile(options.inputfile):
    pytest.skip(
        "Skipping rntuple reading tests, because input is not produced",
        allow_module_level=True,
    )


@pytest.fixture(scope="module")
def inputfile_name(pytestconfig):
    """Get the inputfile name that has been passed via commandline"""
    return pytestconfig.getoption("inputfile")


VERSIONED_FILE_RGX = re.compile(
    r"edm4hep_example_(?:rntuple_)?v(\d+-\d+(?:-\d+)?)_podio_v(\d+-\d+(?:-\d+)?).root"
)


@pytest.fixture(scope="module")
def expected_edm4hep_version(inputfile_name):
    """Get the expected edm4hep version from the file name"""
    rgx_match = re.search(VERSIONED_FILE_RGX, inputfile_name)
    if not rgx_match:
        return podio.version.parse(__version__)
    return podio.version.parse(rgx_match.group(1).replace("-", "."))


@pytest.fixture(scope="module")
def expected_podio_version(inputfile_name):
    """Get the expected edm4hep version from the file name"""
    rgx_match = re.search(VERSIONED_FILE_RGX, inputfile_name)
    if not rgx_match:
        return podio.version.build_version
    return podio.version.parse(rgx_match.group(2).replace("-", "."))


@pytest.fixture(scope="module")
def reader(inputfile_name):
    """Get the reader for the passed filename"""
    return podio.reading.get_reader(inputfile_name)


@pytest.fixture(scope="module")
def events(reader):
    """Get the events from the reader"""
    return reader.get("events")


@pytest.fixture(scope="module")
def event(events):
    """Get the first event from the events. Since all events have the same
    contents, we simply assume that everything is alright if we simply check the
    first one
    """
    return events[0]


@pytest.fixture(scope="module")
def particle(event):
    """Get the particle that is used in some relations"""
    return event.get("MCParticleCollection")[0]


@pytest.fixture(scope="module")
def reco_particle(event):
    """Get the reconstructed particle that is used in some relations"""
    return event.get("ReconstructedParticleCollection")[0]


@pytest.fixture(scope="module")
def track(event):
    """Get the track that is used in some relations"""
    return event.get("TrackCollection")[0]


@pytest.fixture(scope="module")
def edm4hep_version(reader):
    return reader.current_file_version("edm4hep")


def check_cov_matrix(cov_matrix, n_dim):
    """Check the contents of the passed covariance matrix"""
    counter = count(COUNT_START)
    # for val in cov_matrix: # Doesn't work as expected with root
    for i in range(n_dim * (n_dim + 1) // 2):
        assert cov_matrix[i] == next(counter)


def test_basic_file_contents(reader, events, expected_edm4hep_version, expected_podio_version):
    """Make sure the basic file contents are OK"""
    assert len(events) == FRAMES
    assert reader.current_file_version("edm4hep") == expected_edm4hep_version
    assert reader.current_file_version() == expected_podio_version


def test_EventHeaderCollection(event):
    """Check an EventHeaderCollection"""
    counter = count(COUNT_START)
    header = event.get("EventHeader")
    assert len(header) == 1
    h = header[0]
    assert h.getEventNumber() == next(counter)
    assert h.getRunNumber() == next(counter)
    assert h.getTimeStamp() == next(counter)
    assert h.getWeight() == 1.0
    assert len(h.getWeights()) == VECTORSIZE
    for weight in h.getWeights():
        assert weight == next(counter)


def test_MCParticleCollection(event, edm4hep_version):
    """Check the MCParticleCollection"""
    counter = count(COUNT_START)
    particles = event.get("MCParticleCollection")
    assert len(particles) == 3
    for i in range(3):
        particle = particles[i]
        assert particle.getPDG() == next(counter)
        assert particle.getGeneratorStatus() == next(counter)
        assert particle.getSimulatorStatus() == next(counter)
        assert particle.getCharge() == next(counter)
        assert particle.getTime() == next(counter)
        assert particle.getMass() == next(counter)
        assert particle.getVertex() == edm4hep.Vector3d(
            next(counter), next(counter), next(counter)
        )
        assert particle.getEndpoint() == edm4hep.Vector3d(
            next(counter), next(counter), next(counter)
        )
        assert particle.getMomentum() == edm4hep.Vector3d(
            next(counter), next(counter), next(counter)
        )
        assert particle.getMomentumAtEndpoint() == edm4hep.Vector3d(
            next(counter), next(counter), next(counter)
        )

        if edm4hep_version < podio.version.parse("0.99.2"):
            # The spin 3D vector has been replaced by helicity and schema
            # evolution puts the contents of the z-component into the helicity
            # as that was the main use case in any case
            next(counter)
            next(counter)
        assert particle.getHelicity() == next(counter)

        if edm4hep_version < podio.version.parse("0.99.2"):
            # The colorFlow was here so we have increase the counter here to
            # maintain the expected values for all elements of the collection
            next(counter)
            next(counter)

    assert particles[0].getDaughters()[0] == particles[1]
    assert particles[0].getParents()[0] == particles[2]


def test_SimTrackerHitCollection(event, particle):
    """Check the SimTrackerHitCollection"""
    counter = count(COUNT_START)
    hits = event.get("SimTrackerHitCollection")
    assert len(hits) == 1
    hit = hits[0]
    assert hit.getCellID() == next(counter)
    assert hit.getEDep() == next(counter)
    assert hit.getTime() == next(counter)
    assert hit.getPathLength() == next(counter)
    assert hit.getQuality() == next(counter)
    assert hit.getPosition() == edm4hep.Vector3d(next(counter), next(counter), next(counter))
    assert hit.getMomentum() == edm4hep.Vector3f(next(counter), next(counter), next(counter))

    assert hit.getParticle() == particle


def test_CaloHitContributionCollection(event, particle):
    """Check the CaloHitContributionCollection"""
    counter = count(COUNT_START)
    hits = event.get("CaloHitContributionCollection")
    assert len(hits) == 1
    hit = hits[0]
    assert hit.getPDG() == next(counter)
    assert hit.getEnergy() == next(counter)
    assert hit.getTime() == next(counter)
    assert hit.getStepPosition() == edm4hep.Vector3f(next(counter), next(counter), next(counter))

    assert hit.getParticle() == particle


def test_SimCalorimeterHitCollection(event):
    """Check the SimCalorimeterHitCollection"""
    counter = count(COUNT_START)
    hits = event.get("SimCalorimeterHitCollection")
    assert len(hits) == 1
    hit = hits[0]
    assert hit.getCellID() == next(counter)
    assert hit.getEnergy() == next(counter)
    assert hit.getPosition() == edm4hep.Vector3f(next(counter), next(counter), next(counter))

    calo_contrib = event.get("CaloHitContributionCollection")[0]
    assert len(hit.getContributions()) == 1
    assert hit.getContributions()[0] == calo_contrib


def test_RawCalorimeterHitCollection(event):
    """Check the RawCalorimeterHitCollection"""
    counter = count(COUNT_START)
    hits = event.get("RawCalorimeterHitCollection")
    assert len(hits) == 1
    hit = hits[0]
    assert hit.getCellID() == next(counter)
    assert hit.getAmplitude() == next(counter)
    assert hit.getTimeStamp() == next(counter)


def test_CalorimeterHitCollection(event):
    """Check the CalorimeterHitCollection"""
    counter = count(COUNT_START)
    hits = event.get("CalorimeterHitCollection")
    assert len(hits) == 1
    hit = hits[0]
    assert hit.getCellID() == next(counter)
    assert hit.getEnergy() == next(counter)
    assert hit.getEnergyError() == next(counter)
    assert hit.getTime() == next(counter)
    assert hit.getPosition() == edm4hep.Vector3f(next(counter), next(counter), next(counter))
    assert hit.getType() == next(counter)


def test_ParticleIDCollection(event, reco_particle):
    """Check the ParticleIDCollection"""
    counter = count(COUNT_START)
    pids = event.get("ParticleIDCollection")
    assert len(pids) == 1
    pid = pids[0]
    assert pid.getType() == next(counter)
    assert pid.getPDG() == next(counter)
    assert pid.getAlgorithmType() == next(counter)
    assert pid.getLikelihood() == next(counter)
    assert len(pid.getParameters()) == VECTORSIZE
    for param in pid.getParameters():
        assert param == next(counter)

    assert pid.getParticle() == reco_particle


def test_ClusterCollection(event):
    """Check the ClusterCollection"""
    counter = count(COUNT_START)
    clusters = event.get("ClusterCollection")
    assert len(clusters) == 1
    cluster = clusters[0]
    assert cluster.getType() == next(counter)
    assert cluster.getEnergy() == next(counter)
    assert cluster.getEnergyError() == next(counter)
    assert cluster.getPosition() == edm4hep.Vector3f(next(counter), next(counter), next(counter))
    check_cov_matrix(cluster.getPositionError(), 3)
    assert cluster.getITheta() == next(counter)
    assert cluster.getPhi() == next(counter)
    assert cluster.getDirectionError() == edm4hep.Vector3f(
        next(counter), next(counter), next(counter)
    )

    assert len(cluster.getShapeParameters()) == VECTORSIZE
    assert len(cluster.getSubdetectorEnergies()) == VECTORSIZE
    for j in range(VECTORSIZE):
        cluster.getShapeParameters()[j] == next(counter)
        cluster.getSubdetectorEnergies()[j] == next(counter)

    assert len(cluster.getClusters()) == 1
    assert cluster.getClusters()[0] == cluster

    calo_hit = event.get("CalorimeterHitCollection")[0]
    assert len(cluster.getHits()) == 1
    assert cluster.getHits()[0] == calo_hit


def test_TrackerHit3DCollection(event):
    """Check the TrackerHit3DCollection"""
    counter = count(COUNT_START)
    hits = event.get("TrackerHit3DCollection")
    assert len(hits) == 1
    hit = hits[0]
    assert hit.getCellID() == next(counter)
    assert hit.getType() == next(counter)
    assert hit.getQuality() == next(counter)
    assert hit.getTime() == next(counter)
    assert hit.getEDep() == next(counter)
    assert hit.getEDepError() == next(counter)
    assert hit.getPosition() == edm4hep.Vector3d(next(counter), next(counter), next(counter))
    check_cov_matrix(hit.getCovMatrix(), 3)


def test_TrackerHitPlaneCollection(event):
    """Check the TrackerHitPlaneCollection"""
    counter = count(COUNT_START)
    hits = event.get("TrackerHitPlaneCollection")
    assert len(hits) == 1
    hit = hits[0]
    assert hit.getCellID() == next(counter)
    assert hit.getType() == next(counter)
    assert hit.getQuality() == next(counter)
    assert hit.getTime() == next(counter)
    assert hit.getEDep() == next(counter)
    assert hit.getEDepError() == next(counter)
    assert hit.getU() == edm4hep.Vector2f(next(counter), next(counter))
    assert hit.getV() == edm4hep.Vector2f(next(counter), next(counter))
    assert hit.getDu() == next(counter)
    assert hit.getDv() == next(counter)
    assert hit.getPosition() == edm4hep.Vector3d(next(counter), next(counter), next(counter))
    check_cov_matrix(hit.getCovMatrix(), 3)


def test_RawTimeSeriesCollection(event):
    """Check a RawTimeSeriesCollection"""
    counter = count(COUNT_START)
    series = event.get("RawTimeSeriesCollection")
    assert len(series) == 1
    serie = series[0]
    assert serie.getCellID() == next(counter)
    assert serie.getQuality() == next(counter)
    assert serie.getTime() == next(counter)
    assert serie.getCharge() == next(counter)
    assert serie.getInterval() == next(counter)
    assert len(serie.getAdcCounts()) == VECTORSIZE
    for val in serie.getAdcCounts():
        assert val == next(counter)


def test_TrackCollection(event):
    """Check the TrackCollection"""
    counter = count(COUNT_START)
    tracks = event.get("TrackCollection")
    assert len(tracks) == 1
    track = tracks[0]
    assert track.getType() == next(counter)
    assert track.getChi2() == next(counter)
    assert track.getNdf() == next(counter)

    subdetHitNumbers = track.getSubdetectorHitNumbers()
    assert len(subdetHitNumbers) == VECTORSIZE
    subdetHoleNumbers = track.getSubdetectorHoleNumbers()
    assert len(subdetHoleNumbers) == VECTORSIZE
    trackStates = track.getTrackStates()
    assert len(trackStates) == VECTORSIZE

    for j in range(VECTORSIZE):
        subdetHitNumbers[j] == next(counter)
        subdetHoleNumbers[j] == next(counter)

        state = trackStates[j]
        assert state.location == next(counter)
        assert state.D0 == next(counter)
        assert state.phi == next(counter)
        assert state.omega == next(counter)
        assert state.Z0 == next(counter)
        assert state.tanLambda == next(counter)
        assert state.time == next(counter)
        assert state.referencePoint == edm4hep.Vector3f(
            next(counter), next(counter), next(counter)
        )
        check_cov_matrix(state.covMatrix, 6)

    assert track.getNholes() == next(counter)

    tracker_hit = event.get("TrackerHit3DCollection")[0]
    assert len(track.getTrackerHits()) == 1
    assert track.getTrackerHits()[0] == tracker_hit

    assert len(track.getTracks()) == 1
    assert track.getTracks()[0] == track


def test_VertexCollection(event, reco_particle):
    """Check the VertexCollection"""
    counter = count(COUNT_START)
    vertex = event.get("VertexCollection")
    assert len(vertex) == 1
    v = vertex[0]
    assert v.getType() == next(counter)
    assert v.getChi2() == next(counter)
    assert v.getNdf() == next(counter)
    assert v.getPosition() == edm4hep.Vector3f(next(counter), next(counter), next(counter))
    check_cov_matrix(v.getCovMatrix(), 3)
    assert v.getAlgorithmType() == next(counter)
    assert len(v.getParameters()) == VECTORSIZE
    for val in v.getParameters():
        assert val == next(counter)

    assert len(v.getParticles()) == 1
    assert v.getParticles()[0] == reco_particle


def test_ReconstructedParticleCollection(event, track):
    """Check the ReconstructedParticleCollection"""
    counter = count(COUNT_START)
    rparticles = event.get("ReconstructedParticleCollection")
    assert len(rparticles) == 1
    rparticle = rparticles[0]
    assert rparticle.getPDG() == next(counter)
    assert rparticle.getEnergy() == next(counter)
    assert rparticle.getMomentum() == edm4hep.Vector3f(next(counter), next(counter), next(counter))
    assert rparticle.getReferencePoint() == edm4hep.Vector3f(
        next(counter), next(counter), next(counter)
    )
    assert rparticle.getCharge() == next(counter)
    assert rparticle.getMass() == next(counter)
    assert rparticle.getGoodnessOfPID() == next(counter)
    check_cov_matrix(rparticle.getCovMatrix(), 4)

    vertex = event.get("VertexCollection")[0]
    assert rparticle.getDecayVertex() == vertex

    assert len(rparticle.getClusters()) == 1
    cluster = event.get("ClusterCollection")[0]
    assert rparticle.getClusters()[0] == cluster

    assert len(rparticle.getTracks()) == 1
    assert rparticle.getTracks()[0] == track

    assert len(rparticle.getParticles()) == 1
    assert rparticle.getParticles()[0] == rparticle


def test_TimeSeriesCollection(event):
    """Check the TimeSeriesCollection"""
    counter = count(COUNT_START)
    timeseries = event.get("TimeSeriesCollection")
    assert len(timeseries) == 1
    serie = timeseries[0]
    assert serie.getCellID() == next(counter)
    assert serie.getTime() == next(counter)
    assert serie.getInterval() == next(counter)
    assert len(serie.getAmplitude()) == VECTORSIZE
    for val in serie.getAmplitude():
        assert val == next(counter)


def test_RecDqdxCollection(event, track):
    """Check the RecDqdxCollection"""
    counter = count(COUNT_START)
    recdqdx = event.get("RecDqdxCollection")
    assert len(recdqdx) == 1
    dqdx = recdqdx[0]
    q = dqdx.getDQdx()
    assert q.type == next(counter)
    assert q.value == next(counter)
    assert q.error == next(counter)
    assert dqdx.getTrack() == track


def test_GeneratorEventParametersCollection(event, particle):
    """Check the GeneratorEventParametersCollection"""
    counter = count(COUNT_START)
    gep_coll = event.get("GeneratorEventParametersCollection")
    assert len(gep_coll) == 1
    gep = gep_coll[0]
    assert gep.getEventScale() == next(counter)
    assert gep.getAlphaQED() == next(counter)
    assert gep.getAlphaQCD() == next(counter)
    assert gep.getSignalProcessId() == next(counter)
    assert gep.getSqrts() == next(counter)
    xsecs = gep.getCrossSections()
    xsec_errors = gep.getCrossSectionErrors()
    assert len(xsecs) == VECTORSIZE
    assert len(xsec_errors) == VECTORSIZE
    for i in range(VECTORSIZE):
        assert xsecs[i] == next(counter)
        assert xsec_errors[i] == next(counter)

    assert len(gep.getSignalVertex()) == 1
    assert gep.getSignalVertex()[0] == particle


def test_GeneratorPdfInfoCollection(event):
    """Check the GeneratorPdfInfoCollection"""
    counter = count(COUNT_START)
    gpi_coll = event.get("GeneratorPdfInfoCollection")
    assert len(gpi_coll) == 1
    gpi = gpi_coll[0]
    assert gpi.getPartonId(0) == next(counter)
    assert gpi.getPartonId(1) == next(counter)
    assert gpi.getLhapdfId(0) == next(counter)
    assert gpi.getLhapdfId(1) == next(counter)
    assert gpi.getX(0) == next(counter)
    assert gpi.getX(1) == next(counter)
    assert gpi.getXf(0) == next(counter)
    assert gpi.getXf(1) == next(counter)

    assert gpi.getScale() == next(counter)


def check_LinkCollection(event, coll_type, from_el, to_el):
    """Check a single link collection of a given type"""
    counter = count(COUNT_START)
    coll_name = f"{coll_type}Collection"
    link_coll = event.get(coll_name)
    assert len(link_coll) == 1
    link = link_coll[0]
    assert link.getWeight() == next(counter)
    assert link.getFrom() == from_el
    assert link.getTo() == to_el


def test_LinkCollections(event, particle, reco_particle, track):
    """Check all the link collections"""
    calo_hit = event.get("CalorimeterHitCollection")[0]
    simcalo_hit = event.get("SimCalorimeterHitCollection")[0]
    cluster = event.get("ClusterCollection")[0]
    tracker_hit = event.get("TrackerHit3DCollection")[0]
    simtracker_hit = event.get("SimTrackerHitCollection")[0]
    vertex = event.get("VertexCollection")[0]

    check_LinkCollection(event, "RecoMCParticleLink", reco_particle, particle)
    check_LinkCollection(event, "CaloHitSimCaloHitLink", calo_hit, simcalo_hit)
    check_LinkCollection(event, "TrackerHitSimTrackerHitLink", tracker_hit, simtracker_hit)
    check_LinkCollection(event, "CaloHitMCParticleLink", calo_hit, particle)
    check_LinkCollection(event, "ClusterMCParticleLink", cluster, particle)
    check_LinkCollection(event, "TrackMCParticleLink", track, particle)
    check_LinkCollection(event, "VertexRecoParticleLink", vertex, reco_particle)
