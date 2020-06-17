constexpr std::array<std::string_view, 1> MCPARTICLE_OUTPUT = {"GenParticle"};

constexpr std::array<std::string_view, 6> RECOPARTICLE_OUTPUT = {
  "EFlowTrack",
  "EFlowNeutralHadron",
  "EFlowNeutralHadron_MainCal",
  "EFlowPhoton",
  "EFlowPhoton_MainCal",
  "beamcalPhotons"
};

constexpr std::string_view RECOPARTICLE_COLLECTION_NAME = "ReconstructedParticles";

constexpr std::string_view MCRECO_ASSOCIATION_COLLECTION_NAME = "MCRecoAssociations";

constexpr std::array<std::string_view, 2> JET_COLLECTIONS = {"Jet", "Jet_MainCal"};

constexpr std::array<std::string_view, 1> MUON_COLLECTIONS = {"Muon"};

constexpr std::array<std::string_view, 1> ELECTRON_COLLECTIONS = {"Electron"};

constexpr std::array<std::string_view, 3> PHOTON_COLLECTIONS = {
"Photon",
"Photon_MainCal",
"beamcalPhotons"};

constexpr std::array<std::string_view, 1> MISSINGET_COLLECTIONS = {"MissingET"};

constexpr std::array<std::string_view, 1> SCALARHT_COLLECTIONS = {"ScalarHT"};
