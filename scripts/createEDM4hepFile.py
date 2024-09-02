# Description: Create a file with EDM4hep data using the EDM4hep python bindings
# The purpose of the script is to use all the classes of the EDM4hep library
# to create a file with dummy data.
import podio
import edm4hep
from itertools import count
import argparse
import sys

frames = 3  # How many frames or events will be written
vectorsize = 5  # For vector members, each vector member will have this size
counter = count()  # next(counter) will return 0, 1, 2, ...

parser = argparse.ArgumentParser(description="Create a file with EDM4hep data")
parser.add_argument(
    "--rntuple", action="store_true", help="Use a ROOT ntuple instead of EDM4hep"
)
parser.add_argument(
    "--output-file", type=str, help="Output file name", default="edm4hep.root"
)
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

for i in range(frames):
    print(f"Writing frame {i}")
    frame = podio.Frame()

    # Make covariance matrices needed later

    # With the current version of cppyy (from ROOT 6.30.06)
    # it's not possible to initialize an std::array from a list
    # In future versions (works with 6.32.02) it will be possible
    cov3f = edm4hep.CovMatrix3f()
    for i in range(6):
        cov3f[i] = next(counter)
    cov4f = edm4hep.CovMatrix4f()
    for i in range(10):
        cov4f[i] = next(counter)
    cov6f = edm4hep.CovMatrix6f()
    for i in range(21):
        cov6f[i] = next(counter)

    header = edm4hep.EventHeaderCollection()
    h = header.create()
    h.setEventNumber(next(counter))
    h.setRunNumber(next(counter))
    h.setTimeStamp(next(counter))
    h.setWeight(1.0)
    for j in range(vectorsize):
        h.addToWeights(next(counter))
    frame.put(header, "EventHeader")

    particles = edm4hep.MCParticleCollection()
    for i in range(3):
        particle = particles.create()
        particle.setPDG(next(counter))
        particle.setGeneratorStatus(next(counter))
        particle.setSimulatorStatus(next(counter))
        particle.setCharge(next(counter))
        particle.setTime(next(counter))
        particle.setMass(next(counter))
        particle.setVertex(
            edm4hep.Vector3d(next(counter), next(counter), next(counter))
        )
        particle.setEndpoint(
            edm4hep.Vector3d(next(counter), next(counter), next(counter))
        )
        particle.setMomentum(
            edm4hep.Vector3d(next(counter), next(counter), next(counter))
        )
        particle.setMomentumAtEndpoint(
            edm4hep.Vector3d(next(counter), next(counter), next(counter))
        )
        particle.setSpin(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
        particle.setColorFlow(edm4hep.Vector2i(next(counter), next(counter)))

    particles[0].addToDaughters(particles[1])
    particles[0].addToParents(particles[2])
    particle = particles[0]
    frame.put(particles, "MCParticleCollection")

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
    simtracker_hit = hit
    frame.put(hits, "SimTrackerHitCollection")

    hits = edm4hep.CaloHitContributionCollection()
    hit = hits.create()
    hit.setPDG(next(counter))
    hit.setEnergy(next(counter))
    hit.setTime(next(counter))
    hit.setStepPosition(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    hit.setParticle(particle)
    calohit = hit
    frame.put(hits, "CaloHitContributionCollection")

    hits = edm4hep.SimCalorimeterHitCollection()
    hit = hits.create()
    hit.setCellID(next(counter))
    hit.setEnergy(next(counter))
    hit.setPosition(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    hit.addToContributions(calohit)
    simcalo_hit = hit
    frame.put(hits, "SimCalorimeterHitCollection")

    hits = edm4hep.RawCalorimeterHitCollection()
    hit = hits.create()
    hit.setCellID(next(counter))
    hit.setAmplitude(next(counter))
    hit.setTimeStamp(next(counter))
    frame.put(hits, "RawCalorimeterHitCollection")

    hits = edm4hep.CalorimeterHitCollection()
    hit = hits.create()
    hit.setCellID(next(counter))
    hit.setEnergy(next(counter))
    hit.setEnergyError(next(counter))
    hit.setTime(next(counter))
    hit.setPosition(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    hit.setType(next(counter))
    calo_hit = hit
    frame.put(hits, "CalorimeterHitCollection")

    pids = edm4hep.ParticleIDCollection()
    pid = pids.create()
    pid.setType(next(counter))
    pid.setPDG(next(counter))
    pid.setAlgorithmType(next(counter))
    pid.setLikelihood(next(counter))
    for j in range(vectorsize):
        pid.addToParameters(next(counter))
    frame.put(pids, "ParticleIDCollection")

    clusters = edm4hep.ClusterCollection()
    cluster = clusters.create()
    cluster.setType(next(counter))
    cluster.setEnergy(next(counter))
    cluster.setEnergyError(next(counter))
    cluster.setPosition(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    cluster.setPositionError(cov3f)
    cluster.setITheta(next(counter))
    cluster.setPhi(next(counter))
    cluster.setDirectionError(
        edm4hep.Vector3f(next(counter), next(counter), next(counter))
    )
    for j in range(vectorsize):
        cluster.addToShapeParameters(next(counter))
        cluster.addToSubdetectorEnergies(next(counter))
    cluster.addToClusters(cluster)
    cluster.addToHits(calo_hit)
    frame.put(clusters, "ClusterCollection")

    hits = edm4hep.TrackerHit3DCollection()
    hit = hits.create()
    hit.setCellID(next(counter))
    hit.setType(next(counter))
    hit.setQuality(next(counter))
    hit.setTime(next(counter))
    hit.setEDep(next(counter))
    hit.setEDepError(next(counter))
    hit.setPosition(edm4hep.Vector3d(next(counter), next(counter), next(counter)))
    hit.setCovMatrix(cov3f)
    tracker_hit = hit
    frame.put(hits, "TrackerHit3DCollection")

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
    hit.setCovMatrix(cov3f)
    frame.put(hits, "TrackerHitPlaneCollection")

    series = edm4hep.RawTimeSeriesCollection()
    serie = series.create()
    serie.setCellID(next(counter))
    serie.setQuality(next(counter))
    serie.setTime(next(counter))
    serie.setCharge(next(counter))
    serie.setInterval(next(counter))
    for j in range(vectorsize):
        serie.addToAdcCounts(next(counter))
    frame.put(series, "RawTimeSeriesCollection")

    tracks = edm4hep.TrackCollection()
    track = tracks.create()
    track.setType(next(counter))
    track.setChi2(next(counter))
    track.setNdf(next(counter))
    for j in range(vectorsize):
        track.addToSubdetectorHitNumbers(next(counter))
        state = edm4hep.TrackState()
        state.location = next(counter)
        state.d0 = next(counter)
        state.phi = next(counter)
        state.omega = next(counter)
        state.z0 = next(counter)
        state.tanLambda = next(counter)
        state.time = next(counter)
        state.referencePoint = edm4hep.Vector3f(
            next(counter), next(counter), next(counter)
        )
        state.CovMatrix = cov6f
        track.addToTrackStates(state)
    track.addToTrackerHits(tracker_hit)
    track.addToTracks(track)
    track.setNholes(next(counter))
    frame.put(tracks, "TrackCollection")

    vertex = edm4hep.VertexCollection()
    v = vertex.create()
    v.setType(next(counter))
    v.setChi2(next(counter))
    v.setNdf(next(counter))
    v.setPosition(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    v.setCovMatrix(cov3f)
    v.setAlgorithmType(next(counter))
    for j in range(vectorsize):
        v.addToParameters(next(counter))
    frame.put(vertex, "VertexCollection")

    rparticles = edm4hep.ReconstructedParticleCollection()
    rparticle = rparticles.create()
    rparticle.setPDG(next(counter))
    rparticle.setEnergy(next(counter))
    rparticle.setMomentum(edm4hep.Vector3f(next(counter), next(counter), next(counter)))
    rparticle.setReferencePoint(
        edm4hep.Vector3f(next(counter), next(counter), next(counter))
    )
    rparticle.setCharge(next(counter))
    rparticle.setMass(next(counter))
    rparticle.setGoodnessOfPID(next(counter))
    rparticle.setCovMatrix(cov4f)
    rparticle.setDecayVertex(v)
    rparticle.addToClusters(cluster)
    rparticle.addToTracks(track)
    rparticle.addToParticles(rparticle)
    reco_particle = rparticle
    frame.put(rparticles, "ReconstructedParticleCollection")

    v.addToParticles(reco_particle)
    pid.setParticle(reco_particle)

    # TODO: Add when the PID PR is merged
    # if not args.use_pre1:
    #     pid.setParticle(reco_particle)

    links = edm4hep.RecoMCParticleLinkCollection()
    link = links.create()
    link.setWeight(next(counter))
    link.setFrom(reco_particle)
    link.setTo(particle)
    frame.put(links, "RecoMCParticleLinkCollection")

    links = edm4hep.CaloHitSimCaloHitLinkCollection()
    link = links.create()
    link.setWeight(next(counter))
    link.setFrom(calo_hit)
    link.setTo(simcalo_hit)
    frame.put(links, "CaloHitSimCaloHitLinkCollection")

    links = edm4hep.TrackerHitSimTrackerHitLinkCollection()
    link = links.create()
    link.setWeight(next(counter))
    link.setFrom(tracker_hit)
    link.setTo(simtracker_hit)
    frame.put(links, "TrackerHitSimTrackerHitLinkCollection")

    links = edm4hep.CaloHitMCParticleLinkCollection()
    link = links.create()
    link.setWeight(next(counter))
    link.setFrom(calo_hit)
    link.setTo(particle)
    frame.put(links, "CaloHitMCParticleLinkCollection")

    links = edm4hep.ClusterMCParticleLinkCollection()
    link = links.create()
    link.setWeight(next(counter))
    link.setFrom(cluster)
    link.setTo(particle)
    frame.put(links, "ClusterMCParticleLinkCollection")

    links = edm4hep.TrackMCParticleLinkCollection()
    link = links.create()
    link.setWeight(next(counter))
    link.setFrom(track)
    link.setTo(particle)
    frame.put(links, "TrackMCParticleLinkCollection")

    links = edm4hep.VertexRecoParticleLinkCollection()
    link = links.create()
    link.setWeight(next(counter))
    link.setTo(reco_particle)
    link.setFrom(v)
    frame.put(links, "MCVertexRecoParticleLinkCollection")

    timeseries = edm4hep.TimeSeriesCollection()
    serie = timeseries.create()
    serie.setCellID(next(counter))
    serie.setTime(next(counter))
    serie.setInterval(next(counter))
    for j in range(vectorsize):
        serie.addToAmplitude(next(counter))
    frame.put(timeseries, "TimeSeriesCollection")

    recdqdx = edm4hep.RecDqdxCollection()
    dqdx = recdqdx.create()
    q = edm4hep.Quantity()
    q.type = next(counter)
    q.value = next(counter)
    q.error = next(counter)
    dqdx.setDQdx(q)
    dqdx.setTrack(track)
    frame.put(recdqdx, "RecDqdxCollection")

    gep_coll = edm4hep.GeneratorEventParametersCollection()
    gep = gep_coll.create()
    gep.setEventScale(next(counter))
    gep.setAlphaQED(next(counter))
    gep.setAlphaQCD(next(counter))
    gep.setSignalProcessId(next(counter))
    gep.setSqrts(next(counter))
    frame.put(gep_coll, "GeneratorEventParametersCollection")

    for i in range(vectorsize):
        gep.addToCrossSections(next(counter))
        gep.addToCrossSectionErrors(next(counter))
    gep.addToSignalVertex(particle)

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
    frame.put(gpi_coll, "GeneratorPdfInfoCollection")

    writer.write_frame(frame, "events")
