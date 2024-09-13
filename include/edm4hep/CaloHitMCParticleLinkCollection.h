#ifndef EDM4HEP_CALOHITMCPARTICLELINKCOLLECTION_H
#define EDM4HEP_CALOHITMCPARTICLELINKCOLLECTION_H

#include <podio/LinkCollection.h>

#include <edm4hep/CalorimeterHitCollection.h>
#include <edm4hep/MCParticleCollection.h>

namespace edm4hep {
using CaloHitMCParticleLinkCollection = podio::LinkCollection<edm4hep::CalorimeterHit, edm4hep::MCParticle>;
} // namespace edm4hep

#endif // EDM4HEP_CALOHITMCPARTICLELINKCOLLECTION_H
