#ifndef EDM4HEP_TO_JSON_H
#define EDM4HEP_TO_JSON_H

// EDM4hep event data model
#include "edm4hep/CalorimeterHitCollection.h"
#include "edm4hep/ClusterCollection.h"
#include "edm4hep/EventHeaderCollection.h"
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/MCRecoParticleAssociationCollection.h"
#include "edm4hep/ParticleIDCollection.h"
#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/SimCalorimeterHitCollection.h"
#include "edm4hep/SimTrackerHitCollection.h"
#include "edm4hep/TrackCollection.h"
#include "edm4hep/TrackerHitCollection.h"
#include "edm4hep/VertexCollection.h"

// podio specific includes
#include "podio/Frame.h"
#include "podio/IReader.h"
#include "podio/UserDataCollection.h"
#include "podio/podioVersion.h"

// JSON
#include "nlohmann/json.hpp"

// STL
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

nlohmann::json processEvent(const podio::Frame& frame, std::vector<std::string>& collList, bool verboser,
                            unsigned eventNum, podio::version::Version podioVersion) {
  nlohmann::json jsonDict = {{"edm4hepVersion", "0.7.0"}};

  std::string podioVersionStr = std::to_string(podioVersion.major);
  podioVersionStr += ".";
  podioVersionStr += std::to_string(podioVersion.minor);
  podioVersionStr += ".";
  podioVersionStr += std::to_string(podioVersion.patch);
  jsonDict.push_back({"podioVersion", podioVersionStr});

  for (unsigned i = 0; i < collList.size(); ++i) {
    auto coll = frame.get(collList[i]);
    if (!coll) {
      continue;
    }

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
    } else if (coll->getTypeName() == "edm4hep::MCRecoParticleAssociationCollection") {
      auto& assocCollection = frame.get<edm4hep::MCRecoParticleAssociationCollection>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", assocCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::VertexCollection") {
      auto& vertexCollection = frame.get<edm4hep::VertexCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", vertexCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::TrackCollection") {
      auto& trackCollection = frame.get<edm4hep::TrackCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", trackCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::TrackerHitCollection") {
      auto& hitCollection = frame.get<edm4hep::TrackerHitCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", hitCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::SimTrackerHitCollection") {
      auto& hitCollection = frame.get<edm4hep::SimTrackerHitCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", hitCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::CalorimeterHitCollection") {
      auto& hitCollection = frame.get<edm4hep::CalorimeterHitCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", hitCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::SimCalorimeterHitCollection") {
      auto& hitCollection = frame.get<edm4hep::SimCalorimeterHitCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", hitCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::ClusterCollection") {
      auto& clusterCollection = frame.get<edm4hep::ClusterCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", clusterCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::ReconstructedParticleCollection") {
      auto& recoParticleCollection = frame.get<edm4hep::ReconstructedParticleCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", recoParticleCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::ParticleIDCollection") {
      auto& particleIDCollection = frame.get<edm4hep::ParticleIDCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", particleIDCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else {
      std::cout << "WARNING: Collection type not recognized!\n"
                << "         " << coll->getTypeName() << "\n";
    }
  }

  return jsonDict;
}

void printCollTypes(const podio::Frame& frame, std::vector<std::string>& collList) {
  std::cout << "INFO: Converted collections:\n";

  for (unsigned i = 0; i < collList.size(); ++i) {
    auto coll = frame.get(collList[i]);
    if (!coll) {
      std::cout << "WARNING: Something went wrong, ignored collection:\n"
                << "         " << collList[i] << "\n";

      continue;
    }

    std::cout << "       - " << coll->getID() << " | " << collList[i] << " | " << coll->getTypeName() << "\n";
  }
}

std::vector<std::string> splitString(const std::string& inString) {
  std::vector<std::string> outString;
  std::stringstream sStream(inString);

  while (sStream.good()) {
    std::string subString;
    getline(sStream, subString, ',');
    outString.emplace_back(subString);
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
  if (nEventsMax > 0) {
    if ((unsigned)nEventsMax < nEvents) {
      nEvents = nEventsMax;
    }
  }

  auto collList = splitString(requestedCollections);

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
      return EXIT_SUCCESS;
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
    unsigned modInfo = nEvents / 10;
    for (unsigned i = 0; i < nEvents; ++i) {
      if (verboser && i % modInfo == 0) {
        std::cout << "INFO: Reading event " << i << std::endl;
      }

      auto frame = podio::Frame(reader.readNextEntry(frameName));
      auto eventDict = processEvent(frame, collList, verboser, i, reader.currentFileVersion());
      allEventsDict["Event " + std::to_string(i)] = eventDict;
    }
  } else {
    for (auto& i : eventVec) {
      auto frame = podio::Frame(reader.readEntry(frameName, i));
      auto eventDict = processEvent(frame, collList, verboser, i, reader.currentFileVersion());
      allEventsDict["Event " + std::to_string(i)] = eventDict;
    }
  }

  if (verboser) {
    auto frame = podio::Frame(reader.readEntry(frameName, 0));
    printCollTypes(frame, collList);
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
