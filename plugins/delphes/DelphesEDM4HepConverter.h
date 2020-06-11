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


/**
 * Order in which the different delphes output classes will be processed.
 * Everything not defined here will not be processed
 */
constexpr std::array<const char*, 9> PROCESSING_ORDER = {
  "GenParticle",
  "Track",
  "Tower",
  "Jet",
  "Photon",
  "Muon",
  "Electron",
  "MissingET",
  "SclalarHT"
};

// /**
//  * Classes that will be stored as MCParticles
//  */
// constexpr std::array<const char*, 1> MCPARTICLE_OUTPUT = {"GenParticle"};

// /**
//  * Classes that will be stored as reconstructed particles
//  */
// constexpr std::array<const char*, > RECOPARTICLE_OUTPUT = {

// };



struct BranchSettings {
  std::string input;
  std::string name;
  std::string className;
};

class DelphesEDM4HepConverter {
public:
  DelphesEDM4HepConverter(std::string const& outputFile, ExRootConfParam /*const*/& branches);
  void process(Delphes* modularDelphes);
  void finish();

private:
  void writeEvent();

  podio::EventStore m_store{};
  podio::ROOTWriter m_writer;
  std::vector<BranchSettings> m_branches;
  std::vector<std::pair<std::string_view, podio::CollectionBase*>> m_collections;
};

#endif
