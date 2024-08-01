#ifndef EDM4HEP_MutableMCRecoClusterParticleAssociation_H
#define EDM4HEP_MutableMCRecoClusterParticleAssociation_H

#include "edm4hep/ClusterMCParticleLink.h"

namespace edm4hep {
using MutableMCRecoClusterParticleAssociation [[deprecated("use MutableClusterMCParticleLink instead")]] =
    edm4hep::MutableClusterMCParticleLink;
}

#endif
