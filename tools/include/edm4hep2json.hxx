#ifndef EDM4HEP_TO_JSON_H__
#define EDM4HEP_TO_JSON_H__

// event data model
#include "edm4hep/ClusterCollection.h"

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
  auto& clusterCollection = store.get<edm4hep::ClusterCollection>(
      "CorrectedCaloClusters");

  nlohmann::json jsonDict{
    {"calo_clusters", clusterCollection}
  };

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
    if(i%1000==0)
      std::cout<<"reading event "<<i<<std::endl;
    auto eventDict = processEvent(store, true, i);
    allEventsDict["Event " + std::to_string(i)] = eventDict;
    store.clear();
    reader.endOfEvent();
  }
  reader.closeFile();

  std::ofstream outFile(jsonFile);
  if (outFile.is_open()) {
    outFile << allEventsDict << std::endl;
    outFile.close();
  }
}

#endif /* EDM4HEP_TO_JSON_H__ */
