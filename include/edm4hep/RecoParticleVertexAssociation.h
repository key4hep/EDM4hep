#ifndef EDM4HEP_RecoParticleVertexAssociation_H
#define EDM4HEP_RecoParticleVertexAssociation_H

#include "edm4hep/VertexRecoParticleLink.h"

namespace edm4hep {
using RecoParticleVertexAssociation [[deprecated("use VertexRecoParticleLink instead")]] =
    edm4hep::VertexRecoParticleLink;
}

#endif
