#ifndef EDM4HEP_MutableMCRecoCaloAssociation_H
#define EDM4HEP_MutableMCRecoCaloAssociation_H

#include "edm4hep/CaloHitSimCaloHitLink.h"

namespace edm4hep {
using MutableMCRecoCaloAssociation [[deprecated("use MutableCaloHitSimCaloHitLink instead")]] =
    edm4hep::MutableCaloHitSimCaloHitLink;
}

#endif
