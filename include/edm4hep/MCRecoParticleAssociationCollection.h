#ifndef EDM4HEP_MCRecoParticleAssociationCollection_H
#define EDM4HEP_MCRecoParticleAssociationCollection_H

#include "edm4hep/RecoMCParticleLinkCollection.h"

namespace edm4hep {
using MCRecoParticleAssociationCollection [[deprecated("use RecoMCParticleLinkCollection instead")]] =
    edm4hep::RecoMCParticleLinkCollection;
}

#endif
