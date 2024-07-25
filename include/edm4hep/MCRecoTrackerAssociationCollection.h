#ifndef EDM4HEP_MCRecoTrackerAssociationCollection_H
#define EDM4HEP_MCRecoTrackerAssociationCollection_H

#include "edm4hep/TrackerHitSimTrackerHitLinkCollection.h"

namespace edm4hep {
using MCRecoTrackerAssociationCollection [[deprecated("use TrackerHitSimTrackerHitLinkCollection instead")]] =
    edm4hep::TrackerHitSimTrackerHitLinkCollection;
}

#endif
