#ifndef EDM4HEP_TO_JSON_H__
#define EDM4HEP_TO_JSON_H__

// event data model
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/VertexCollection.h"
#include "edm4hep/TrackCollection.h"
#include "edm4hep/ClusterCollection.h"
#include "edm4hep/CalorimeterHitCollection.h"
#include "edm4hep/ReconstructedParticleCollection.h"

// podio specific includes
#include "podio/EventStore.h"

// JSON
#include "nlohmann/json.hpp"

// STL
#include <vector>
#include <iostream>
#include <fstream>
#include <exception>
#include <cassert>


nlohmann::json processEvent(podio::EventStore& store,
                            bool verboser,
                            unsigned eventNum) {
  bool verbose = true;
  // auto& particleCollection = store.get<edm4hep::MCParticleCollection>(
      // "MCParticles");
      // "GenParticles");
  // auto& clusterCollection = store.get<edm4hep::ClusterCollection>(
  //     "ECALBarrel");
      // "HCALBarrel");
      // "CorrectedCaloClusters");

  // std::cout << particleCollection.at(0) << std::endl;
  // std::cout << clusterCollection.at(0) << std::endl;

  /*
  bool err = store.get<podio::CollectionBase>("MCParticles", coll);
  if (!err) {
    std::cerr << "Can't find the collection" << std::endl;
    nlohmann::json jsonDict{};
    return jsonDict;
  }

  std::cout << "Collections:\n"
            << " - " << "ECALBarrel" << " | " << coll->getID() << " | "
            << coll->getTypeName() << "\n";
  */

  const podio::CollectionIDTable* collTable = store.getCollectionIDTable();
  const std::vector<std::string> collNames = collTable->names();
  const std::vector<int> collIDs = collTable->ids();
  podio::CollectionBase* coll;
  nlohmann::json jsonDict = {
    {"eventNum", eventNum}
  };

  if (verbose) {
    std::cout << "INFO: Collections:\n";
  }
  for (unsigned i = 0; i < collNames.size(); ++i) {
    int err = store.get(collIDs[i], coll);
    if (!err) {
      if (verbose) {
        std::cout << "WARNING: Something went wrong, ignoring collection:\n"
                  << "         " << collNames[i] << "\n";
      }
      continue;
    }

    if (verbose) {
      std::cout << "       - " << collIDs[i] << " | " << collNames[i] << " | "
                << coll->getTypeName() << "\n";
    }

    /*
    if (coll->getTypeName() == "edm4hep::MCParticleCollection") {
      auto& particleCollection =
          store.get<edm4hep::MCParticleCollection>(collNames[i]);
      nlohmann::json jsonColl {
        {collNames[i], particleCollection}
      };
      jsonDict += jsonColl;
    }
    */

    if (coll->getTypeName() == "edm4hep::VertexCollection") {
      auto& vertexCollection =
          store.get<edm4hep::VertexCollection>(collNames[i]);
      nlohmann::json jsonColl {
        {collNames[i], vertexCollection}
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    }

    if (coll->getTypeName() == "edm4hep::TrackCollection") {
      auto& trackCollection =
          store.get<edm4hep::TrackCollection>(collNames[i]);
      nlohmann::json jsonColl {
        {collNames[i], trackCollection}
      };
      // jsonColl[collNames[i]]["typeName"] = coll->getTypeName();
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    }

    /*
    if (coll->getTypeName() == "edm4hep::CalorimeterHitCollection") {
      auto& caloHitCollection =
          store.get<edm4hep::CalorimeterHitCollection>(collNames[i]);
      nlohmann::json jsonColl {
        {collNames[i], caloHitCollection}
      };
      jsonDict += jsonColl;
    }
    */
    if (coll->getTypeName() == "edm4hep::ClusterCollection") {
      auto& clusterCollection =
          store.get<edm4hep::ClusterCollection>(collNames[i]);
      nlohmann::json jsonColl {
        {collNames[i], clusterCollection}
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    }

    if (coll->getTypeName() == "edm4hep::ReconstructedParticleCollection") {
      auto& recoParticleCollection =
          store.get<edm4hep::ReconstructedParticleCollection>(collNames[i]);
      nlohmann::json jsonColl {
        {collNames[i], recoParticleCollection}
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    }
  }

  return jsonDict;
}

template<typename ReaderT>
void read_events(const std::string& filename, const std::string& jsonFile) {
  ReaderT reader;
  reader.openFile(filename);

  auto store = podio::EventStore();
  store.setReader(&reader);

  nlohmann::json allEventsDict;

  unsigned nEvents = reader.getEntries();
  for(unsigned i=0; i<nEvents; ++i) {
    if (i % 1000 == 0) {
      std::cout << "INFO: Reading event " << i << std::endl;
    }
    auto eventDict = processEvent(store, true, i);
    allEventsDict["Event " + std::to_string(i)] = eventDict;
    store.clear();
    reader.endOfEvent();
  }
  reader.closeFile();

  std::ofstream outFile(jsonFile);
  if (outFile.is_open()) {
    outFile << std::setw(4) << allEventsDict << std::endl;
    outFile.close();
  }
}

#endif /* EDM4HEP_TO_JSON_H__ */
