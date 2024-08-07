#ifndef EDM4HEP_MCRecoClusterParticleAssociationCollection_H
#define EDM4HEP_MCRecoClusterParticleAssociationCollection_H

#include "edm4hep/ClusterMCParticleLinkCollection.h"
#include "edm4hep/MCRecoClusterParticleAssociation.h"
#include "edm4hep/MutableMCRecoClusterParticleAssociation.h"

namespace edm4hep {
using MCRecoClusterParticleAssociationCollection [[deprecated("use ClusterMCParticleLinkCollection instead")]] =
    edm4hep::ClusterMCParticleLinkCollection;
}

#endif
