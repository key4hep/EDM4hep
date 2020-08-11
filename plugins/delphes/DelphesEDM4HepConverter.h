#ifndef DELPHESEDM4HEP_DELPHESEDM4HEP_CONVERTER_H__
#define DELPHESEDM4HEP_DELPHESEDM4HEP_CONVERTER_H__

// delphes
#include "classes/DelphesClasses.h"
#include "ExRootAnalysis/ExRootConfReader.h" // ExRootConfParam
#include "modules/Delphes.h"

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
#include <array>
#include <unordered_map>


/**
 * The settings steering the output of the DelphesEDM4HepConverter. Default
 * values correspond to the ones found in delphes example cards.
 */
struct OutputSettings {

  /**
   * Branches that will be stored in one global ReconstructedParticle collection
   * under the name specified below. These will be associated with the generated
   * particles.
   */
  std::vector<std::string> ReconstructedParticleCollections{{"EflowTrack", "EFlowPhoton","EFlowNeutralHadron"}};

  /**
   * Branches that will be considered and stored as MCParticle. Each Delphes
   * collection will get its own edm4hep collection, but all generated particles
   * will be considered for the associations to reconstructed particles.
   */
  std::vector<std::string> GenParticleCollections{{"GenParticle"}};

  /**
   * Branches that contain delphes jet collections that will be converted to
   * ReconstructedParticle. Each delphes collection gets its own edm4hep
   * collection. The constituents of the jets will be taken from the global
   * ReconstructedParticle collection.
   */
  std::vector<std::string> JetCollections{{"Jet"}};

  /**
   * Name of the delphes muon collections that will be converted to
   * RecoParticleRefs that point into the global reconstructed particle
   * collection. Each delphes collection gets its own edm4hep collection.
   */
  std::vector<std::string> MuonCollections{{"Muon"}};

  /**
   * Name of the delphes electron collections that will be converted to
   * RecoParticleRefs that point into the global reconstructed particle
   * collection. Each delphes collection gets its own edm4hep collection.
   */
  std::vector<std::string> ElectronCollections{{"Electron"}};

  /**
   * Name of the delphes photon collections that will be converted to
   * RecoParticleRefs that point into the global reconstructed particle
   * collection. Each delphes collection gets its own edm4hep collection.
   */
  std::vector<std::string> PhotonCollections{{"Photon"}};

  /**
   * Name of the delphes MissingET collections that are converted and stored as
   * ReconstructedParticle (one per event). Each delphes collection will get its
   * own edm4hep collection.
   */
  std::vector<std::string> MissingETCollections{{"MissingET"}};

  /**
   * Name of the delphes ScalarHT collections that are converted and stored as
   * ParticleID objects (one per event). Each delphes collection will get its
   * own edm4hep collection.
   */
  std::vector<std::string> ScalarHTCollections{{"ScalarHT"}};

  /**
   * Name of the global ReconstructedParticle collection
   */
  std::string RecoParticleCollectionName{"ReconstructedParticles"};

  /**
   * Name of the MCRecoParticleAssociationCollection holding the associations of
   * generated to reconstructed particles.
   */
  std::string MCRecoAssociationCollectionName{"MCRecoAssociations"};


  /**
   * Name of the ParticleIDCollection holding the ctags / isolation variables.
   */
  std::string ParticleIDCollectionName{"ParticleIDs"};
};


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
constexpr std::array<std::string_view, 9> PROCESSING_ORDER = {
  "GenParticle",
  "Track",
  "Tower",
  "Jet",
  "Muon",
  "Electron",
  "Photon",
  "MissingET",
  "SclalarHT"
};


struct BranchSettings {
  std::string input;
  std::string name;
  std::string className;
};

class DelphesEDM4HepConverter {
public:
  DelphesEDM4HepConverter( ExRootConfParam /*const*/& branches,
                          OutputSettings const& outputSettings, double magFieldBz);
  void process(Delphes* modularDelphes);
  inline std::unordered_map<std::string_view, podio::CollectionBase*> getCollections() {return m_collections;};

private:
  void processParticles(const TObjArray* delphesCollection, std::string_view const branch);
  void processTracks(const TObjArray* delphesCollection, std::string_view const branch);
  void processClusters(const TObjArray* delphesCollection, std::string_view const branch);
  void processJets(const TObjArray* delphesCollection, std::string_view const branch);
  void processPhotons(const TObjArray* delphesCollection, std::string_view const branch);
  void processMuonsElectrons(const TObjArray* delphesCollection, std::string_view const branch);
  void processMissingET(const TObjArray* delphesCollection, std::string_view const branch);
  void processScalarHT(const TObjArray* delphesCollection, std::string_view const branch);

  void registerGlobalCollections();

  template<typename CollectionT>
  void registerCollection(std::string_view const name);

  using ProcessFunction = void (DelphesEDM4HepConverter::*)(const TObjArray*, std::string_view const);

  std::vector<BranchSettings> m_branches;
  std::unordered_map<std::string_view, podio::CollectionBase*> m_collections;
  std::unordered_map<std::string_view, ProcessFunction> m_processFunctions;
  double m_magneticFieldBz; // necessary for determining track parameters
  std::string m_recoCollName;
  std::string m_particleIDName;
  std::string m_mcRecoAssocCollName;

  // map from UniqueIDs (delphes generated particles) to MCParticles
  std::unordered_map<UInt_t, edm4hep::ConstMCParticle> m_genParticleIds;
  // map from UniqueIDs (delphes generated particles) to (possibly multiple)
  // ReconstructedParticles
  std::unordered_multimap<UInt_t, edm4hep::ReconstructedParticle> m_recoParticleGenIds;
};

template<typename CollectionT>
void DelphesEDM4HepConverter::registerCollection(std::string_view name) {
  // todo: this is not registering in the event store anymore,
  // but only in the collections map, could be renamed.
  std::string nameStr(name);
  CollectionT* col = new CollectionT();
  m_collections.emplace(name, col);
}

#endif
