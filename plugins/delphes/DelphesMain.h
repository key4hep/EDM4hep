/*
 *  Delphes: a framework for fast simulation of a generic collider experiment
 *  Copyright (C) 2012-2014  Universite catholique de Louvain (UCL), Belgium
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <functional>

#include <map>
#include <unordered_map>
#include <vector>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "TApplication.h"
#include "TROOT.h"

#include "TClonesArray.h"
#include "TDatabasePDG.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TObjArray.h"
#include "TParticlePDG.h"
#include "TStopwatch.h"

#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"
#include "classes/DelphesStream.h"
#include "modules/Delphes.h"

#include "ExRootAnalysis/ExRootProgressBar.h"
#include "ExRootAnalysis/ExRootTreeBranch.h"
#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ExRootAnalysis/ExRootTreeWriter.h"

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTWriter.h"


#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/MCParticleCollection.h"

#include "DelphesRootReader.h"


using std::cout;
using std::cerr;
using std::endl;

static bool interrupted = false;

void SignalHandler(int sig) {
  interrupted = true;
}


int doit(int argc, char *argv[], DelphesInputReader& inputReader) {
  std::string appName = "DelphesROOT_EDM4HEP";
  std::unique_ptr<TFile> outputFile = nullptr;
  std::unique_ptr<TFile> inputFile = nullptr;

  //DelphesRootReader inputReader = DelphesRootReader();
  inputReader.init(argc, argv);


  Int_t i;
  Long64_t eventCounter, numberOfEvents;

  if(argc < 4)
  {
    cout << " Usage: " << appName << " config_file"
         << " output_file"
         << " input_file(s)" << endl;
    cout << " config_file - configuration file in Tcl format," << endl;
    cout << " output_file - output file in ROOT format," << endl;
    cout << " input_file(s) - input file(s) in ROOT format." << endl;
    return 1;
  }

  signal(SIGINT, SignalHandler);


  try {

    podio::EventStore store;
    podio::ROOTWriter  writer(argv[2], &store);

    auto confReader = std::make_unique<ExRootConfReader>();
    confReader->ReadFile(argv[1]);

    // todo: ROOT error on 6.20.04 if this is a unique pointer
    Delphes* modularDelphes = new Delphes("Delphes");
    modularDelphes->SetConfReader(confReader.get());

    ExRootConfParam branches = confReader->GetParam("TreeWriter::Branch");
    int nParams = branches.GetSize();

    // create collections in eventstore
    // unfortunately cannot create map with references
    std::vector<std::reference_wrapper<edm4hep::ReconstructedParticleCollection>> recPartCollVec;
    // TODO: need additional vectors for other 
    for(int b = 0; b < nParams; b += 3) {
      TString input = branches[b].GetString();
      TString name = branches[b + 1].GetString();
      TString className = branches[b + 2].GetString();
      //std::cout <<  input << "\t" << name << "\t" << className << std::endl;
      // classes that are to be translated to a Reconstructed Particle
      if (className == "Jet" || className == "Electron" || className ==  "Muon" || className == "Photon") {
        edm4hep::ReconstructedParticleCollection& _coll = store.create<edm4hep::ReconstructedParticleCollection>(name.Data());
        writer.registerForWrite(name.Data());
        recPartCollVec.push_back(_coll);
      } else if (className == "GenParticle") {
        //TODO
      } else if (className == "ScalarHT") {
        //TODO
      } else if (className == "MissingET") {
        //TODO
      }
    }



    // has to happen before InitTask
    TObjArray* allParticleOutputArray = modularDelphes->ExportArray("allParticles");
    TObjArray* stableParticleOutputArray = modularDelphes->ExportArray("stableParticles");
    TObjArray* partonOutputArray = modularDelphes->ExportArray("partons");

    modularDelphes->InitTask();


    ExRootProgressBar progressBar(-1);
    // Loop over all objects
    eventCounter = 0;
    modularDelphes->Clear();


    for (Int_t entry = 0; entry < inputReader.getNumberOfEvents() && !interrupted; ++entry) {
      
      inputReader.readEvent(modularDelphes, allParticleOutputArray, stableParticleOutputArray, partonOutputArray);

      modularDelphes->ProcessTask();

        unsigned int collcounter = 0;
        for(int b = 0; b < nParams; b += 3) {
          TString input = branches[b].GetString();
          TString name = branches[b + 1].GetString();
          TString className = branches[b + 2].GetString();
          //std::cout << input << "\t" << name << "\t" << className << std::endl;
          if (className == "Jet") {
            edm4hep::ReconstructedParticleCollection& mcps = recPartCollVec[collcounter++];
            const TObjArray* delphesColl = modularDelphes->ImportArray(input);
            for (int j = 0; j < delphesColl->GetEntries(); j++) {
              auto cand = static_cast<Candidate*>(delphesColl->At(j));
              auto mcp1 = mcps.create();
              mcp1.setMass( cand->Mass ) ;
              mcp1.setCharge( cand->Charge );
              mcp1.setMomentum( { cand->Momentum.Px(), cand->Momentum.Py(), cand->Momentum.Pz() }  ) ;
              //TODO set particleID
              //TODO set location
              //TODO ...
            }
          }
        }
        modularDelphes->Clear();
        writer.writeEvent();
        store.clearCollections();
        progressBar.Update(eventCounter, eventCounter);
        ++eventCounter;
      }
      progressBar.Update(eventCounter, eventCounter, kTRUE);
      progressBar.Finish();
      writer.finish();
      modularDelphes->FinishTask();
      cout << "** Exiting..." << endl;
      return 0;
    } catch(std::runtime_error &e) {
      cerr << "** ERROR: " << e.what() << endl;
      return 1;
    }
}

