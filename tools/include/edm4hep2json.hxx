#ifndef EDM4HEP_TO_JSON_H__
#define EDM4HEP_TO_JSON_H__

// event data model
#include "edm4hep/SimCalorimeterHitCollection.h"

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
  auto& clusterCollection = store.get<edm4hep::SimCalorimeterHitCollection>(
      "CorrectedCaloClusters");

  nlohmann::json eventDict;
  eventDict["event number"] = eventNum;
  eventDict["run number"] = 666;
  nlohmann::json allClustersArray;

  if(clusterCollection.isValid()) {

    /*
    std::cout << "Collection: " << "CalorimeterHitCollection" <<  " of type "
        <<  clusterCollection.getValueTypeName() << "\n\n"
        << clusterCollection << std::endl;
    */


    for (size_t i = 0;  i < clusterCollection.size(); ++i) {
      auto cluster = clusterCollection[i];
      auto clusterPosition = cluster.getPosition();
      std::cout << "Event: " << eventNum <<  std::endl;
      std::cout << "x: " << clusterPosition.x << std::endl;
      std::cout << "y: " << clusterPosition.y << std::endl;
      std::cout << "z: " << clusterPosition.z << std::endl;
      std::cout << "e: " << cluster.getEnergy() << std::endl;

      float pt = std::sqrt(std::pow(clusterPosition.x, 2) +
                           std::pow(clusterPosition.y, 2));

      float eta = std::asinh(clusterPosition.z/pt);
      float phi = std::acos(clusterPosition.x/pt);
      if (clusterPosition.y < 0.) {
        phi *= -1;
      }
      float energy = cluster.getEnergy()*5000;
      if (energy < 0.) {
        energy *= -1;
      }

      nlohmann::json clusterDict;
      clusterDict["phi"] = phi;
      clusterDict["eta"] = eta;
      clusterDict["energy"]= energy;

      allClustersArray.emplace_back(clusterDict);
    }

  } else {
    throw std::runtime_error("Collection 'MCParticles' should be present");
  }

  nlohmann::json allJetsDict;
  allJetsDict["ECal Clusters"] = allClustersArray;
  eventDict["Jets"] = allJetsDict;

  return eventDict;
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
