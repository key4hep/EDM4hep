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
#include "edm4hep/SimCalorimeterHitCollection.h"
#include "edm4hep/SimPrimaryIonizationClusterCollection.h"
#include "edm4hep/SimTrackerHitCollection.h"
#include "edm4hep/TimeSeriesCollection.h"
#include "edm4hep/TrackCollection.h"
#include "edm4hep/TrackerHitCollection.h"
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

// podio specific includes
#include "podio/Frame.h"
#include "podio/UserDataCollection.h"
#include "podio/podioVersion.h"

// JSON
#include "nlohmann/json.hpp"

// STL
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

nlohmann::json processEvent(const podio::Frame& frame, std::vector<std::string>& collList, bool verboser,
                            podio::version::Version podioVersion) {
  std::stringstream podioVersionStr;
  podioVersionStr << podioVersion;
  nlohmann::json jsonDict = {{"podioVersion", podioVersionStr.str()}};

  for (unsigned i = 0; i < collList.size(); ++i) {
    auto coll = frame.get(collList[i]);
    if (!coll) {
      continue;
    }

    // Datatypes
    if (coll->getTypeName() == "edm4hep::EventHeaderCollection") {
      auto& eventCollection = frame.get<edm4hep::EventHeaderCollection>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", eventCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::MCParticleCollection") {
      auto& particleCollection = frame.get<edm4hep::MCParticleCollection>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", particleCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::SimTrackerHitCollection") {
      auto& hitCollection = frame.get<edm4hep::SimTrackerHitCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", hitCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::CaloHitContributionCollection") {
      auto& hitContribCollection = frame.get<edm4hep::CaloHitContributionCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", hitContribCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::SimCalorimeterHitCollection") {
      auto& hitCollection = frame.get<edm4hep::SimCalorimeterHitCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", hitCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::RawCalorimeterHitCollection") {
      auto& hitCollection = frame.get<edm4hep::RawCalorimeterHitCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", hitCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::CalorimeterHitCollection") {
      auto& hitCollection = frame.get<edm4hep::CalorimeterHitCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", hitCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::ParticleIDCollection") {
      auto& particleIDCollection = frame.get<edm4hep::ParticleIDCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", particleIDCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::ClusterCollection") {
      auto& clusterCollection = frame.get<edm4hep::ClusterCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", clusterCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::TrackerHitCollection") {
      auto& hitCollection = frame.get<edm4hep::TrackerHitCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", hitCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::TrackerHitPlaneCollection") {
      auto& hitPlaneCollection = frame.get<edm4hep::TrackerHitPlaneCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", hitPlaneCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::RawTimeSeriesCollection") {
      auto& rtsCollection = frame.get<edm4hep::RawTimeSeriesCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", rtsCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::TrackCollection") {
      auto& trackCollection = frame.get<edm4hep::TrackCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", trackCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::VertexCollection") {
      auto& vertexCollection = frame.get<edm4hep::VertexCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", vertexCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::ReconstructedParticleCollection") {
      auto& recoParticleCollection = frame.get<edm4hep::ReconstructedParticleCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", recoParticleCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::SimPrimaryIonizationClusterCollection") {
      auto& spicCollection = frame.get<edm4hep::SimPrimaryIonizationClusterCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", spicCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::TrackerPulseCollection") {
      auto& tpCollection = frame.get<edm4hep::TrackerPulseCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", tpCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::RecIonizationClusterCollection") {
      auto& ricCollection = frame.get<edm4hep::RecIonizationClusterCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", ricCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::TimeSeriesCollection") {
      auto& tsCollection = frame.get<edm4hep::TimeSeriesCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", tsCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::RecDqdxCollection") {
      auto& recCollection = frame.get<edm4hep::RecDqdxCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", recCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    }
    // Associations
    else if (coll->getTypeName() == "edm4hep::MCRecoParticleAssociationCollection") {
      auto& assocCollection = frame.get<edm4hep::MCRecoParticleAssociationCollection>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", assocCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::MCRecoCaloAssociationCollection") {
      auto& assocCollection = frame.get<edm4hep::MCRecoCaloAssociationCollection>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", assocCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::MCRecoTrackerAssociationCollection") {
      auto& assocCollection = frame.get<edm4hep::MCRecoTrackerAssociationCollection>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", assocCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::MCRecoTrackerHitPlaneAssociationCollection") {
      auto& assocCollection = frame.get<edm4hep::MCRecoTrackerHitPlaneAssociationCollection>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", assocCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::MCRecoCaloParticleAssociationCollection") {
      auto& assocCollection = frame.get<edm4hep::MCRecoCaloParticleAssociationCollection>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", assocCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::MCRecoClusterParticleAssociationCollection") {
      auto& assocCollection = frame.get<edm4hep::MCRecoClusterParticleAssociationCollection>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", assocCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::MCRecoTrackParticleAssociationCollection") {
      auto& assocCollection = frame.get<edm4hep::MCRecoTrackParticleAssociationCollection>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", assocCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::RecoParticleVertexAssociationCollection") {
      auto& assocCollection = frame.get<edm4hep::RecoParticleVertexAssociationCollection>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", assocCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    }
    // Podio user data
    else if (coll->getTypeName() == "podio::UserDataCollection<float>") {
      auto& userCollection = frame.get<podio::UserDataCollection<float>>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", userCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "podio::UserDataCollection<double>") {
      auto& userCollection = frame.get<podio::UserDataCollection<double>>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", userCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "podio::UserDataCollection<int8_t>") {
      auto& userCollection = frame.get<podio::UserDataCollection<int8_t>>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", userCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "podio::UserDataCollection<int16_t>") {
      auto& userCollection = frame.get<podio::UserDataCollection<int16_t>>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", userCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "podio::UserDataCollection<int32_t>") {
      auto& userCollection = frame.get<podio::UserDataCollection<int32_t>>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", userCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "podio::UserDataCollection<int64_t>") {
      auto& userCollection = frame.get<podio::UserDataCollection<int64_t>>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", userCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "podio::UserDataCollection<uint8_t>") {
      auto& userCollection = frame.get<podio::UserDataCollection<uint8_t>>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", userCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "podio::UserDataCollection<uint16_t>") {
      auto& userCollection = frame.get<podio::UserDataCollection<uint16_t>>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", userCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "podio::UserDataCollection<uint32_t>") {
      auto& userCollection = frame.get<podio::UserDataCollection<uint32_t>>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", userCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "podio::UserDataCollection<uint64_t>") {
      auto& userCollection = frame.get<podio::UserDataCollection<uint64_t>>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", userCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
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
      auto eventDict = processEvent(frame, collList, verboser, reader.currentFileVersion());
      allEventsDict["Event " + std::to_string(i)] = eventDict;
    }
  } else {
    for (auto& i : eventVec) {
      auto frame = podio::Frame(reader.readEntry(frameName, i));
      auto eventDict = processEvent(frame, collList, verboser, reader.currentFileVersion());
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
