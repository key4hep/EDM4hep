#ifndef EDM4HEP_TRACKMCPARTICLELINKCOLLECTION_H
#define EDM4HEP_TRACKMCPARTICLELINKCOLLECTION_H

#include <podio/LinkCollection.h>

#include <edm4hep/MCParticleCollection.h>
#include <edm4hep/TrackCollection.h>

namespace edm4hep {
using TrackMCParticleLinkCollection = podio::LinkCollection<edm4hep::Track, edm4hep::MCParticle>;
} // namespace edm4hep

#endif // EDM4HEP_CLUSTERMCPARTICLELINKCOLLECTION_H
