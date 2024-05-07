#ifndef EDM4HEP_TO_JSON_H
#define EDM4HEP_TO_JSON_H

// EDM4hep event data model
#include "edm4hep/CaloHitContributionCollection.h"
#include "edm4hep/CalorimeterHitCollection.h"
#include "edm4hep/ClusterCollection.h"
#include "edm4hep/EventHeaderCollection.h"
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/ParticleIDCollection.h"
#include "edm4hep/RawCalorimeterHitCollection.h"
#include "edm4hep/RawTimeSeriesCollection.h"
#include "edm4hep/RecDqdxCollection.h"
#include "edm4hep/RecIonizationClusterCollection.h"
#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/RecTofCollection.h"
#include "edm4hep/SimCalorimeterHitCollection.h"
#include "edm4hep/SimPrimaryIonizationClusterCollection.h"
#include "edm4hep/SimTrackerHitCollection.h"
#include "edm4hep/TimeSeriesCollection.h"
#include "edm4hep/TrackCollection.h"
#include "edm4hep/TrackerHit3DCollection.h"
#include "edm4hep/TrackerHitPlaneCollection.h"
#include "edm4hep/TrackerPulseCollection.h"
#include "edm4hep/VertexCollection.h"

#include "edm4hep/MCRecoCaloAssociationCollection.h"
#include "edm4hep/MCRecoCaloParticleAssociationCollection.h"
#include "edm4hep/MCRecoClusterParticleAssociationCollection.h"
#include "edm4hep/MCRecoParticleAssociationCollection.h"
#include "edm4hep/MCRecoTrackParticleAssociationCollection.h"
#include "edm4hep/MCRecoTrackerAssociationCollection.h"
#include "edm4hep/MCRecoTrackerHitPlaneAssociationCollection.h"
#include "edm4hep/RecoParticleVertexAssociationCollection.h"

#include "edm4hep/EDM4hepVersion.h"

// podio specific includes
#include "podio/Frame.h"
#include "podio/UserDataCollection.h"
#include "podio/podioVersion.h"

// JSON
#include "nlohmann/json.hpp"

// STL
#include <cassert>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

template <typename CollT>
void insertIntoJson(nlohmann::json& jsonDict, const podio::CollectionBase* coll, const std::string& name) {
  const auto* typedColl = static_cast<const CollT*>(coll); // safe to cast, since we have queried the type before
  nlohmann::json jsonColl{
      {name, {{"collection", *typedColl}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
  jsonDict.insert(jsonColl.begin(), jsonColl.end());
}

nlohmann::json processEvent(const podio::Frame& frame, std::vector<std::string>& collList,
                            podio::version::Version podioVersion) {
  std::stringstream podioVersionStr;
  podioVersionStr << podioVersion;
  std::stringstream e4hVersionStr;
  e4hVersionStr << edm4hep::version::build_version;
  nlohmann::json jsonDict = {{"podioVersion", podioVersionStr.str()}, {"edm4hepVersion", e4hVersionStr.str()}};

  for (unsigned i = 0; i < collList.size(); ++i) {
    auto coll = frame.get(collList[i]);
    if (!coll) {
      continue;
    }

    // Datatypes
    if (coll->getTypeName() == "edm4hep::EventHeaderCollection") {
      insertIntoJson<edm4hep::EventHeaderCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::MCParticleCollection") {
      insertIntoJson<edm4hep::MCParticleCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::SimTrackerHitCollection") {
      insertIntoJson<edm4hep::SimTrackerHitCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::CaloHitContributionCollection") {
      insertIntoJson<edm4hep::CaloHitContributionCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::SimCalorimeterHitCollection") {
      insertIntoJson<edm4hep::SimCalorimeterHitCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::RawCalorimeterHitCollection") {
      insertIntoJson<edm4hep::RawCalorimeterHitCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::CalorimeterHitCollection") {
      insertIntoJson<edm4hep::CalorimeterHitCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::ParticleIDCollection") {
      insertIntoJson<edm4hep::ParticleIDCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::ClusterCollection") {
      insertIntoJson<edm4hep::ClusterCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::TrackerHit3DCollection") {
      insertIntoJson<edm4hep::TrackerHit3DCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::TrackerHitPlaneCollection") {
      insertIntoJson<edm4hep::TrackerHitPlaneCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::RawTimeSeriesCollection") {
      insertIntoJson<edm4hep::RawTimeSeriesCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::TrackCollection") {
      insertIntoJson<edm4hep::TrackCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::VertexCollection") {
      insertIntoJson<edm4hep::VertexCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::ReconstructedParticleCollection") {
      insertIntoJson<edm4hep::ReconstructedParticleCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::SimPrimaryIonizationClusterCollection") {
      insertIntoJson<edm4hep::SimPrimaryIonizationClusterCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::TrackerPulseCollection") {
      insertIntoJson<edm4hep::TrackerPulseCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::RecIonizationClusterCollection") {
      insertIntoJson<edm4hep::RecIonizationClusterCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::TimeSeriesCollection") {
      insertIntoJson<edm4hep::TimeSeriesCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::RecDqdxCollection") {
      insertIntoJson<edm4hep::RecDqdxCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::RecTofCollection") {
      insertIntoJson<edm4hep::RecTofCollection>(jsonDict, coll, collList[i]);
    }
    // Associations
    else if (coll->getTypeName() == "edm4hep::MCRecoParticleAssociationCollection") {
      insertIntoJson<edm4hep::MCRecoParticleAssociationCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::MCRecoCaloAssociationCollection") {
      insertIntoJson<edm4hep::MCRecoCaloAssociationCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::MCRecoTrackerAssociationCollection") {
      insertIntoJson<edm4hep::MCRecoTrackerAssociationCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::MCRecoTrackerHitPlaneAssociationCollection") {
      insertIntoJson<edm4hep::MCRecoTrackerHitPlaneAssociationCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::MCRecoCaloParticleAssociationCollection") {
      insertIntoJson<edm4hep::MCRecoCaloParticleAssociationCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::MCRecoClusterParticleAssociationCollection") {
      insertIntoJson<edm4hep::MCRecoClusterParticleAssociationCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::MCRecoTrackParticleAssociationCollection") {
      insertIntoJson<edm4hep::MCRecoTrackParticleAssociationCollection>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "edm4hep::RecoParticleVertexAssociationCollection") {
      insertIntoJson<edm4hep::RecoParticleVertexAssociationCollection>(jsonDict, coll, collList[i]);
    }
    // Podio user data
    else if (coll->getTypeName() == "podio::UserDataCollection<float>") {
      insertIntoJson<podio::UserDataCollection<float>>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "podio::UserDataCollection<double>") {
      insertIntoJson<podio::UserDataCollection<double>>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "podio::UserDataCollection<int8_t>") {
      insertIntoJson<podio::UserDataCollection<int8_t>>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "podio::UserDataCollection<int16_t>") {
      insertIntoJson<podio::UserDataCollection<int16_t>>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "podio::UserDataCollection<int32_t>") {
      insertIntoJson<podio::UserDataCollection<int32_t>>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "podio::UserDataCollection<int64_t>") {
      insertIntoJson<podio::UserDataCollection<int64_t>>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "podio::UserDataCollection<uint8_t>") {
      insertIntoJson<podio::UserDataCollection<uint8_t>>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "podio::UserDataCollection<uint16_t>") {
      insertIntoJson<podio::UserDataCollection<uint16_t>>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "podio::UserDataCollection<uint32_t>") {
      insertIntoJson<podio::UserDataCollection<uint32_t>>(jsonDict, coll, collList[i]);
    } else if (coll->getTypeName() == "podio::UserDataCollection<uint64_t>") {
      insertIntoJson<podio::UserDataCollection<uint64_t>>(jsonDict, coll, collList[i]);
    } else {
      std::cout << "WARNING: Collection type not recognized!\n"
                << "         " << coll->getTypeName() << "\n";
    }
  }

  return jsonDict;
}

std::vector<std::string> splitString(const std::string& inString) {
  std::vector<std::string> outString;
  std::stringstream sStream(inString);

  while (sStream.good()) {
    std::string subString;
    getline(sStream, subString, ',');
    if (!subString.empty()) {
      outString.emplace_back(subString);
    }
  }

  return outString;
}

template <typename ReaderT>
int read_frames(const std::string& filename, const std::string& jsonFile, const std::string& requestedCollections,
                const std::string& requestedEvents, const std::string& frameName, int nEventsMax = -1,
                bool verboser = false) {
  ReaderT reader;
  reader.openFile(filename);

  nlohmann::json allEventsDict;

  unsigned nEvents = reader.getEntries(frameName);
  if (nEvents < 1) {
    std::cout << "WARNING: Input file contains no events!" << std::endl;
    return EXIT_SUCCESS;
  }
  if (nEventsMax > 0) {
    if ((unsigned)nEventsMax < nEvents) {
      nEvents = nEventsMax;
    }
  }

  auto collList = splitString(requestedCollections);
  if (collList.empty()) {
    auto frame = podio::Frame(reader.readEntry(frameName, 0));
    collList = frame.getAvailableCollections();
  }
  if (collList.empty()) {
    std::cout << "WARNING: Input file does not contain any collections!" << std::endl;
    return EXIT_SUCCESS;
  }
  if (verboser) {
    std::cout << "INFO: Following collections will be converted:" << std::endl;
    for (const auto& collName : collList) {
      std::cout << "      * " << collName << std::endl;
    }
  }

  std::vector<int> eventVec;
  if (!requestedEvents.empty()) {
    auto eventList = splitString(requestedEvents);
    for (auto& evnt : eventList) {
      if (eventVec.size() >= nEvents) {
        break;
      }

      int evntNum = -1;
      try {
        evntNum = std::stoi(evnt);
      } catch (...) {
        if (verboser) {
          std::cout << "WARNING: This is not a number:\n"
                    << "         " << evnt << "\n";
        }
        continue;
      }

      if (evntNum < 0) {
        if (verboser) {
          std::cout << "WARNING: Event number lower than zero:\n"
                    << "         " << evnt << "\n";
        }
        continue;
      }

      if ((unsigned)evntNum > nEvents) {
        if (verboser) {
          std::cout << "WARNING: Event number larger than number of events in "
                       "the file or maximal event number to be processed:\n"
                    << "         " << evnt << "\n";
        }
        continue;
      }

      eventVec.emplace_back(evntNum);
    }

    if (eventVec.empty()) {
      return EXIT_FAILURE;
    }

    if (verboser) {
      std::cout << "INFO: Converting the following events:\n";
      std::string evntStr;
      for (auto& evnt : eventVec) {
        evntStr += std::to_string(evnt);
        evntStr += ",";
      }
      evntStr.pop_back();
      std::cout << "      " << evntStr << std::endl;
    }
  }

  if (eventVec.empty()) {
    const unsigned modInfo = nEvents > 10 ? nEvents / 10 : 1;
    for (unsigned i = 0; i < nEvents; ++i) {
      if (verboser && i % modInfo == 0) {
        std::cout << "INFO: Reading event " << i << std::endl;
      }

      auto frame = podio::Frame(reader.readEntry(frameName, i));
      auto eventDict = processEvent(frame, collList, reader.currentFileVersion());
      allEventsDict["Event " + std::to_string(i)] = eventDict;
    }
  } else {
    for (auto& i : eventVec) {
      auto frame = podio::Frame(reader.readEntry(frameName, i));
      auto eventDict = processEvent(frame, collList, reader.currentFileVersion());
      allEventsDict["Event " + std::to_string(i)] = eventDict;
    }
  }

  std::ofstream outFile(jsonFile);
  if (outFile.is_open()) {
    outFile << std::setw(2) << allEventsDict << std::endl;
    outFile.close();
  }

  if (verboser) {
    std::cout << "INFO: Result of the conversion written to:\n"
              << "      " << jsonFile << std::endl;
  }

  return EXIT_SUCCESS;
}

#endif /* EDM4HEP_TO_JSON_H__ */
