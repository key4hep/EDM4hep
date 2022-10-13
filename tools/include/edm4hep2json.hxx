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
                            std::vector<std::string>& collList,
                            bool verboser,
                            unsigned eventNum) {
  const podio::CollectionBase* coll;
  nlohmann::json jsonDict = {
    {"eventNum", eventNum},
    {"edm4hepVersion", "0.7.0"}
  };

  for (unsigned i = 0; i < collList.size(); ++i) {
    int err = store.get<podio::CollectionBase>(collList[i], coll);
    if (!err) {
      continue;
    }

    if (coll->getTypeName() == "edm4hep::VertexCollection") {
      auto& vertexCollection =
          store.get<edm4hep::VertexCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", vertexCollection},
                       {"collType", coll->getTypeName()}}
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    }

    if (coll->getTypeName() == "edm4hep::TrackCollection") {
      auto& trackCollection =
          store.get<edm4hep::TrackCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", trackCollection},
                       {"collType", coll->getTypeName()}}
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    }

    if (coll->getTypeName() == "edm4hep::ClusterCollection") {
      auto& clusterCollection =
          store.get<edm4hep::ClusterCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", clusterCollection},
                       {"collType", coll->getTypeName()}}
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    }

    if (coll->getTypeName() == "edm4hep::ReconstructedParticleCollection") {
      auto& recoParticleCollection =
          store.get<edm4hep::ReconstructedParticleCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", recoParticleCollection},
                       {"collType", coll->getTypeName()}}
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    }
  }

  return jsonDict;
}


void printCollTypes(podio::EventStore& store,
                    std::vector<std::string>& collList) {
  const podio::CollectionBase* coll;

  std::cout << "INFO: Converting collections:\n";

  for (unsigned i = 0; i < collList.size(); ++i) {
    int err = store.get<podio::CollectionBase>(collList[i], coll);
    if (!err) {
        std::cout << "WARNING: Something went wrong, ignoring collection:\n"
                  << "         " << collList[i] << "\n";

      continue;
    }

    std::cout << "       - " << coll->getID() << " | " << collList[i]
              << " | " << coll->getTypeName() << "\n";
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


template<typename ReaderT>
void read_events(const std::string& filename,
                 const std::string& jsonFile,
                 const std::string& requestedCollections,
                 bool verboser = false) {
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
