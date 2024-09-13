#include "edm4hep/CaloHitMCParticleLinkCollection.h"
#include "edm4hep/CaloHitSimCaloHitLinkCollection.h"
#include "edm4hep/ClusterMCParticleLinkCollection.h"
#include "edm4hep/RecoMCParticleLinkCollection.h"
#include "edm4hep/TrackMCParticleLinkCollection.h"
#include "edm4hep/TrackerHitSimTrackerHitLinkCollection.h"
#include "edm4hep/VertexRecoParticleLinkCollection.h"

PODIO_DECLARE_LINK(edm4hep::ReconstructedParticle, edm4hep::MCParticle)
PODIO_DECLARE_LINK(edm4hep::TrackerHit, edm4hep::SimTrackerHit)
PODIO_DECLARE_LINK(edm4hep::Track, edm4hep::MCParticle)
PODIO_DECLARE_LINK(edm4hep::Cluster, edm4hep::MCParticle)
PODIO_DECLARE_LINK(edm4hep::CalorimeterHit, edm4hep::SimCalorimeterHit)
PODIO_DECLARE_LINK(edm4hep::CalorimeterHit, edm4hep::MCParticle)
PODIO_DECLARE_LINK(edm4hep::Vertex, edm4hep::ReconstructedParticle)
