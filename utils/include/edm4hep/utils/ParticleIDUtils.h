#ifndef EDM4HEP_UTILS_PARTICLEIDUTILS_H
#define EDM4HEP_UTILS_PARTICLEIDUTILS_H

#include <edm4hep/ParticleIDCollection.h>
#include <edm4hep/ReconstructedParticle.h>

#include <podio/Frame.h>

#include <map>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

namespace edm4hep::utils {

/// A simple struct bundling relevant metadata for a ParticleID collection
struct ParticleIDMeta {
  std::string algoName{};                ///< The name of the algorithm
  int32_t algoType{0};                   ///< The (user defined) algorithm type
  std::vector<std::string> paramNames{}; ///< The names of the parameters
};

/// Utility class to invert the ParticleID to ReconstructedParticle relation
class PIDHandler {

  using RecoPidMapT = std::multimap<edm4hep::ReconstructedParticle, edm4hep::ParticleID>;

  RecoPidMapT m_recoPidMap{}; ///< The internal map from recos to pids

  std::map<std::string, int> m_algoTypes{}; ///< Maps algo names to algo types

  /// Maps algo types to the parameter names for each algorithm
  std::map<int, std::vector<std::string>> m_algoParamNames{};

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

  /// Create a PIDHandler from a Frame potentially also populating some metadata information.
  static PIDHandler from(const podio::Frame& event, const podio::Frame& metadata = {});

  /// Add the information from one ParticleIDCollection to the handler
  void addColl(const edm4hep::ParticleIDCollection& coll);

  /// Retrieve all ParticleIDs that are related to the passed
  /// ReconstructedParticle
  std::vector<edm4hep::ParticleID> getPIDs(const edm4hep::ReconstructedParticle& reco) const;

  /// Retrieve the ParticleID for a given algorithm type
  std::optional<edm4hep::ParticleID> getPID(const edm4hep::ReconstructedParticle& reco, int algoType) const;

  /// Retrieve the index in the parameters for a given parameter name and
  /// algoType
  std::optional<int> getParamIndex(int32_t algoType, const std::string& paramName) const;

  /// Retrieve the algoType for a given algorithm name
  std::optional<int32_t> getAlgoType(const std::string& algoName) const;

  static void setAlgoInfo(podio::Frame& metadata, edm4hep::ParticleIDCollection& pidcoll, const std::string& collname,
                          const edm4hep::utils::ParticleIDMeta& pidMetaInfo);

  static void setAlgoInfo(podio::Frame& metadata, const std::string& collname,
                          const edm4hep::utils::ParticleIDMeta& pidMetaInfo);

  static std::optional<edm4hep::utils::ParticleIDMeta> getAlgoInfo(const podio::Frame& metadata,
                                                                   const std::string& collName);
};
} // namespace edm4hep::utils

#endif // EDM4HEP_UTILS_PARTICLEIDUTILS_H
