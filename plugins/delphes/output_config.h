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
