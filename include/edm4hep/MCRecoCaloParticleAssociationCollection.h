#ifndef EDM4HEP_MCRecoCaloParticleAssociationCollection_H
#define EDM4HEP_MCRecoCaloParticleAssociationCollection_H

#include "edm4hep/CaloHitMCParticleLinkCollection.h"

namespace edm4hep {
using MCRecoCaloParticleAssociationCollection [[deprecated("use CaloHitMCParticleLinkCollection instead")]] =
    edm4hep::CaloHitMCParticleLinkCollection;
}

#endif
