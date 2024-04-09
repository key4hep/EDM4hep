#ifndef EDM4HEP_UTILS_PARTICLEIDUTILS_H
#define EDM4HEP_UTILS_PARTICLEIDUTILS_H

#include <edm4hep/ParticleIDCollection.h>
#include <edm4hep/ReconstructedParticle.h>

#include <map>
#include <type_traits>
#include <vector>

namespace edm4hep::utils {

/// Utility class to invert the ParticleID to ReconstructedParticle relation
class PIDHandler {

  using MapType = std::multimap<edm4hep::ReconstructedParticle, edm4hep::ParticleID>;

  MapType m_recoPidMap{}; ///< The internal map from recos to pids

public:
  PIDHandler() = default;
  ~PIDHandler() = default;
  PIDHandler(const PIDHandler&) = default;
  PIDHandler& operator=(const PIDHandler&) = default;
  PIDHandler(PIDHandler&&) = default;
  PIDHandler& operator=(PIDHandler&&) = default;

  /// Construct a PIDHandler from an arbitrary number of ParticleIDCollections
  template <typename... PIDColls>
  static PIDHandler from(const ParticleIDCollection& coll, const PIDColls&... pidColls) {
    static_assert((std::is_same_v<PIDColls, edm4hep::ParticleIDCollection> && ...),
                  "PIDHandler can only be constructed from ParticleIDCollections");
    PIDHandler handler{};
    handler.addColl(coll);
    (handler.addColl(pidColls), ...);
    return handler;
  }

  /// Add the information from one ParticleIDCollection to the handler
  void addColl(const edm4hep::ParticleIDCollection& coll);

  /// Retrieve all ParticleIDs that are related to the passed
  /// ReconstructedParticle
  std::vector<edm4hep::ParticleID> getPIDs(const edm4hep::ReconstructedParticle& reco) const;
};
} // namespace edm4hep::utils

#endif // EDM4HEP_UTILS_PARTICLEIDUTILS_H
