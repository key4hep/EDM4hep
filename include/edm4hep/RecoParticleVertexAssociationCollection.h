#ifndef EDM4HEP_RecoParticleVertexAssociationCollection_H
#define EDM4HEP_RecoParticleVertexAssociationCollection_H

#include "edm4hep/VertexRecoParticleLinkCollection.h"

namespace edm4hep {
using RecoParticleVertexAssociationCollection [[deprecated("use VertexRecoParticleLinkCollection instead")]] =
    edm4hep::VertexRecoParticleLinkCollection;
}

#endif
