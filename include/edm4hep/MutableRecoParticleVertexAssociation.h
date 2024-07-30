#ifndef EDM4HEP_MutableRecoParticleVertexAssociation_H
#define EDM4HEP_MutableRecoParticleVertexAssociation_H

#include "edm4hep/VertexRecoParticleLink.h"

namespace edm4hep {
using MutableRecoParticleVertexAssociation [[deprecated("use MutableVertexRecoParticleLink instead")]] =
    edm4hep::MutableVertexRecoParticleLink;
}

#endif
