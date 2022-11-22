#ifndef EDM4HEP_TO_JSON_H__
#define EDM4HEP_TO_JSON_H__

// event data model
#include "edm4hep/EventHeaderCollection.h"
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/MCRecoParticleAssociationCollection.h"
#include "edm4hep/VertexCollection.h"
#include "edm4hep/TrackCollection.h"
#include "edm4hep/TrackerHitCollection.h"
#include "edm4hep/TrackerHitCollection.h"
#include "edm4hep/SimTrackerHitCollection.h"
#include "edm4hep/CalorimeterHitCollection.h"
#include "edm4hep/SimCalorimeterHitCollection.h"
#include "edm4hep/ClusterCollection.h"
#include "edm4hep/CalorimeterHitCollection.h"
#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/ParticleIDCollection.h"

// podio specific includes
#include "podio/EventStore.h"
#include "podio/IReader.h"
#include "podio/UserDataCollection.h"
#include "podio/podioVersion.h"
#include "podio/Frame.h"

// JSON
#include "nlohmann/json.hpp"

// STL
#include <vector>
#include <iostream>
#include <fstream>
#include <exception>
#include <cassert>


template <typename StoreT = podio::EventStore>
nlohmann::json processEvent(StoreT& store,
                            std::vector<std::string>& collList,
                            bool verboser,
                            unsigned eventNum) {
  nlohmann::json jsonDict = {
    {"edm4hepVersion", "0.7.0"}
  };

  for (unsigned i = 0; i < collList.size(); ++i) {
    auto coll = store.template get(collList[i]);
    if (!coll->isValid()) {
      continue;
    }

    if (coll->getTypeName() == "edm4hep::EventHeaderCollection") {
      auto& eventCollection =
          store.template get<edm4hep::EventHeaderCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", eventCollection},
                       {"collID", coll->getID()},
                       {"collType", coll->getTypeName()}},
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::MCParticleCollection") {
      auto& particleCollection =
          store.template get<edm4hep::MCParticleCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", particleCollection},
                       {"collID", coll->getID()},
                       {"collType", coll->getTypeName()}},
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::MCRecoParticleAssociationCollection") {
      auto& assocCollection =
          store.template get<edm4hep::MCRecoParticleAssociationCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", assocCollection},
                       {"collID", coll->getID()},
                       {"collType", coll->getTypeName()}},
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::VertexCollection") {
      auto& vertexCollection =
          store.template get<edm4hep::VertexCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", vertexCollection},
                       {"collID", coll->getID()},
                       {"collType", coll->getTypeName()}}
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::TrackCollection") {
      auto& trackCollection =
          store.template get<edm4hep::TrackCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", trackCollection},
                       {"collID", coll->getID()},
                       {"collType", coll->getTypeName()}}
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::TrackerHitCollection") {
      auto& hitCollection =
          store.template get<edm4hep::TrackerHitCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", hitCollection},
                       {"collID", coll->getID()},
                       {"collType", coll->getTypeName()}}
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::SimTrackerHitCollection") {
      auto& hitCollection =
          store.template get<edm4hep::SimTrackerHitCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", hitCollection},
                       {"collID", coll->getID()},
                       {"collType", coll->getTypeName()}}
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::CalorimeterHitCollection") {
      auto& hitCollection =
          store.template get<edm4hep::CalorimeterHitCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", hitCollection},
                       {"collID", hitCollection.getID()},
                       {"collType", hitCollection.getTypeName()}}
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::SimCalorimeterHitCollection") {
      auto& hitCollection =
          store.template get<edm4hep::SimCalorimeterHitCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", hitCollection},
                       {"collID", coll->getID()},
                       {"collType", coll->getTypeName()}}
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::ClusterCollection") {
      auto& clusterCollection =
          store.template get<edm4hep::ClusterCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", clusterCollection},
                       {"collID", coll->getID()},
                       {"collType", coll->getTypeName()}}
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::ReconstructedParticleCollection") {
      auto& recoParticleCollection =
          store.template get<edm4hep::ReconstructedParticleCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", recoParticleCollection},
                       {"collID", coll->getID()},
                       {"collType", coll->getTypeName()}}
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else if (coll->getTypeName() == "edm4hep::ParticleIDCollection") {
      auto& particleIDCollection =
          store.template get<edm4hep::ParticleIDCollection>(collList[i]);
      nlohmann::json jsonColl {
        {collList[i], {{"collection", particleIDCollection},
                       {"collID", coll->getID()},
                       {"collType", coll->getTypeName()}}
        }
      };
      jsonDict.insert(jsonColl.begin(), jsonColl.end());
    } else {
      std::cout << "WARNING: Collection type not recognized!\n"
                << "         " << coll->getTypeName() << "\n";
    }
  }

  return jsonDict;
}


template <typename StoreT = podio::EventStore>
void printCollTypes(StoreT& store,
                    std::vector<std::string>& collList) {
  std::cout << "INFO: Converted collections:\n";

  for (unsigned i = 0; i < collList.size(); ++i) {
    auto coll = store.template get(collList[i]);
    if (!coll->isValid()) {
      std::cout << "WARNING: Something went wrong, ignored collection:\n"
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
int read_frames(const std::string& filename,
                const std::string& jsonFile,
                const std::string& requestedCollections,
                const std::string& requestedEvents,
                const std::string& frameName,
                int nEventsMax = -1,
                bool verboser = false) {
  ReaderT reader;
  reader.openFile(filename);

  /*
  // if (reader.currentFileVersion() != podio::version::build_version) {
  if (reader.currentFileVersion()) {
    std::cerr << "ERROR: The podio build version could not be read back "
        //       << "correctly. (expected:" << podio::version::build_version
              << ", actual: " << reader.currentFileVersion() << ")"
              << std::endl;
    // return EXIT_FAILURE;
  }
  */

  nlohmann::json allEventsDict;

  unsigned nEvents = reader.getEntries(frameName);
  if (nEventsMax > 0) {
    if ((unsigned) nEventsMax < nEvents) {
      nEvents = nEventsMax;
    }
  }

  auto collList = splitString(requestedCollections);

  auto eventList = splitString(requestedEvents);
  std::vector<int> eventVec;
  for (auto& evnt: eventList) {
    if (eventVec.size() >= (unsigned) nEvents) {
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

    if ((unsigned) evntNum > nEvents) {
      if (verboser) {
        std::cout << "WARNING: Event number larger than number of events in the file or number of events to be processed:\n"
                  << "         " << evnt << "\n";
      }
      continue;
    }

    eventVec.emplace_back(evntNum);
  }
  if (verboser) {
    std::cout << "INFO: Converting the following events:\n";
    for (auto& evnt: eventVec) {
      std::cout << "      " << evnt << "\n";
    }
  }

  if (eventVec.empty()) {
    for (unsigned i = 0; i < nEvents; ++i) {
      if (verboser && i % 1000 == 0) {
        std::cout << "INFO: Reading event " << i << std::endl;
      }
      auto frame = podio::Frame(reader.readNextEntry(frameName));
      auto eventDict = processEvent(frame,
                                    collList,
                                    verboser,
                                    i);
      allEventsDict["Event " + std::to_string(i)] = eventDict;
    }
  } else {
    for (auto& i: eventVec) {
      if (verboser) {
        std::cout << "INFO: Reading event " << i << std::endl;
      }
      auto frame = podio::Frame(reader.readEntry(frameName, i));
      auto eventDict = processEvent(frame,
                                    collList,
                                    verboser,
                                    i);
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
