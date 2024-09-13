#ifndef EDM4HEP_RECOMCPARTICLELINKCOLLECTION_H
#define EDM4HEP_RECOMCPARTICLELINKCOLLECTION_H

#include <podio/LinkCollection.h>

#include <edm4hep/MCParticleCollection.h>
#include <edm4hep/ReconstructedParticleCollection.h>

namespace edm4hep {
using RecoMCParticleLinkCollection = podio::LinkCollection<edm4hep::ReconstructedParticle, edm4hep::MCParticle>;
} // namespace edm4hep

#endif // EDM4HEP_RECOMCPARTICLELINKCOLLECTION_H
