#ifndef EDM4HEP_TO_JSON_H
#define EDM4HEP_TO_JSON_H

#include "edm4hep/EDM4hepVersion.h"
#include "edm4hep/edm4hep.h"

#include "podio/Frame.h"
#include "podio/Reader.h"
#include "podio/UserDataCollection.h"
#include "podio/podioVersion.h"

#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

template <typename CollT>
void insertIntoJson(nlohmann::json& jsonDict, const podio::CollectionBase* coll, const std::string& name) {
  const auto* typedColl = static_cast<const CollT*>(coll); // safe to cast, since we have queried the type before
  nlohmann::json jsonColl{{name,
                           {{"collection", *typedColl},
                            {"collID", coll->getID()},
                            {"collType", coll->getTypeName()},
                            {"collSchemaVersion", coll->getSchemaVersion()},
                            {"isSubsetColl", coll->isSubsetCollection()}}}};
  jsonDict.insert(jsonColl.begin(), jsonColl.end());
}

class MapHelper {

  using FunType = void (*)(nlohmann::json&, const podio::CollectionBase*, const std::string&);
  std::map<std::string_view, FunType> m_map;

  template <typename T>
  void addToMap() {
    m_map[T::collection_type::typeName] = &insertIntoJson<typename T::collection_type>;
  }

  template <typename T>
  void addToMapUserDataColl() {
    m_map[T::typeName] = &insertIntoJson<T>;
  }

public:

  template <typename... T>
  void addToMapAll(podio::utils::TypeList<T...>&&) {
    (addToMap<T>(), ...);
  }

  template <typename... T>
  void addToMapAllUserDataColl(podio::utils::TypeList<T...>&&) {
    (addToMapUserDataColl<T>(), ...);
  }

  auto getMap() const { return m_map; }
};


nlohmann::json processEvent(const podio::Frame& frame, std::vector<std::string>& collList,
                            podio::version::Version podioVersion, const MapHelper& mapHelper) {
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

    if (mapHelper.getMap().contains(coll->getTypeName())) {
      mapHelper.getMap().at(coll->getTypeName())(jsonDict, coll, collList[i]);
    }
    // Unknown
    else {
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
int read_frames(const std::string& filename, const std::string& jsonFile, const std::string& requestedCollections,
                const std::string& requestedEvents, const std::string& frameName, MapHelper& mapHelper,
                int nEventsMax = -1, bool verboser = false) {
  podio::Reader reader = podio::makeReader(filename);

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
    auto frame = reader.readFrame(frameName, 0);
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

  mapHelper.addToMapAll(edm4hep::edm4hepDataTypes{});
  mapHelper.addToMapAll(edm4hep::edm4hepLinkTypes{});
  mapHelper.addToMapAllUserDataColl(podio::UserDataCollectionList{});

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

      auto frame = reader.readFrame(frameName, i);
      auto eventDict = processEvent(frame, collList, reader.currentFileVersion(), mapHelper);
      allEventsDict["Event " + std::to_string(i)] = eventDict;
    }
  } else {
    for (auto& i : eventVec) {
      auto frame = reader.readFrame(frameName, i);
      auto eventDict = processEvent(frame, collList, reader.currentFileVersion(), mapHelper);
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
