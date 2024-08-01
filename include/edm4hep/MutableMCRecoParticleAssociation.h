#ifndef EDM4HEP_MutableMCRecoParticleAssociation_H
#define EDM4HEP_MutableMCRecoParticleAssociation_H

#include "edm4hep/RecoMCParticleLink.h"

namespace edm4hep {
using MutableMCRecoParticleAssociation [[deprecated("use MutableRecoMCParticleLink instead")]] =
    edm4hep::MutableRecoMCParticleLink;
}

#endif
