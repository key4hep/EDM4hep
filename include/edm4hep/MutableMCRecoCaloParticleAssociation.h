#ifndef EDM4HEP_MutableMCRecoCaloParticleAssociation_H
#define EDM4HEP_MutableMCRecoCaloParticleAssociation_H

#include "edm4hep/CaloHitMCParticleLink.h"

namespace edm4hep {
using MutableMCRecoCaloParticleAssociation [[deprecated("use MutableCaloHitMCParticleLink instead")]] =
    edm4hep::MutableCaloHitMCParticleLink;
}

#endif
