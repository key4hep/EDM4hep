#ifndef EDM4HEP_MCRecoCaloAssociationCollection_H
#define EDM4HEP_MCRecoCaloAssociationCollection_H

#include "edm4hep/CaloHitSimCaloHitLinkCollection.h"

namespace edm4hep {
using MCRecoCaloAssociationCollection [[deprecated("use CaloHitSimCaloHitLinkCollection instead")]] =
    edm4hep::CaloHitSimCaloHitLinkCollection;
}

#endif
