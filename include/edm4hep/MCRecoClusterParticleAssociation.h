#ifndef EDM4HEP_MCRecoClusterParticleAssociation_H
#define EDM4HEP_MCRecoClusterParticleAssociation_H

#include "edm4hep/ClusterMCParticleLink.h"

namespace edm4hep {
using MCRecoClusterParticleAssociation [[deprecated("use ClusterMCParticleLink instead")]] =
    edm4hep::ClusterMCParticleLink;
}

#endif
