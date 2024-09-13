#ifndef EDM4HEP_TRACKERHITSIMTRACKERHITLINKCOLLECTION_H
#define EDM4HEP_TRACKERHITSIMTRACKERHITLINKCOLLECTION_H

#include <podio/LinkCollection.h>

#include <edm4hep/SimTrackerHitCollection.h>
#include <edm4hep/TrackerHit.h>

namespace edm4hep {
using TrackerHitSimTrackerHitLinkCollection = podio::LinkCollection<edm4hep::TrackerHit, edm4hep::SimTrackerHit>;
} // namespace edm4hep

#endif // EDM4HEP_TRACKERHITSIMTRACKERHITLINKCOLLECTION_H
