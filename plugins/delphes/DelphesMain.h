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
#include "Math/GenVector/LorentzVector.h"
#include "Math/Vector4D.h"
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
// NOTE: locally updated  prototypes!
#include "ROOTWriter.h"
#include "EventStore2.h"


#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/MCParticleCollection.h"

#include "DelphesRootReader.h"


using std::cout;
using std::cerr;
using std::endl;

// gracefully handle ctrl+c
static bool interrupted = false;
void SignalHandler(int sig) {
  interrupted = true;
}


// main function with generic input
int doit(int argc, char *argv[], DelphesInputReader& inputReader) {
  std::string appName = "DelphesROOT_EDM4HEP";
  inputReader.init(argc, argv);
  Long64_t eventCounter, numberOfEvents;
  // command line argument handling
  if(argc < 4) {
    cout << " Usage: " << appName << " config_file"
         << " output_file"
         << " input_file(s)" << endl;
    cout << " config_file - configuration file in Tcl format," << endl;
    cout << " output_file - output file in ROOT format," << endl;
    cout << " input_file(s) - input file(s) in ROOT format." << endl;
    return 1;
  }
  // gracefully handle ctrl+c
  signal(SIGINT, SignalHandler);
  try {
    podio::EventStore store;
    podio::ROOTWriter  writer(argv[2], &store);
    // expose ttree directly to add additional branches (experimental)
    TTree* eventsTree = writer.getEventsTree();

    auto confReader = std::make_unique<ExRootConfReader>();
    confReader->ReadFile(argv[1]);

    // todo: ROOT error on 6.20.04 if this is a unique pointer
    Delphes* modularDelphes = new Delphes("Delphes");
    modularDelphes->SetConfReader(confReader.get());

    ExRootConfParam branches = confReader->GetParam("TreeWriter::Branch");
    int nParams = branches.GetSize();

    std::unordered_map<std::string, podio::CollectionBase*> collmap;
    std::unordered_map<std::string, TLorentzVector*> collmap_met;
    std::unordered_map<std::string, std::vector<ROOT::Math::PxPyPzEVector>*> collmap_4v;
    std::unordered_map<std::string, std::vector<float>*> collmap_float;
    for(int b = 0; b < nParams; b += 3) {
      TString input = branches[b].GetString();
      TString name = branches[b + 1].GetString();
      TString className = branches[b + 2].GetString();
      //std::cout <<  input << "\t" << name << "\t" << className << std::endl;
      // classes that are to be translated to a Reconstructed Particle
      if (className == "Jet") {
        store.create<edm4hep::ReconstructedParticleCollection>(name.Data());

        writer.registerForWrite(name.Data());
        edm4hep::ReconstructedParticleCollection* col2;
        store.get2(name.Data(), col2);
        collmap.insert({name.Data(), col2});

        // additional unstructured branches
        std::vector<ROOT::Math::PxPyPzEVector>* _v = new std::vector<ROOT::Math::PxPyPzEVector>();
        collmap_4v.insert({(name+"SoftDroppedJet").Data(), _v});
        eventsTree->Branch((name + "SoftDroppedJet").Data(), &(collmap_4v[(name+"SoftDroppedJet").Data()]));

        // additional unstructured branches
        _v = new std::vector<ROOT::Math::PxPyPzEVector>();
        collmap_4v.insert({(name+"SoftDroppedSubJet1").Data(), _v});
        eventsTree->Branch((name + "SoftDroppedSubJet1").Data(), &(collmap_4v[(name+"SoftDroppedSubJet1").Data()]));

        _v = new std::vector<ROOT::Math::PxPyPzEVector>();
        collmap_4v.insert({(name+"SoftDroppedSubJet2").Data(), _v});
        eventsTree->Branch((name + "SoftDroppedSubJet2").Data(), &(collmap_4v[(name+"SoftDroppedSubJet2").Data()]));

        auto _vf = new std::vector<float>();
        collmap_float.insert({(name+"Tau1").Data(), _vf});
        eventsTree->Branch((name + "Tau1").Data(), &(collmap_float[(name+"Tau1").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"Tau2").Data(), _vf});
        eventsTree->Branch((name + "Tau2").Data(), &(collmap_float[(name+"Tau2").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"Tau3").Data(), _vf});
        eventsTree->Branch((name + "Tau3").Data(), &(collmap_float[(name+"Tau3").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"Tau4").Data(), _vf});
        eventsTree->Branch((name + "Tau4").Data(), &(collmap_float[(name+"Tau4").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"Tau5").Data(), _vf});
        eventsTree->Branch((name + "Tau5").Data(), &(collmap_float[(name+"Tau5").Data()]));

        auto _vff = new std::vector<Float_t[5]>();
        eventsTree->Branch((name + "Tau").Data(), &_vff);

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"DeltaEta").Data(), _vf});
        eventsTree->Branch((name + "DeltaEta").Data(), &(collmap_float[(name+"DeltaEta").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"DeltaPhi").Data(), _vf});
        eventsTree->Branch((name + "DeltaPhi").Data(), &(collmap_float[(name+"DeltaPhi").Data()]));

        // ...


      } else if (className == "Photon") {
        store.create<edm4hep::ReconstructedParticleCollection>(name.Data());
        writer.registerForWrite(name.Data());
        edm4hep::ReconstructedParticleCollection* col2;
        store.get2(name.Data(), col2);
        collmap.insert({name.Data(), col2});
        auto _vf = new std::vector<float>();
        collmap_float.insert({(name+"EhadOverEem").Data(), _vf});
        eventsTree->Branch((name + "EhadOverEem").Data(), &(collmap_float[(name+"EhadOverEem").Data()]));

         _vf = new std::vector<float>();
        collmap_float.insert({(name+"IsolationVar").Data(), _vf});
        eventsTree->Branch((name + "IsolationVar").Data(), &(collmap_float[(name+"IsolationVar").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"IsolationVarRhoCorr").Data(), _vf});
        eventsTree->Branch((name + "IsolationVarRhoCorr").Data(), &(collmap_float[(name+"IsolationVarRhoCorr").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"SumPtCharged").Data(), _vf});
        eventsTree->Branch((name + "SumPtCharged").Data(), &(collmap_float[(name+"SumPtCharged").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"SumPtChargedPU").Data(), _vf});
        eventsTree->Branch((name + "SumPtChargedPU").Data(), &(collmap_float[(name+"SumPtChargedPU").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"SumPt").Data(), _vf});
        eventsTree->Branch((name + "SumPt").Data(), &(collmap_float[(name+"SumPt").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"SumPtNeutral").Data(), _vf});
        eventsTree->Branch((name + "SumPtNeutral").Data(), &(collmap_float[(name+"SumPtNeutral").Data()]));

      } else if (className == "Electron") {
        store.create<edm4hep::ReconstructedParticleCollection>(name.Data());
        writer.registerForWrite(name.Data());
        edm4hep::ReconstructedParticleCollection* col2;
        store.get2(name.Data(), col2);
        collmap.insert({name.Data(), col2});




        auto _vf = new std::vector<float>();
        collmap_float.insert({(name+"IsolationVar").Data(), _vf});
        eventsTree->Branch((name + "IsolationVar").Data(), &(collmap_float[(name+"IsolationVar").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"IsolationVarRhoCorr").Data(), _vf});
        eventsTree->Branch((name + "IsolationVarRhoCorr").Data(), &(collmap_float[(name+"IsolationVarRhoCorr").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"SumPtCharged").Data(), _vf});
        eventsTree->Branch((name + "SumPtCharged").Data(), &(collmap_float[(name+"SumPtCharged").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"SumPtChargedPU").Data(), _vf});
        eventsTree->Branch((name + "SumPtChargedPU").Data(), &(collmap_float[(name+"SumPtChargedPU").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"SumPt").Data(), _vf});
        eventsTree->Branch((name + "SumPt").Data(), &(collmap_float[(name+"SumPt").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"SumPtNeutral").Data(), _vf});
        eventsTree->Branch((name + "SumPtNeutral").Data(), &(collmap_float[(name+"SumPtNeutral").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"D0").Data(), _vf});
        eventsTree->Branch((name + "D0").Data(), &(collmap_float[(name+"D0").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"ErrorD0").Data(), _vf});
        eventsTree->Branch((name + "ErrorD0").Data(), &(collmap_float[(name+"ErrorD0").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"DZ").Data(), _vf});
        eventsTree->Branch((name + "DZ").Data(), &(collmap_float[(name+"DZ").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"ErrorDZ").Data(), _vf});
        eventsTree->Branch((name + "ErrorDZ").Data(), &(collmap_float[(name+"ErrorDZ").Data()]));



      } else if (className == "Muon") {
        store.create<edm4hep::ReconstructedParticleCollection>(name.Data());
        writer.registerForWrite(name.Data());
        edm4hep::ReconstructedParticleCollection* col2;
        store.get2(name.Data(), col2);
        collmap.insert({name.Data(), col2});

        auto _vf = new std::vector<float>();
        collmap_float.insert({(name+"IsolationVar").Data(), _vf});
        eventsTree->Branch((name + "IsolationVar").Data(), &(collmap_float[(name+"IsolationVar").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"IsolationVarRhoCorr").Data(), _vf});
        eventsTree->Branch((name + "IsolationVarRhoCorr").Data(), &(collmap_float[(name+"IsolationVarRhoCorr").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"SumPtCharged").Data(), _vf});
        eventsTree->Branch((name + "SumPtCharged").Data(), &(collmap_float[(name+"SumPtCharged").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"SumPtChargedPU").Data(), _vf});
        eventsTree->Branch((name + "SumPtChargedPU").Data(), &(collmap_float[(name+"SumPtChargedPU").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"SumPt").Data(), _vf});
        eventsTree->Branch((name + "SumPt").Data(), &(collmap_float[(name+"SumPt").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"SumPtNeutral").Data(), _vf});
        eventsTree->Branch((name + "SumPtNeutral").Data(), &(collmap_float[(name+"SumPtNeutral").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"D0").Data(), _vf});
        eventsTree->Branch((name + "D0").Data(), &(collmap_float[(name+"D0").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"ErrorD0").Data(), _vf});
        eventsTree->Branch((name + "ErrorD0").Data(), &(collmap_float[(name+"ErrorD0").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"DZ").Data(), _vf});
        eventsTree->Branch((name + "DZ").Data(), &(collmap_float[(name+"DZ").Data()]));

        _vf = new std::vector<float>();
        collmap_float.insert({(name+"ErrorDZ").Data(), _vf});
        eventsTree->Branch((name + "ErrorDZ").Data(), &(collmap_float[(name+"ErrorDZ").Data()]));
      } else if (className == "GenParticle") {
        //TODO
      } else if (className == "ScalarHT") {
        auto _vf = new std::vector<float>();
        collmap_float.insert({(name+"HT").Data(), _vf});
        eventsTree->Branch((name+"HT").Data(), &(collmap_float[(name+"HT").Data()]));
      } else if (className == "MissingET") {
        TLorentzVector* _v = new TLorentzVector();
        collmap_met.insert({name.Data(), _v});
        eventsTree->Branch(name.Data(), &(collmap_met[name.Data()]));
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
          const TObjArray* delphesColl = modularDelphes->ImportArray(input);
          if (className == "Jet") {
            edm4hep::ReconstructedParticleCollection* mcps = 
                static_cast<edm4hep::ReconstructedParticleCollection*>(collmap[name.Data()]);
            std::vector<ROOT::Math::PxPyPzEVector>* _softdropped = 
                collmap_4v[(name+"SoftDroppedJet").Data()];
            _softdropped->clear();
            std::vector<ROOT::Math::PxPyPzEVector>* _softdroppedsubjet1 = collmap_4v[(name+"SoftDroppedSubJet1").Data()];
            _softdroppedsubjet1->clear();
            std::vector<ROOT::Math::PxPyPzEVector>* _softdroppedsubjet2 = collmap_4v[(name+"SoftDroppedSubJet2").Data()];
            _softdroppedsubjet2->clear();
            std::vector<float>* _tau1 = collmap_float[(name+"Tau1").Data()];
            _tau1->clear();
            std::vector<float>* _tau2 = collmap_float[(name+"Tau2").Data()];
            _tau2->clear();
            std::vector<float>* _tau3 = collmap_float[(name+"Tau3").Data()];
            _tau3->clear();
            std::vector<float>* _tau4 = collmap_float[(name+"Tau4").Data()];
            _tau4->clear();
            std::vector<float>* _tau5 = collmap_float[(name+"Tau5").Data()];
            _tau5->clear();
            std::vector<float>* _DeltaEta = collmap_float[(name+"DeltaEta").Data()];
            _DeltaEta->clear();
            std::vector<float>* _DeltaPhi = collmap_float[(name+"DeltaPhi").Data()];
            _DeltaPhi->clear();
            for (int j = 0; j < delphesColl->GetEntries(); j++) {
              auto cand = static_cast<Candidate*>(delphesColl->At(j));
              auto mcp1 = mcps->create();
              mcp1.setMass( cand->Mass ) ;
              mcp1.setCharge( cand->Charge );
              mcp1.setMomentum( { (float) cand->Momentum.Px(), 
                                  (float) cand->Momentum.Py(),
                                  (float) cand->Momentum.Pz() }  ) ;
              _softdropped->emplace_back(cand->SoftDroppedJet.Px(), 
                                         cand->SoftDroppedJet.Py(),
                                         cand->SoftDroppedJet.Pz(),
                                         cand->SoftDroppedJet.E());

              _tau1->emplace_back(cand->Tau[0]);
              _tau2->emplace_back(cand->Tau[1]);
              _tau3->emplace_back(cand->Tau[2]);
              _tau4->emplace_back(cand->Tau[3]);
              _tau5->emplace_back(cand->Tau[4]);
              _DeltaEta->emplace_back(cand->DeltaEta);
              _DeltaPhi->emplace_back(cand->DeltaPhi);
              //TODO set particleID
              //TODO set location
              //TODO ...
            }
          } else if (className == "Photon") {
            edm4hep::ReconstructedParticleCollection* mcps =
                static_cast<edm4hep::ReconstructedParticleCollection*>(collmap[name.Data()]);
            std::vector<float>* _EhadOverEem = collmap_float[(name+"EhadOverEem").Data()];
            _EhadOverEem->clear();

            std::vector<float>* _IsolationVar = collmap_float[(name+"IsolationVar").Data()];
            _IsolationVar->clear();

            std::vector<float>* _IsolationVarRhoCorr = collmap_float[(name+"IsolationVarRhoCorr").Data()];
            _IsolationVarRhoCorr->clear();

            std::vector<float>* _SumPtCharged = collmap_float[(name+"SumPtCharged").Data()];
            _SumPtCharged->clear();

            std::vector<float>* _SumPtNeutral = collmap_float[(name+"SumPtNeutral").Data()];
            _SumPtNeutral->clear();

            std::vector<float>* _SumPtChargedPU = collmap_float[(name+"SumPtChargedPU").Data()];
            _SumPtChargedPU->clear();

            std::vector<float>* _SumPt = collmap_float[(name+"SumPt").Data()];
            _SumPt->clear();
            for (int j = 0; j < delphesColl->GetEntries(); j++) {
              auto cand = static_cast<Candidate*>(delphesColl->At(j));
              auto mcp1 = mcps->create();
              mcp1.setMass( cand->Mass ) ;
              mcp1.setCharge( cand->Charge );
              mcp1.setMomentum( { (float) cand->Momentum.Px(), 
                                  (float) cand->Momentum.Py(),
                                  (float) cand->Momentum.Pz() }  ) ;

                _EhadOverEem->emplace_back(cand->Eem > 0.0 ? cand->Ehad / cand->Eem : 999.9);
                _IsolationVar->emplace_back(cand->IsolationVar);
                _IsolationVarRhoCorr->emplace_back(cand->IsolationVarRhoCorr);
                _SumPt->emplace_back(cand->SumPt);
                _SumPtCharged->emplace_back(cand->SumPtCharged);
                _SumPtChargedPU->emplace_back(cand->SumPtChargedPU);
                _SumPtNeutral->emplace_back(cand->SumPtNeutral);
            }
          } else if (className == "Electron") {

            edm4hep::ReconstructedParticleCollection* mcps =
                static_cast<edm4hep::ReconstructedParticleCollection*>(collmap[name.Data()]);

            std::vector<float>* _IsolationVar = collmap_float[(name+"IsolationVar").Data()];
            _IsolationVar->clear();

            std::vector<float>* _IsolationVarRhoCorr = collmap_float[(name+"IsolationVarRhoCorr").Data()];
            _IsolationVarRhoCorr->clear();

            std::vector<float>* _SumPtCharged = collmap_float[(name+"SumPtCharged").Data()];
            _SumPtCharged->clear();

            std::vector<float>* _SumPtNeutral = collmap_float[(name+"SumPtNeutral").Data()];
            _SumPtNeutral->clear();

            std::vector<float>* _SumPtChargedPU = collmap_float[(name+"SumPtChargedPU").Data()];
            _SumPtChargedPU->clear();

            std::vector<float>* _SumPt = collmap_float[(name+"SumPt").Data()];
            _SumPt->clear();

            std::vector<float>* _D0 = collmap_float[(name+"D0").Data()];
            _D0->clear();

            std::vector<float>* _ErrorD0 = collmap_float[(name+"ErrorD0").Data()];
            _ErrorD0->clear();

            std::vector<float>* _DZ = collmap_float[(name+"D0").Data()];
            _DZ->clear();

            std::vector<float>* _ErrorDZ = collmap_float[(name+"ErrorD0").Data()];
            _ErrorDZ->clear();

            for (int j = 0; j < delphesColl->GetEntries(); j++) {
              auto cand = static_cast<Candidate*>(delphesColl->At(j));
              auto mcp1 = mcps->create();
              mcp1.setMass( cand->Mass ) ;
              mcp1.setCharge( cand->Charge );
              mcp1.setMomentum( { (float) cand->Momentum.Px(), 
                                  (float) cand->Momentum.Py(),
                                  (float) cand->Momentum.Pz() }  ) ;


                _IsolationVar->emplace_back(cand->IsolationVar);
                _IsolationVarRhoCorr->emplace_back(cand->IsolationVarRhoCorr);
                _SumPt->emplace_back(cand->SumPt);
                _SumPtCharged->emplace_back(cand->SumPtCharged);
                _SumPtChargedPU->emplace_back(cand->SumPtChargedPU);
                _SumPtNeutral->emplace_back(cand->SumPtNeutral);
                _D0->emplace_back(cand->D0);
                _DZ->emplace_back(cand->DZ);
                _ErrorDZ->emplace_back(cand->ErrorDZ);
                _ErrorD0->emplace_back(cand->ErrorD0);
            }
          } else if (className == "Muon") {
            edm4hep::ReconstructedParticleCollection* mcps =
                static_cast<edm4hep::ReconstructedParticleCollection*>(collmap[name.Data()]);
            std::vector<float>* _IsolationVar = collmap_float[(name+"IsolationVar").Data()];
            _IsolationVar->clear();

            std::vector<float>* _IsolationVarRhoCorr = collmap_float[(name+"IsolationVarRhoCorr").Data()];
            _IsolationVarRhoCorr->clear();

            std::vector<float>* _SumPtCharged = collmap_float[(name+"SumPtCharged").Data()];
            _SumPtCharged->clear();

            std::vector<float>* _SumPtNeutral = collmap_float[(name+"SumPtNeutral").Data()];
            _SumPtNeutral->clear();

            std::vector<float>* _SumPtChargedPU = collmap_float[(name+"SumPtChargedPU").Data()];
            _SumPtChargedPU->clear();

            std::vector<float>* _SumPt = collmap_float[(name+"SumPt").Data()];
            _SumPt->clear();

            std::vector<float>* _D0 = collmap_float[(name+"D0").Data()];
            _D0->clear();

            std::vector<float>* _ErrorD0 = collmap_float[(name+"ErrorD0").Data()];
            _ErrorD0->clear();

            std::vector<float>* _DZ = collmap_float[(name+"D0").Data()];
            _DZ->clear();

            std::vector<float>* _ErrorDZ = collmap_float[(name+"ErrorD0").Data()];
            _ErrorDZ->clear();
            for (int j = 0; j < delphesColl->GetEntries(); j++) {
              auto cand = static_cast<Candidate*>(delphesColl->At(j));
              auto mcp1 = mcps->create();
              mcp1.setMass( cand->Mass ) ;
              mcp1.setCharge( cand->Charge );
              mcp1.setMomentum( { (float) cand->Momentum.Px(), 
                                  (float) cand->Momentum.Py(),
                                  (float) cand->Momentum.Pz() }  ) ;

                _IsolationVar->emplace_back(cand->IsolationVar);
                _IsolationVarRhoCorr->emplace_back(cand->IsolationVarRhoCorr);
                _SumPt->emplace_back(cand->SumPt);
                _SumPtCharged->emplace_back(cand->SumPtCharged);
                _SumPtChargedPU->emplace_back(cand->SumPtChargedPU);
                _SumPtNeutral->emplace_back(cand->SumPtNeutral);
                _D0->emplace_back(cand->D0);
                _DZ->emplace_back(cand->DZ);
                _ErrorDZ->emplace_back(cand->ErrorDZ);
                _ErrorD0->emplace_back(cand->ErrorD0);
            }
          } else if (className == "ScalarHT") {
            std::vector<float>* _ScalarHTHT = collmap_float[(name+"HT").Data()];
            _ScalarHTHT->clear();
            auto cand = static_cast<Candidate*>(delphesColl->At(0));
            _ScalarHTHT->emplace_back(cand->Momentum.Pt());
          } else if (className == "MissingET") {
            // there will only ever be one element in this array
            // no need to save it as a container 
            auto cand = static_cast<Candidate*>(delphesColl->At(0));
            collmap_met[name.Data()] = &(cand->Momentum);
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

