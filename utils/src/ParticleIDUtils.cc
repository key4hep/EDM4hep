#include "edm4hep/ReconstructedParticle.h"
#include <edm4hep/utils/ParticleIDUtils.h>

#include <iterator>

namespace edm4hep::utils {

void PIDHandler::addColl(const edm4hep::ParticleIDCollection& coll) {
  for (const auto pid : coll) {
    m_recoPidMap.emplace(pid.getParticle(), pid);
  }
}

std::vector<edm4hep::ParticleID> PIDHandler::getPIDs(const edm4hep::ReconstructedParticle& reco) const {
  std::vector<edm4hep::ParticleID> pids;
  const auto& [begin, end] = m_recoPidMap.equal_range(reco);
  pids.reserve(std::distance(begin, end));

  for (auto it = begin; it != end; ++it) {
    pids.emplace_back(it->second);
  }

  return pids;
}

} // namespace edm4hep::utils
