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

// edm4hep
#include "edm4hep/MCParticleConst.h"
#include "edm4hep/ReconstructedParticle.h"

#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <array>
#include <unordered_map>



/**
 * Classes that will be stored as MCParticles (each collection gets its own
 * edm4hep output collection)
 */
constexpr std::array<std::string_view, 1> MCPARTICLE_OUTPUT = {"GenParticle"};

/**
 * Branches that will be stored in the global reconstructed particle collection.
 * These will also get associations with MC particles
 */
constexpr std::array<std::string_view, 3> RECOPARTICLE_OUTPUT = {
  "EFlowTrack",
  "EFlowPhoton",
  "EFlowNeutralHadron"
};

/**
 * Name of the output global reconstructed particle collection that is the
 * combination of all delphes collections in RECOPARTICLE_OUTPUT
 */
constexpr std::string_view RECOPARTICLE_COLLECTION_NAME = "ReconstructedParticles";

/**
 * Name of the collection holding the MC <-> Reco associations
 */
constexpr std::string_view MCRECO_ASSOCIATION_COLLECTION_NAME = "MCRecoAssociations";


/**
 * Name of the delphes jet collections that are converted (each collection gets
 * its own edm4hep output collection)
 */
constexpr std::array<std::string_view, 1> JET_COLLECTIONS = {"Jet"};

/**
 * Name of the delphes muon collections that are converted and stored as
 * RecoParticleRefs that point into the global reconstructed particle collection
 */
constexpr std::array<std::string_view, 1> MUON_COLLECTIONS = {"Muon"};


/**
 * Name of the delphes electron collections that are converted and stored as
 * RecoParticleRefs that point into the global reconstructed particle collection
 */
constexpr std::array<std::string_view, 1> ELECTRON_COLLECTIONS = {"Electron"};

/**
 * Name of the delphes photon collections that are converted and stored as
 * RecoParticleRefs that point into the global reconstructed particle collection
 */
constexpr std::array<std::string_view, 1> PHOTON_COLLECTIONS = {"Photon"};


/**
 * Classes that will be stored as reconstructed particle with an attached track
 */
constexpr std::array<std::string_view, 1> RECO_TRACK_OUTPUT = {"Track"};

/**
 * Classes that will be stored as reconstructed particle with an attached cluster
 */
constexpr std::array<std::string_view, 1> RECO_CLUSTER_OUTPUT = {"Tower"};


/**
 * Order in which the different delphes output classes will be processed.
 * Everything not defined here will not be processed.
 *
 * NOTE: not a configuration parameter. this has to be done in this order to
 * ensure that products required by later stages are producd early enough
 */
constexpr std::array<std::string_view, 7> PROCESSING_ORDER = {
  "GenParticle",
  "Track",
  "Tower",
  "Jet",
  "Muon",
  "Electron",
  "Photon",
  // "MissingET",
  // "SclalarHT"
};


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
  void processJets(const TObjArray* delphesCollection, std::string_view const branch);
  void processPhotons(const TObjArray* delphesCollection, std::string_view const branch);
  void processMuonsElectrons(const TObjArray* delphesCollection, std::string_view const branch);

  void registerGlobalCollections();

  using ProcessFunction = void (DelphesEDM4HepConverter::*)(const TObjArray*, std::string_view const);

  podio::EventStore m_store{};
  podio::ROOTWriter m_writer;
  std::vector<BranchSettings> m_branches;
  std::unordered_map<std::string_view, podio::CollectionBase*> m_collections;
  std::unordered_map<std::string_view, ProcessFunction> m_processFunctions;
  double m_magneticFieldBz; // necessary for determining track parameters

  // map from UniqueIDs (delphes generated particles) to MCParticles
  std::unordered_map<UInt_t, edm4hep::ConstMCParticle> m_genParticleIds;
  // map from UniqueIDs (delphes generated particles) to (possibly multiple)
  // ReconstructedParticles
  std::unordered_multimap<UInt_t, edm4hep::ReconstructedParticle> m_recoParticleGenIds;
};

#endif
