#ifndef EDM4HEP_CLUSTERMCPARTICLELINKCOLLECTION_H
#define EDM4HEP_CLUSTERMCPARTICLELINKCOLLECTION_H

#include <podio/LinkCollection.h>

#include <edm4hep/ClusterCollection.h>
#include <edm4hep/MCParticleCollection.h>

namespace edm4hep {
using ClusterMCParticleLinkCollection = podio::LinkCollection<edm4hep::Cluster, edm4hep::MCParticle>;
} // namespace edm4hep

#endif // EDM4HEP_CLUSTERMCPARTICLELINKCOLLECTION_H
