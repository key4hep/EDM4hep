#ifndef EDM4HEP_TO_JSON_H
#define EDM4HEP_TO_JSON_H

// event data model
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
#include "podio/EventStore.h"

// JSON
#include "nlohmann/json.hpp"

// STL
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

template <typename PodioStoreT>
nlohmann::json processEvent(PodioStoreT& store, std::vector<std::string>& collList, bool verboser, unsigned eventNum) {
  const podio::CollectionBase* coll;
  nlohmann::json jsonDict = {{"edm4hepVersion", "0.7.0"}};

  for (unsigned i = 0; i < collList.size(); ++i) {
    int err = store.template get<podio::CollectionBase>(collList[i], coll);
    if (!err) {
      continue;
    }

    if (coll->getTypeName() == "edm4hep::EventHeaderCollection") {
      auto& eventCollection = store.template get<edm4hep::EventHeaderCollection>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", eventCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::MCParticleCollection") {
      auto& particleCollection = store.template get<edm4hep::MCParticleCollection>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", particleCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::MCRecoParticleAssociationCollection") {
      auto& assocCollection = store.template get<edm4hep::MCRecoParticleAssociationCollection>(collList[i]);
      nlohmann::json jsonColl{{
          collList[i],
          {{"collection", assocCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}},
      }};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::VertexCollection") {
      auto& vertexCollection = store.template get<edm4hep::VertexCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", vertexCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::TrackCollection") {
      auto& trackCollection = store.template get<edm4hep::TrackCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", trackCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::TrackerHitCollection") {
      auto& hitCollection = store.template get<edm4hep::TrackerHitCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", hitCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::SimTrackerHitCollection") {
      auto& hitCollection = store.template get<edm4hep::SimTrackerHitCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", hitCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::CalorimeterHitCollection") {
      auto& hitCollection = store.template get<edm4hep::CalorimeterHitCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", hitCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::SimCalorimeterHitCollection") {
      auto& hitCollection = store.template get<edm4hep::SimCalorimeterHitCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i], {{"collection", hitCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::ClusterCollection") {
      auto& clusterCollection = store.template get<edm4hep::ClusterCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", clusterCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::ReconstructedParticleCollection") {
      auto& recoParticleCollection = store.template get<edm4hep::ReconstructedParticleCollection>(collList[i]);
      nlohmann::json jsonColl{
          {collList[i],
           {{"collection", recoParticleCollection}, {"collID", coll->getID()}, {"collType", coll->getTypeName()}}}};
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::ParticleIDCollection") {
      auto& particleIDCollection = store.template get<edm4hep::ParticleIDCollection>(collList[i]);
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

template <typename PodioStoreT>
void printCollTypes(PodioStoreT& store, std::vector<std::string>& collList) {
  const podio::CollectionBase* coll;

  std::cout << "INFO: Converting collections:\n";

  for (unsigned i = 0; i < collList.size(); ++i) {
    int err = store.template get<podio::CollectionBase>(collList[i], coll);
    if (!err) {
      std::cout << "WARNING: Something went wrong, ignoring collection:\n"
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
void read_events(const std::string& filename, const std::string& jsonFile, const std::string& requestedCollections,
                 int nEventsMax = -1, bool verboser = false) {
  ReaderT reader;
  reader.openFile(filename);

  auto store = podio::EventStore();
  store.setReader(&reader);

  nlohmann::json allEventsDict;

  auto collList = splitString(requestedCollections);

  if (verboser) {
    printCollTypes(store, collList);
  }

  unsigned nEvents = reader.getEntries();
  if (nEventsMax > 0) {
    if ((unsigned)nEventsMax < nEvents) {
      nEvents = nEventsMax;
    }
  }
  for (unsigned i = 0; i < nEvents; ++i) {
    if (verboser && i % 1000 == 0) {
      std::cout << "INFO: Reading event " << i << std::endl;
    }
    auto eventDict = processEvent(store, collList, verboser, i);
    allEventsDict["Event " + std::to_string(i)] = eventDict;
    store.clear();
    reader.endOfEvent();
  }
  reader.closeFile();

  std::ofstream outFile(jsonFile);
  if (outFile.is_open()) {
    outFile << std::setw(2) << allEventsDict << std::endl;
    outFile.close();
  }

  if (verboser) {
    std::cout << "INFO: Result of the conversion written to:\n"
              << "      " << jsonFile << std::endl;
  }
}

#endif /* EDM4HEP_TO_JSON_H__ */
