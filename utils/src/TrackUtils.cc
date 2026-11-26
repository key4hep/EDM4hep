#include "edm4hep/utils/TrackUtils.h"
#include "edm4hep/RecDqdxCollection.h"
#include "edm4hep/Track.h"

#include <ranges>

namespace edm4hep::utils {

void TrackPIDHandler::addColl(const edm4hep::RecDqdxCollection& coll) {
  for (const auto dqdx : coll) {
    m_trackDqMap.emplace(dqdx.getTrack(), dqdx);
  }
}

std::vector<edm4hep::RecDqdx> TrackPIDHandler::getDqdxValues(const edm4hep::Track& track) const {
  const auto [begin, end] = m_trackDqMap.equal_range(track);
  return std::ranges::subrange(begin, end) | std::views::values | std::ranges::to<std::vector>();
}

} // namespace edm4hep::utils
