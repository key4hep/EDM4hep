#include "DelphesEDM4HepConverter.h"
#include "DelphesInputReader.h"

#include "TObjArray.h"

#include "ExRootAnalysis/ExRootProgressBar.h"
#include "modules/Delphes.h"

#include <iostream>
#include <signal.h> // SIGINT
#include <memory>

// gracefully handle ctrl+c
static bool interrupted = false;
void SignalHandler(int /*sig*/) {
  interrupted = true;
}

std::vector<std::string> toVecString(ExRootConfParam param, std::vector<std::string>&& defVals)
{
  if (!param.GetSize()) {
    return defVals;
  }

  std::vector<std::string> paramVals(param.GetSize());
  for (int i = 0; i < param.GetSize(); ++i) {
    paramVals[i] = param[i].GetString();
  }
  return paramVals;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& container) {
  if (container.empty()) {
    os << " -empty- ";
    return os;
  }

  for (size_t i = 0; i < container.size() - 1; ++i) {
    os << container[i] << ", ";
  }
  os << container.back();
  return os;
}

std::ostream& operator<<(std::ostream& os, OutputSettings const& settings) {
  os << "---------- DelphesEDM4HepConverter OutputSettings ----------\n";
  os << std::setw(40) << " ReconstructedParticleCollections: " << settings.ReconstructedParticleCollections << "\n";
  os << std::setw(40) << " GenParticleCollections: " << settings.GenParticleCollections << "\n";
  os << std::setw(40) << " JetCollections: " << settings.JetCollections << "\n";
  os << std::setw(40) << " MuonCollections: " << settings.MuonCollections << "\n";
  os << std::setw(40) << " ElectronCollections: " << settings.ElectronCollections << "\n";
  os << std::setw(40) << " PhotonCollections: " << settings.PhotonCollections << "\n";
  os << std::setw(40) << " MissingETCollections: " << settings.MissingETCollections << "\n";
  os << std::setw(40) << " ScalarHTCollections: " << settings.ScalarHTCollections << "\n";
  os << std::setw(40) << " RecoParticleCollectionName: " << settings.RecoParticleCollectionName << "\n";
  os << std::setw(40) << " MCRecoAssociationCollectionName: " << settings.MCRecoAssociationCollectionName << "\n";
  os << "------------------------------------------------------------\n";

  return os;
}

OutputSettings getEDM4hepOutputSettings(ExRootConfReader* confReader)
{
  OutputSettings settings;

  settings.ReconstructedParticleCollections = toVecString(
    confReader->GetParam("EDM4HepOutput::ReconstructedParticleCollections"),
    {"EFlowTrack", "EFlowPhoton", "EFlowNeutralHadron"});

  settings.GenParticleCollections = toVecString(
    confReader->GetParam("EDM4HepOutput::GenParticleCollections"),
    {"GenParticle"});

  settings.JetCollections = toVecString(
    confReader->GetParam("EDM4HepOutput::JetCollections"),
    {"Jet"});

  settings.MuonCollections = toVecString(
    confReader->GetParam("EDM4HepOutput::MuonCollections"),
    {"Muon"});

  settings.ElectronCollections = toVecString(
    confReader->GetParam("EDM4HepOutput::ElectronCollections"),
    {"Electron"});

  settings.PhotonCollections = toVecString(
    confReader->GetParam("EDM4HepOutput::PhotonCollections"),
    {"Photon"});

  settings.MissingETCollections = toVecString(
    confReader->GetParam("EDM4HepOutput::MissingETCollections"),
    {"MissingET"});

   settings.ScalarHTCollections = toVecString(
    confReader->GetParam("EDM4HepOutput::ScalarHTCollections"),
    {"ScalarHT"});

   settings.RecoParticleCollectionName = confReader->GetString("EDM4HepOutput::RecoParticleCollectionName", "ReconstructedParticles");

   settings.MCRecoAssociationCollectionName = confReader->GetString("EDM4HepOutput::MCRecoAssociationCollectionName", "MCRecoAssociations");

  std::cout << settings << std::endl;
  return settings;
}

bool validateDelphesCard(ExRootConfReader* confReader)
{
  const auto* modules = confReader->GetModules();
  const auto execPath = toVecString(confReader->GetParam("::ExecutionPath"), {});

  const std::vector<std::string> outputModules = {"TreeWriter", "EDM4HepOutput"};

  // collect / print all error messages before returning, so that all can be
  // fixed in one go
  bool valid = true;

  for (const auto& required : outputModules) {
    const auto modIt = modules->find(required);
    if (modIt == modules->end()) {
      std::cerr << "**** ERROR: Required module \'" << required << "\' is not defined in the configuration. Please define it." << std::endl;
      valid = false;
    }

    if (std::find(execPath.cbegin(), execPath.cend(), required) != execPath.cend()) {
      std::cerr << "**** ERROR: Module \'" << required << "\' is set in ExecutionPath. Please remove it." << std::endl;
      valid = false;
    }
  }

  return valid;
}

int doit(int argc, char* argv[], DelphesInputReader& inputReader) {
    Delphes* modularDelphes = new Delphes("Delphes");
    std::string outputfile;
    if (!inputReader.init(modularDelphes, argc, argv, outputfile)) {
        return 1;
    }
    signal(SIGINT, SignalHandler);

    try {
      auto confReader = std::make_unique<ExRootConfReader>();
      confReader->ReadFile(argv[1]);
      if(!validateDelphesCard(confReader.get())) {
        return 1;
      }
      modularDelphes->SetConfReader(confReader.get());

      // since even ExRootConfParam::GetSize() is not marked const it is useless
      // to get a const version of it here
      auto branches = confReader->GetParam("TreeWriter::Branch");

      const auto edm4hepOutSettings = getEDM4hepOutputSettings(confReader.get());
      DelphesEDM4HepConverter edm4hepConverter(outputfile, branches,
                                               edm4hepOutSettings,
                                               confReader->GetDouble("ParticlePropagator::Bz", 0) );

      // has to happen before InitTask
      TObjArray* allParticleOutputArray = modularDelphes->ExportArray("allParticles");
      TObjArray* stableParticleOutputArray = modularDelphes->ExportArray("stableParticles");
      TObjArray* partonOutputArray = modularDelphes->ExportArray("partons");

      modularDelphes->InitTask();
      modularDelphes->Clear();

      int maxEvents = confReader->GetInt("::MaxEvents", 0);
      ExRootProgressBar progressBar(-1);
      Int_t eventCounter = 0;
      for (Int_t entry = 0;
           !inputReader.finished() && (maxEvents > 0 ?  entry < maxEvents : true) && !interrupted;
           ++entry) {

        bool success = inputReader.readEvent(modularDelphes,
                                             allParticleOutputArray,
                                             stableParticleOutputArray,
                                             partonOutputArray);
        if (!success) {
          break;
        }

        modularDelphes->ProcessTask();
        edm4hepConverter.process(modularDelphes);
        edm4hepConverter.writeEvent();
        modularDelphes->Clear();
        progressBar.Update(eventCounter, eventCounter);
        eventCounter++;
      }

      progressBar.Update(eventCounter, eventCounter, true);
      progressBar.Finish();
      edm4hepConverter.finish();
      modularDelphes->Finish();
      std::cout << "** Exiting ..." << std::endl;


    } catch (std::runtime_error& e) {
      std::cerr << "** ERROR: " << e.what() << std::endl;
      return 1;
    }

    return 0;
}
