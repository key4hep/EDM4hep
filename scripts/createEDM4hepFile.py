#!/usr/bin/env python3

# Description: Create a file with EDM4hep data using the EDM4hep python bindings
# The purpose of the script is to use all the classes of the EDM4hep library
# to create a file with dummy data.

import argparse
import sys
from itertools import count

import podio
import edm4hep

FRAMES = 3  # How many frames or events will be written
VECTORSIZE = 5  # For vector members, each vector member will have this size

COUNT_START = 42  # Where to the counter from


def create_EventHeaderCollection(vectorsize):
    """Create an EventHeaderCollection"""
    counter = count(COUNT_START)
    header = edm4hep.EventHeaderCollection()
    h = header.create()
    h.setEventNumber(next(counter))
    h.setRunNumber(next(counter))
    h.setTimeStamp(next(counter))
    h.setWeight(1.0)
    for j in range(vectorsize):
        h.addToWeights(next(counter))
    return header


def create_MCParticleCollection():
    """Create an MCParticleCollection"""
    counter = count(COUNT_START)
    particles = edm4hep.MCParticleCollection()
    p_list = []
    for _ in range(3):
        particle = particles.create()
        p_list.append(particle)
        particle.setPDG(next(counter))
        particle.setGeneratorStatus(next(counter))
        particle.setSimulatorStatus(next(counter))
        particle.setCharge(next(counter))
        particle.setTime(next(counter))
        particle.setMass(next(counter))
        particle.setVertex(edm4hep.Vector3d(next(counter), next(counter), next(counter)))
        particle.setEndpoint(edm4hep.Vector3d(next(counter), next(counter), next(counter)))
        particle.setMomentum(edm4hep.Vector3d(next(counter), next(counter), next(counter)))
        particle.setMomentumAtEndpoint(
            edm4hep.Vector3d(next(counter), next(counter), next(counter))
        )
        particle.setHelicity(next(counter))

    p_list[0].addToDaughters(p_list[1])
    p_list[0].addToParents(p_list[2])
    return particles


def create_SimTrackerHitCollection(particle):
    """Create a SimTrackerHitCollection"""
    counter = count(COUNT_START)
    hits = edm4hep.SimTrackerHitCollection()
    hit = hits.create()
    hit.setCellID(next(counter))
    hit.setEDep(next(counter))
    hit.setTime(next(counter))
    hit.setPathLength(next(counter))
    hit.setQuality(next(counter))
    hit.setPosition(edm4hep.Vector3d(next(counter), next(counter), next(counter)))
    hit.setMomentum(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    hit.setParticle(particle)
    return hits


def create_CaloHitContributionCollection(particle):
    """Create a CaloHitContributionCollection"""
    counter = count(COUNT_START)
    hits = edm4hep.CaloHitContributionCollection()
    hit = hits.create()
    hit.setPDG(next(counter))
    hit.setEnergy(next(counter))
    hit.setTime(next(counter))
    hit.setStepPosition(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    hit.setParticle(particle)
    return hits


def create_SimCalorimeterHitCollection(calo_contrib):
    """Create a SimCalorimeterHitCollection"""
    counter = count(COUNT_START)
    hits = edm4hep.SimCalorimeterHitCollection()
    hit = hits.create()
    hit.setCellID(next(counter))
    hit.setEnergy(next(counter))
    hit.setPosition(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    hit.addToContributions(calo_contrib)
    return hits


def create_RawCalorimeterHitCollection():
    """Crate a RawCalorimeterHitCollection"""
    counter = count(COUNT_START)
    hits = edm4hep.RawCalorimeterHitCollection()
    hit = hits.create()
    hit.setCellID(next(counter))
    hit.setAmplitude(next(counter))
    hit.setTimeStamp(next(counter))
    return hits


def create_CalorimeterHitCollection():
    """Create a CalorimeterHitCollection"""
    counter = count(COUNT_START)
    hits = edm4hep.CalorimeterHitCollection()
    hit = hits.create()
    hit.setCellID(next(counter))
    hit.setEnergy(next(counter))
    hit.setEnergyError(next(counter))
    hit.setTime(next(counter))
    hit.setPosition(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    hit.setType(next(counter))
    return hits


def create_CovMatrixNf(n_dim):
    """Create a covariance matrix of dimension n_dim"""
    if n_dim not in (2, 3, 4, 6):
        return ValueError(
            f"{n_dim} is not a valid dimension for a CovMatrix in EDM4hep. Valid: (2, 3, 4, 6)"
        )
    counter = count(COUNT_START)
    # With the current version of cppyy (from ROOT 6.30.06)
    # it's not possible to initialize an std::array from a list
    # In future versions (works with 6.32.02) it will be possible
    covNf = getattr(edm4hep, f"CovMatrix{n_dim}f")()
    for i in range(n_dim * (n_dim + 1) // 2):
        covNf[i] = next(counter)
    return covNf


def create_ParticleIDCollection(vectorsize):
    """Create a ParticleIDCollection"""
    counter = count(COUNT_START)
    pids = edm4hep.ParticleIDCollection()
    pid = pids.create()
    pid.setType(next(counter))
    pid.setPDG(next(counter))
    pid.setAlgorithmType(next(counter))
    pid.setLikelihood(next(counter))
    for j in range(vectorsize):
        pid.addToParameters(next(counter))

    return pids, pid


def create_ClusterCollection(vectorsize, calo_hit):
    """Create a ClusterCollection"""
    counter = count(COUNT_START)
    clusters = edm4hep.ClusterCollection()
    cluster = clusters.create()
    cluster.setType(next(counter))
    cluster.setEnergy(next(counter))
    cluster.setEnergyError(next(counter))
    cluster.setPosition(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    cluster.setPositionError(create_CovMatrixNf(3))
    cluster.setITheta(next(counter))
    cluster.setPhi(next(counter))
    cluster.setDirectionError(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    for j in range(vectorsize):
        cluster.addToShapeParameters(next(counter))
        cluster.addToSubdetectorEnergies(next(counter))
    cluster.addToClusters(cluster)
    cluster.addToHits(calo_hit)

    return clusters


def create_TrackerHit3DCollection():
    """Create a TrackerHit3DCollection"""
    counter = count(COUNT_START)
    hits = edm4hep.TrackerHit3DCollection()
    hit = hits.create()
    hit.setCellID(next(counter))
    hit.setType(next(counter))
    hit.setQuality(next(counter))
    hit.setTime(next(counter))
    hit.setEDep(next(counter))
    hit.setEDepError(next(counter))
    hit.setPosition(edm4hep.Vector3d(next(counter), next(counter), next(counter)))
    hit.setCovMatrix(create_CovMatrixNf(3))
    return hits


def create_TrackerHitPlaneCollection():
    """Create a TrackerHitPlaneCollection"""
    counter = count(COUNT_START)
    hits = edm4hep.TrackerHitPlaneCollection()
    hit = hits.create()
    hit.setCellID(next(counter))
    hit.setType(next(counter))
    hit.setQuality(next(counter))
    hit.setTime(next(counter))
    hit.setEDep(next(counter))
    hit.setEDepError(next(counter))
    hit.setU(edm4hep.Vector2f(next(counter), next(counter)))
    hit.setV(edm4hep.Vector2f(next(counter), next(counter)))
    hit.setDu(next(counter))
    hit.setDv(next(counter))
    hit.setPosition(edm4hep.Vector3d(next(counter), next(counter), next(counter)))
    hit.setCovMatrix(create_CovMatrixNf(3))
    return hits


def create_RawTimeSeriesCollection(vectorsize):
    """Create a RawTimeSeriesCollection"""
    counter = count(COUNT_START)
    series = edm4hep.RawTimeSeriesCollection()
    serie = series.create()
    serie.setCellID(next(counter))
    serie.setQuality(next(counter))
    serie.setTime(next(counter))
    serie.setCharge(next(counter))
    serie.setInterval(next(counter))
    for j in range(vectorsize):
        serie.addToAdcCounts(next(counter))
    return series


def create_TrackCollection(vectorsize, tracker_hit):
    """Create a TrackCollection"""
    counter = count(COUNT_START)
    tracks = edm4hep.TrackCollection()
    track = tracks.create()
    track.setType(next(counter))
    track.setChi2(next(counter))
    track.setNdf(next(counter))
    for j in range(vectorsize):
        track.addToSubdetectorHitNumbers(next(counter))
        track.addToSubdetectorHoleNumbers(next(counter))
        state = edm4hep.TrackState()
        state.location = next(counter)
        state.D0 = next(counter)
        state.phi = next(counter)
        state.omega = next(counter)
        state.Z0 = next(counter)
        state.tanLambda = next(counter)
        state.time = next(counter)
        state.referencePoint = edm4hep.Vector3f(next(counter), next(counter), next(counter))
        state.covMatrix = create_CovMatrixNf(6)
        track.addToTrackStates(state)

    track.addToTrackerHits(tracker_hit)
    track.addToTracks(track)
    track.setNholes(next(counter))
    return tracks


def create_VertexCollection(vectorsize):
    """Create a VertexCollection"""
    counter = count(COUNT_START)
    vertex = edm4hep.VertexCollection()
    v = vertex.create()
    v.setType(next(counter))
    v.setChi2(next(counter))
    v.setNdf(next(counter))
    v.setPosition(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    v.setCovMatrix(create_CovMatrixNf(3))
    v.setAlgorithmType(next(counter))
    for j in range(vectorsize):
        v.addToParameters(next(counter))
    return vertex, v


def create_ReconstructedParticleCollection(vertex, cluster, track):
    """Create a ReconstructedParticleCollection"""
    counter = count(COUNT_START)
    rparticles = edm4hep.ReconstructedParticleCollection()
    rparticle = rparticles.create()
    rparticle.setPDG(next(counter))
    rparticle.setEnergy(next(counter))
    rparticle.setMomentum(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    rparticle.setReferencePoint(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    rparticle.setCharge(next(counter))
    rparticle.setMass(next(counter))
    rparticle.setGoodnessOfPID(next(counter))
    rparticle.setCovMatrix(create_CovMatrixNf(4))
    rparticle.setDecayVertex(vertex)
    rparticle.addToClusters(cluster)
    rparticle.addToTracks(track)
    rparticle.addToParticles(rparticle)
    return rparticles


def create_LinkCollection(coll_type, from_el, to_el):
    """Create a LinkCollection of the given type and add one link to it"""
    counter = count(COUNT_START)
    links = coll_type()
    link = links.create()
    link.setWeight(next(counter))
    link.setFrom(from_el)
    link.setTo(to_el)
    return links


def create_TimeSeriesCollection(vectorsize):
    """Create a TimeSeriesCollection"""
    counter = count(COUNT_START)
    timeseries = edm4hep.TimeSeriesCollection()
    serie = timeseries.create()
    serie.setCellID(next(counter))
    serie.setTime(next(counter))
    serie.setInterval(next(counter))
    for j in range(vectorsize):
        serie.addToAmplitude(next(counter))
    return timeseries


def create_RecDqdxCollection(track):
    """Create a RecDqdxCollection"""
    counter = count(COUNT_START)
    recdqdx = edm4hep.RecDqdxCollection()
    dqdx = recdqdx.create()
    q = edm4hep.Quantity()
    q.type = next(counter)
    q.value = next(counter)
    q.error = next(counter)
    dqdx.setDQdx(q)
    dqdx.setTrack(track)
    return recdqdx


def create_GeneratorEventParametersCollection(vectorsize, particle):
    """Create a GeneratorEventParametersCollection"""
    counter = count(COUNT_START)
    gep_coll = edm4hep.GeneratorEventParametersCollection()
    gep = gep_coll.create()
    gep.setEventScale(next(counter))
    gep.setAlphaQED(next(counter))
    gep.setAlphaQCD(next(counter))
    gep.setSignalProcessId(next(counter))
    gep.setSqrts(next(counter))
    for i in range(vectorsize):
        gep.addToCrossSections(next(counter))
        gep.addToCrossSectionErrors(next(counter))
    gep.addToSignalVertex(particle)
    return gep_coll


def create_GeneratorPdfInfoCollection():
    """Create a GeneratorPdfInfoCollection"""
    counter = count(COUNT_START)
    gpi_coll = edm4hep.GeneratorPdfInfoCollection()
    gpi = gpi_coll.create()
    # Doesn't work with ROOT 6.30.06
    # gpi.setPartonId([next(counter), next(counter)])
    gpi.setPartonId(0, next(counter))
    gpi.setPartonId(1, next(counter))
    # Doesn't work with ROOT 6.30.06
    # gpi.setLhapdfId([next(counter), next(counter)])
    gpi.setLhapdfId(0, next(counter))
    gpi.setLhapdfId(1, next(counter))
    # Doesn't work with ROOT 6.30.06
    # gpi.setX([next(counter), next(counter)])
    gpi.setX(0, next(counter))
    gpi.setX(1, next(counter))
    # Doesn't work with ROOT 6.30.06
    # gpi.setXf([next(counter), next(counter)])
    gpi.setXf(0, next(counter))
    gpi.setXf(1, next(counter))
    gpi.setScale(next(counter))
    return gpi_coll


def put_link_collection(frame, link_name, from_el, to_el):
    """Helper function to put a link collection into the frame"""
    coll_name = f"{link_name}Collection"
    coll_type = getattr(edm4hep, coll_name)
    frame.put(create_LinkCollection(coll_type, from_el, to_el), coll_name)


def create_frame():
    """Create a Frame with all types"""
    frame = podio.Frame()

    frame.put(create_EventHeaderCollection(VECTORSIZE), "EventHeader")

    particles = frame.put(create_MCParticleCollection(), "MCParticleCollection")
    particle = particles[0]

    hits = frame.put(create_SimTrackerHitCollection(particle), "SimTrackerHitCollection")
    simtracker_hit = hits[0]

    calo_contribs = frame.put(
        create_CaloHitContributionCollection(particle), "CaloHitContributionCollection"
    )
    calo_contrib = calo_contribs[0]

    hits = frame.put(
        create_SimCalorimeterHitCollection(calo_contrib), "SimCalorimeterHitCollection"
    )
    simcalo_hit = hits[0]

    frame.put(create_RawCalorimeterHitCollection(), "RawCalorimeterHitCollection")

    hits = frame.put(create_CalorimeterHitCollection(), "CalorimeterHitCollection")
    calo_hit = hits[0]

    clusters = frame.put(create_ClusterCollection(VECTORSIZE, calo_hit), "ClusterCollection")
    cluster = clusters[0]

    hits = frame.put(create_TrackerHit3DCollection(), "TrackerHit3DCollection")
    tracker_hit = hits[0]
    frame.put(create_TrackerHitPlaneCollection(), "TrackerHitPlaneCollection")

    frame.put(create_RawTimeSeriesCollection(VECTORSIZE), "RawTimeSeriesCollection")

    tracks = frame.put(create_TrackCollection(VECTORSIZE, tracker_hit), "TrackCollection")
    track = tracks[0]

    pids, pid = create_ParticleIDCollection(VECTORSIZE)

    vertices, vertex = create_VertexCollection(VECTORSIZE)

    reco_particles = frame.put(
        create_ReconstructedParticleCollection(vertex, cluster, track),
        "ReconstructedParticleCollection",
    )
    reco_particle = reco_particles[0]

    vertex.addToParticles(reco_particle)
    frame.put(vertices, "VertexCollection")

    pid.setParticle(reco_particle)
    frame.put(pids, "ParticleIDCollection")

    put_link_collection(frame, "RecoMCParticleLink", reco_particle, particle)
    put_link_collection(frame, "CaloHitSimCaloHitLink", calo_hit, simcalo_hit)
    put_link_collection(frame, "TrackerHitSimTrackerHitLink", tracker_hit, simtracker_hit)
    put_link_collection(frame, "CaloHitMCParticleLink", calo_hit, particle)
    put_link_collection(frame, "ClusterMCParticleLink", cluster, particle)
    put_link_collection(frame, "TrackMCParticleLink", track, particle)
    put_link_collection(frame, "VertexRecoParticleLink", vertex, reco_particle)

    frame.put(create_TimeSeriesCollection(VECTORSIZE), "TimeSeriesCollection")
    frame.put(create_RecDqdxCollection(track), "RecDqdxCollection")

    frame.put(
        create_GeneratorEventParametersCollection(VECTORSIZE, particle),
        "GeneratorEventParametersCollection",
    )
    frame.put(create_GeneratorPdfInfoCollection(), "GeneratorPdfInfoCollection")

    return frame


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Create a file with EDM4hep data")
    parser.add_argument(
        "--rntuple", action="store_true", help="Use a ROOT ntuple instead of EDM4hep"
    )
    parser.add_argument("--output-file", type=str, help="Output file name", default="edm4hep.root")
    args = parser.parse_args()
    output_file = args.output_file

    if args.rntuple:
        try:
            writer = podio.root_io.RNTupleWriter(output_file)
        except AttributeError:
            print("The RNTuple writer from podio is not available but was requested")
            sys.exit(1)
    else:
        writer = podio.root_io.Writer(output_file)

    for i in range(FRAMES):
        frame = create_frame()
        print(f"Writing frame {i}")

        writer.write_frame(frame, "events")
