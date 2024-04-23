#include <edm4hep/utils/ParticleIDUtils.h>

#include "edm4hep/Constants.h"

#include <podio/FrameCategories.h>

#include <cassert>
#include <iterator>
#include <stdexcept>
#include <string>

namespace edm4hep::utils {

std::optional<int> getParamIndex(const ParticleIDMeta& pidMetaInfo, const std::string& param) {
  const auto nameIt = std::find(pidMetaInfo.paramNames.begin(), pidMetaInfo.paramNames.end(), param);
  if (nameIt != pidMetaInfo.paramNames.end()) {
    return std::distance(pidMetaInfo.paramNames.begin(), nameIt);
  }
  return std::nullopt;
}

void PIDHandler::addColl(const edm4hep::ParticleIDCollection& coll) {
  for (const auto pid : coll) {
    m_recoPidMap.emplace(pid.getParticle(), pid);
  }
}

void PIDHandler::addColl(const edm4hep::ParticleIDCollection& coll, const edm4hep::utils::ParticleIDMeta& pidInfo) {
  addColl(coll);
  addMetaInfo(pidInfo);
}

void PIDHandler::addMetaInfo(const edm4hep::utils::ParticleIDMeta& pidInfo) {
  const auto [algoIt, inserted] = m_algoTypes.emplace(pidInfo.algoName, pidInfo.algoType);
  if (!inserted) {
    throw std::runtime_error("Cannot have duplicate algorithm names (" + pidInfo.algoName + " already exists)");
  }

  const auto [__, metaInserted] = m_algoPidMeta.emplace(pidInfo.algoType, pidInfo);
  if (!metaInserted) {
    if (inserted) {
      m_algoTypes.erase(algoIt);
    }
    throw std::runtime_error("Cannot have duplicate algorithm types (" + std::to_string(pidInfo.algoType) +
                             " already exists)");
  }
}

PIDHandler PIDHandler::from(const podio::Frame& event, const podio::Frame& metadata) {
  PIDHandler handler{};
  for (const auto& name : event.getAvailableCollections()) {
    const auto* coll = event.get(name);
    if (const auto pidColl = dynamic_cast<const edm4hep::ParticleIDCollection*>(coll)) {
      handler.addColl(*pidColl);

      auto maybeMetaInfo = PIDHandler::getAlgoInfo(metadata, name);
      if (maybeMetaInfo) {
        handler.addMetaInfo(std::move(maybeMetaInfo.value()));
      }
    }
  }

  return handler;
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

std::optional<edm4hep::ParticleID> PIDHandler::getPID(const edm4hep::ReconstructedParticle& reco,
                                                      int32_t algoType) const {
  const auto& [begin, end] = m_recoPidMap.equal_range(reco);
  for (auto it = begin; it != end; ++it) {
    if (it->second.getAlgorithmType() == algoType) {
      return it->second;
    }
  }

  return std::nullopt;
}

std::optional<int> PIDHandler::getParamIndex(int32_t algoType, const std::string& paramName) const {
  if (const auto it = m_algoPidMeta.find(algoType); it != m_algoPidMeta.end()) {
    return edm4hep::utils::getParamIndex(it->second, paramName);
  }

  return std::nullopt;
}

std::optional<int32_t> PIDHandler::getAlgoType(const std::string& algoName) const {
  if (const auto it = m_algoTypes.find(algoName); it != m_algoTypes.end()) {
    return it->second;
  }

  return std::nullopt;
}

void PIDHandler::setAlgoInfo(podio::Frame& metadata, edm4hep::ParticleIDCollection& pidColl,
                             const std::string& collName, const edm4hep::utils::ParticleIDMeta& pidMetaInfo) {
  for (auto pid : pidColl) {
    pid.setAlgorithmType(pidMetaInfo.algoType);
  }

  PIDHandler::setAlgoInfo(metadata, collName, pidMetaInfo);
}

void PIDHandler::setAlgoInfo(podio::Frame& metadata, const std::string& collName,
                             const edm4hep::utils::ParticleIDMeta& pidMetaInfo) {
  metadata.putParameter(podio::collMetadataParamName(collName, edm4hep::pidAlgoName), pidMetaInfo.algoName);
  metadata.putParameter(podio::collMetadataParamName(collName, edm4hep::pidAlgoType), pidMetaInfo.algoType);
  metadata.putParameter(podio::collMetadataParamName(collName, edm4hep::pidParameterNames), pidMetaInfo.paramNames);
}

std::optional<edm4hep::utils::ParticleIDMeta> PIDHandler::getAlgoInfo(const podio::Frame& metadata,
                                                                      const std::string& collName) {
  ParticleIDMeta pidInfo{};

  pidInfo.algoName = metadata.getParameter<std::string>(podio::collMetadataParamName(collName, edm4hep::pidAlgoName));
  // Use the algoName as proxy to see whether we could actually get the
  // information from the metadata
  if (pidInfo.algoName.empty()) {
    return std::nullopt;
  }

  pidInfo.algoType = metadata.getParameter<int>(podio::collMetadataParamName(collName, edm4hep::pidAlgoType));
  pidInfo.paramNames = metadata.getParameter<std::vector<std::string>>(
      podio::collMetadataParamName(collName, edm4hep::pidParameterNames));

  return pidInfo;
}

} // namespace edm4hep::utils
