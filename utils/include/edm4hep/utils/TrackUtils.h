#ifndef EDM4HEP_UTILS_TRACKUTILS_H
#define EDM4HEP_UTILS_TRACKUTILS_H

#include <edm4hep/RecDqdxCollection.h>
#include <edm4hep/Track.h>

#include <podio/Frame.h>

#include <map>
#include <vector>

namespace edm4hep::utils {
/// Utility class to invert the relations between RecDqdx to Track relation
class TrackPIDHandler {
  using TrackMapT = std::multimap<edm4hep::Track, edm4hep::RecDqdx>;

  TrackMapT m_trackDqMap{}; ///< The internal map from tracks to RecDqdx
public:
  /// Add the information from the passed collection to the handler
  void addColl(const edm4hep::RecDqdxCollection& coll);

  /// Get all RecDqdx objects for the given track
  std::vector<edm4hep::RecDqdx> getDqdxValues(const edm4hep::Track& track) const;
};
} // namespace edm4hep::utils

#endif // EDM4HEP_UTILS_TRACKUTILS_H
