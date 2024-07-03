#include "edm4hep/utils/TrackUtils.h"
#include "edm4hep/RecDqdxCollection.h"

#include <iterator>

namespace edm4hep::utils {

void TrackPIDHandler::addColl(const edm4hep::RecDqdxCollection& coll) {
  for (const auto dqdx : coll) {
    m_trackDqMap.emplace(dqdx.getTrack(), dqdx);
  }
}

std::vector<edm4hep::RecDqdx> TrackPIDHandler::getDqdxValues(const edm4hep::Track& track) const {
  const auto& [begin, end] = m_trackDqMap.equal_range(track);
  std::vector<edm4hep::RecDqdx> dqdxs{};
  dqdxs.reserve(std::distance(begin, end));

  for (auto it = begin; it != end; ++it) {
    dqdxs.emplace_back(it->second);
  }

  return dqdxs;
}

} // namespace edm4hep::utils
