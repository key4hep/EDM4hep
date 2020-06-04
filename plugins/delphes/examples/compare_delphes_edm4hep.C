// Make sure these can all be found on LD_LIBRARY_PATH
#ifdef __CLING__
R__LOAD_LIBRARY(libpodio)
R__LOAD_LIBRARY(libpodioDict)
R__LOAD_LIBRARY(libpodioRootIO)
R__LOAD_LIBRARY(libedm4hep)
R__LOAD_LIBRARY(libedm4hepDict)
R__LOAD_LIBRARY(libDelphes)
#endif

#include "AssociationHelper.h"

// make sure that the EDM4hep include directory is in ROOT_INCLUDE_PATH
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/MCRecoParticleAssociationCollection.h"

// also make sure that the podio include directory is in ROOT_INCLUDE_PATH
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

// make sure that the Delphes include directory is in ROOT_INCLUDE_PATH
#include "classes/DelphesClasses.h"
#include "ExRootAnalysis/ExRootTreeReader.h"


void loadLibs() {
   gSystem->Load("libpodio.so");
   gSystem->Load("libpodioDict.so");
   gSystem->Load("libpodioRootIO.so");
   gSystem->Load("libedm4hep.so");
   gSystem->Load("libedm4hepDict.so");
   gSystem->Load("libedm4hepDict.so");
   gSystem->Load("libDelphes.so");
}




void compare_delphes_edm4hep(const char* delphesFile, std::string&& edm4hepFile) {
    loadLibs();

    // initialize delphes
    auto* chain = new TChain("Delphes");
    chain->Add(delphesFile);
    auto* treeReader = new ExRootTreeReader(chain);
    const auto nEntriesDelphes = treeReader->GetEntries();

    auto* genParticlesDelphes = treeReader->UseBranch("Particle");
    auto* electronsDelphes = treeReader->UseBranch("Electron");
    auto* muonsDelphes = treeReader->UseBranch("Muon");
    auto* photonsDelphes = treeReader->UseBranch("Photon");
    auto* jetsDelphes = treeReader->UseBranch("Jet");

    // initialize edm4hep
    auto reader = podio::ROOTReader();
    reader.openFile(edm4hepFile);
    auto store = podio::EventStore();
    store.setReader(&reader);

    std::cout << "Events in Delphes file: " << nEntriesDelphes << "\n"
              << "Events in EDM4Hep file: " << reader.getEntries() << std::endl;



}
