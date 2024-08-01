#ifndef EDM4HEP_MCRecoCaloAssociation_H
#define EDM4HEP_MCRecoCaloAssociation_H

#include "edm4hep/CaloHitSimCaloHitLink.h"

namespace edm4hep {
using MCRecoCaloAssociation [[deprecated("use CaloHitSimCaloHitLink instead")]] = edm4hep::CaloHitSimCaloHitLink;
}

#endif
