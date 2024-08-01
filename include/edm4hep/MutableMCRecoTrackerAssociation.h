#ifndef EDM4HEP_MutableMCRecoTrackerAssociation_H
#define EDM4HEP_MutableMCRecoTrackerAssociation_H

#include "edm4hep/TrackerHitSimTrackerHitLink.h"

namespace edm4hep {
using MutableMCRecoTrackerAssociation [[deprecated("use MutableTrackerHitSimTrackerHitLink instead")]] =
    edm4hep::MutableTrackerHitSimTrackerHitLink;
}

#endif
