#ifndef DELPHESEDM4HEP_DELPHESEDM4HEP_CONVERTER_H__
#define DELPHESEDM4HEP_DELPHESEDM4HEP_CONVERTER_H__

// delphes
#include "classes/DelphesClasses.h"
#include "ExRootAnalysis/ExRootConfReader.h" // ExRootConfParam
#include "Delphes.h" // NOTE: currently own "fork"

// podio
#include "podio/EventStore.h"
#include "podio/ROOTWriter.h"
#include "podio/CollectionBase.h"

#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <array>
#include <unordered_map>


/**
 * Order in which the different delphes output classes will be processed.
 * Everything not defined here will not be processed
 */
constexpr std::array<std::string_view, 3> PROCESSING_ORDER = {
  "GenParticle",
  "Track",
  "Tower",
  // "Jet",
  // "Photon",
  // "Muon",
  // "Electron",
  // "MissingET",
  // "SclalarHT"
};

/**
 * Classes that will be stored as MCParticles
 */
constexpr std::array<std::string_view, 1> MCPARTICLE_OUTPUT = {"GenParticle"};

/**
 * Branches that will be stored in the global reconstructed particle collection
 */
constexpr std::array<std::string_view, 3> RECOPARTICLE_OUTPUT = {
  "EFlowTrack",
  "EFlowPhoton",
  "EFlowNeutralHadron"
};

constexpr std::string_view RECOPARTICLE_COLLECTION_NAME = "ReconstructedParticles";

/**
 * Classes that will be stored as reconstructed particle with an attached track
 */
constexpr std::array<std::string_view, 1> RECO_TRACK_OUTPUT = {"Track"};

/**
 * Classes that will be stored as reconstructed particle with an attached cluster
 */
constexpr std::array<std::string_view, 1> RECO_CLUSTER_OUTPUT = {"Tower"};

// /**
//  *
//  */
// constexpr std::string_view TRACK_PARTICLE_NAME = ""

// constexpr std::array<std::pair<const char*, const char*>>


struct BranchSettings {
  std::string input;
  std::string name;
  std::string className;
};

class DelphesEDM4HepConverter {
public:
  DelphesEDM4HepConverter(std::string const& outputFile, ExRootConfParam /*const*/& branches,
                          ExRootConfReader* confReader);
  void process(Delphes* modularDelphes);
  void writeEvent();
  void finish();

private:
  void processParticles(const TObjArray* delphesCollection, std::string_view const branch);
  void processTracks(const TObjArray* delphesCollection, std::string_view const branch);
  void processClusters(const TObjArray* delphesCollection, std::string_view const branch);


  using ProcessFunction = void (DelphesEDM4HepConverter::*)(const TObjArray*, std::string_view const);

  podio::EventStore m_store{};
  podio::ROOTWriter m_writer;
  std::vector<BranchSettings> m_branches;
  std::unordered_map<std::string_view, podio::CollectionBase*> m_collections;
  std::unordered_map<std::string_view, ProcessFunction> m_processFunctions;
  double m_magneticFieldBz; // necessary for determining track parameters
};

#endif
