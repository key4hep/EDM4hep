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
#include "podio/ROOTWriter.h"
#include "podio/EventStore.h"


#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/ParticleIDCollection.h"
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/MCRecoParticleAssociationCollection.h"

#include "DelphesRootReader.h"


using std::cout;
using std::cerr;
using std::endl;

// gracefully handle ctrl+c
static bool interrupted = false;
void SignalHandler(int sig) {
  interrupted = true;
}

/**
 * To fill the basic information that is shared by all edm4hep types
 */
template <typename EDM4hepT>
EDM4hepT fromDelphesBase(Candidate* delphesCand) {
  EDM4hepT cand;
  cand.setCharge(delphesCand->Charge);
  // TODO: Delphes deals with masses a bit strangely. For the Muons and
  // Electrons it is set at the Candidate level (i.e. before TreeWriter) but for
  // Jets it is not. On the other hand, at the level of the classes that are
  // written out by the TreeWriter, it doesn't store the masses for the Muons
  // and Electrons, but it computes the Jet masses from the 4-momentum and
  // stores that. Additionally, when getting the 4-vector via Muon::P4() or
  // Electron::P4() delphes sets the masses to 0, but uses the Jet::Mass
  // member in the case of Jet::P4()
  // See: https://github.com/delphes/delphes/blob/master/classes/DelphesClasses.cc#L84-L98
  //
  // The consequence is that Muons and Electrons stored as
  // edm4hep::ReconstructedParticle will currently have non-zero masses, whereas
  // Jets will have zero masses. Delphes outputs on the other hand will have
  // zero masses for the Muons and Electrons, but non-zero masses for the Jets
  // (i.e. reversed to edm4hep).
  //
  // This mainly makes a difference for the Energy computation of the
  // reconstructed Jets, as can be seen by comparing the histograms produced by
  // examples/read_{delphes,edm4hep}.C. For the Muons and Electrons the masses
  // are negligible enough for the total energy that no difference arises.
  cand.setMass(delphesCand->Mass);
  cand.setMomentum({
    (float) delphesCand->Momentum.Px(),
    (float) delphesCand->Momentum.Py(),
    (float) delphesCand->Momentum.Pz()
  });

  return cand;
}

// Following what is done in TreeWriter::FillParticles
std::vector<UInt_t> findParticles(Candidate* candidate) {
  std::vector<UInt_t> relatedParticles;
  TIter it1(candidate->GetCandidates());
  it1.Reset();

  while((candidate = static_cast<Candidate*>(it1.Next()))) {
    TIter it2(candidate->GetCandidates());

    // particle
    if (candidate->GetCandidates()->GetEntriesFast() == 0) {
      relatedParticles.push_back(candidate->GetUniqueID());
      continue;
    }

    // track
    candidate = static_cast<Candidate*>(candidate->GetCandidates()->At(0));
    if (candidate->GetCandidates()->GetEntriesFast() == 0) {
      relatedParticles.push_back(candidate->GetUniqueID());
      continue;
    }

    // tower
    it2.Reset();
    while((candidate = static_cast<Candidate*>(it2.Next()))) {
      relatedParticles.push_back(candidate->GetCandidates()->At(0)->GetUniqueID());
    }
  }

  return relatedParticles;
}

int findPhotonMC(Candidate* candidate) {
  const auto relatedParticles = findParticles(candidate);
  // If there is only one relation, assume that it is the generated particle and
  // return the ID. Otherwise return -1 to signal that this was not the case
  if (relatedParticles.size() == 1) {
    return relatedParticles[0];
  }

  std::cout << "Found " << relatedParticles.size() << " candidates related to the photon" << std::endl;

  return -1;
}


void printCandidates(TObjArray* candArray)
{
  Candidate* candidate = nullptr;
  TIter candidatesIt(candArray);
  candidatesIt.Reset();


  std::cout << "*** Related candidates as they appear without sorting\n";
  while((candidate = static_cast<Candidate*>(candidatesIt.Next()))) {
    const TLorentzVector& momentum = candidate->Momentum;
    std::cout << " UniqueID: " << candidate->GetUniqueID() << ", "
              << "(Px, Py, Pz) = " << "(" << momentum.Px() << ", " << momentum.Py() << ", " << momentum.Pz() << "), "
              << "E = " << momentum.E() << ", M = " << momentum.M() << "\n";
  }
}



// main function with generic input
int doit(int argc, char *argv[], DelphesInputReader& inputReader) {
  std::string appName = "DelphesROOT_EDM4HEP";
  Delphes* modularDelphes = new Delphes("Delphes");
  std::string outputfile;
  inputReader.init(modularDelphes,argc, argv, outputfile);
  Long64_t eventCounter, numberOfEvents;
  // gracefully handle ctrl+c
  signal(SIGINT, SignalHandler);
  try {
    podio::EventStore store;
    podio::ROOTWriter  writer(outputfile, &store);

    auto confReader = std::make_unique<ExRootConfReader>();
    confReader->ReadFile(argv[1]);

    // todo: ROOT error on 6.20.04 if this is a unique pointer
    modularDelphes->SetConfReader(confReader.get());

    ExRootConfParam branches = confReader->GetParam("TreeWriter::Branch");
    int maxEvents = confReader->GetInt("::MaxEvents", 0);



    int nParams = branches.GetSize();

    std::unordered_map<std::string, podio::CollectionBase*> collmap;
    edm4hep::ReconstructedParticleCollection* _col;
    for(int b = 0; b < nParams; b += 3) {
      TString input = branches[b].GetString();
      TString name = branches[b + 1].GetString();
      TString className = branches[b + 2].GetString();
      std::string _name;
      //std::cout <<  input << "\t" << name << "\t" << className << std::endl;
      // classes that are to be translated to a Reconstructed Particle
      if (className == "Jet") {

        // convert TString to std::string
        _name = name.Data();
        store.create<edm4hep::ReconstructedParticleCollection>(_name);
        writer.registerForWrite(_name);
        store.get(_name, _col);
        collmap.insert({_name, _col});

        _name = (name + "SubJets").Data();
        store.create<edm4hep::ReconstructedParticleCollection>(_name);
        writer.registerForWrite(_name);
        store.get(_name, _col);
        collmap.insert({_name, _col});

        _name = (name + "ParticleIDs").Data();
        store.create<edm4hep::ParticleIDCollection>(_name);
        writer.registerForWrite(_name);
        store.get(_name, _col);
        collmap.insert({_name, _col});


      } else if (className == "Photon" || className == "Electron" || className == "Muon") {

        _name = name.Data();
        store.create<edm4hep::ReconstructedParticleCollection>(_name);
        writer.registerForWrite(_name);
        store.get(_name, _col);
        collmap.insert({_name, _col});

        _name = (name + "ParticleIDs").Data();
        store.create<edm4hep::ParticleIDCollection>(_name);
        writer.registerForWrite(_name);
        store.get(_name, _col);
        collmap.insert({_name, _col});


      } else if (className == "GenParticle") {
        // see below
      } else if (className == "ScalarHT") {
        _name = name.Data();
        store.create<edm4hep::ParticleIDCollection>(_name);
        writer.registerForWrite(_name);
        store.get(_name, _col);
        collmap.insert({_name, _col});
      } else if (className == "MissingET") {
        _name = name.Data();
        store.create<edm4hep::ReconstructedParticleCollection>(_name);
        writer.registerForWrite(_name);
        store.get(_name, _col);
        collmap.insert({_name, _col});
      }

    }

    auto& mcParticleCollection = store.create<edm4hep::MCParticleCollection>("MCParticles");
    writer.registerForWrite("MCParticles");
    store.get("MCParticles", _col);
    collmap.insert({"MCParticles", _col});
    auto& mcRecoAssociationCollection = store.create<edm4hep::MCRecoParticleAssociationCollection>("MCRecoAssociations");
    writer.registerForWrite("MCRecoAssociations");
    store.get("MCRecoAssociations", _col);
    collmap.insert({"MCRecoAssociations", _col});

    // has to happen before InitTask
    TObjArray* allParticleOutputArray = modularDelphes->ExportArray("allParticles");
    TObjArray* stableParticleOutputArray = modularDelphes->ExportArray("stableParticles");
    TObjArray* partonOutputArray = modularDelphes->ExportArray("partons");

    modularDelphes->InitTask();

    ExRootProgressBar progressBar(-1);
    // Loop over all objects
    eventCounter = 0;
    modularDelphes->Clear();


    for (Int_t entry = 0;
         inputReader.finished() && maxEvents > 0 ?  entry < maxEvents : true && !interrupted; 
         ++entry) {
      
      bool success = inputReader.readEvent(modularDelphes, 
                                           allParticleOutputArray, 
                                           stableParticleOutputArray, 
                                           partonOutputArray);
      if (!success) {
        break;
      }

      modularDelphes->ProcessTask();

      // collect all associations and process them after all Delphes candidates
      // have been stored
      std::vector<std::pair<UInt_t, edm4hep::ConstReconstructedParticle>> mcParticleRelations;
      std::unordered_map<UInt_t, edm4hep::ConstMCParticle> mcParticleIds;


        unsigned int collcounter = 0;
        for(int b = 0; b < nParams; b += 3) {
          TString input = branches[b].GetString();
          TString name = branches[b + 1].GetString();
          TString className = branches[b + 2].GetString();
          //std::cout << input << "\t" << name << "\t" << className << std::endl;
          const TObjArray* delphesColl = modularDelphes->ImportArray(input);
          std::string _name = name.Data();
          if (className == "Jet") {
            edm4hep::ReconstructedParticleCollection* mcps = 
                static_cast<edm4hep::ReconstructedParticleCollection*>(collmap[_name]);

            // subjets
            edm4hep::ReconstructedParticleCollection* subjetcoll = 
                static_cast<edm4hep::ReconstructedParticleCollection*>(collmap[_name + "SubJets"]);

            edm4hep::ParticleIDCollection* idcoll = 
                static_cast<edm4hep::ParticleIDCollection*>(collmap[_name + "ParticleIDs"]);

            for (int j = 0; j < delphesColl->GetEntries(); j++) {
              auto cand = static_cast<Candidate*>(delphesColl->At(j));
              auto mcp1 = mcps->create();
              mcp1.setMass( cand->Mass ) ;
              mcp1.setCharge( cand->Charge );
              mcp1.setMomentum( { (float) cand->Momentum.Px(), 
                                  (float) cand->Momentum.Py(),
                                  (float) cand->Momentum.Pz() }  ) ;

              for (const auto delphesId : findParticles(cand)) {
                mcParticleRelations.emplace_back(delphesId, mcp1);
              }

              auto ids = idcoll->create();
              // T calculated as in Delphes TreeWriter
              ids.addToParameters(cand->Position.T() * 1.0E-3  / 2.99792458E8 ); 
              ids.addToParameters(cand->DeltaEta);
              ids.addToParameters(cand->DeltaPhi);
              ids.addToParameters(cand->DeltaPhi);
              ids.addToParameters(cand->Flavor);
              ids.addToParameters(cand->FlavorAlgo);
              ids.addToParameters(cand->FlavorPhys);
              ids.addToParameters(cand->BTag);
              ids.addToParameters(cand->BTagAlgo);
              ids.addToParameters(cand->BTagPhys);
              ids.addToParameters(cand->TauTag);
              ids.addToParameters(cand->TauWeight);

              TIter itConstituents(cand->GetCandidates());
              itConstituents.Reset();
              double ecalEnergy = 0.0;
              double hcalEnergy = 0.0;
              Candidate* constituent = 0; 
              while((constituent = static_cast<Candidate *>(itConstituents.Next()))) {
                // todo:
                ////entry->Constituents.Add(constituent);
                ecalEnergy += constituent->Eem;
                hcalEnergy += constituent->Ehad;
              }
              float EhadOverEem = ecalEnergy > 0.0 ? hcalEnergy / ecalEnergy : 999.9;
              ids.addToParameters(EhadOverEem);

              auto area = subjetcoll->create();
              area.setMomentum( { (float) cand->Area.Px(), 
                                     (float) cand->Area.Py(),
                                     (float) cand->Area.Pz() }  ) ;
              area.setMass( cand->Area.M());
              mcp1.addToParticles(area);

              auto subjet1 = subjetcoll->create();
              subjet1.setMomentum( { (float) cand->SoftDroppedJet.Px(), 
                                     (float) cand->SoftDroppedJet.Py(),
                                     (float) cand->SoftDroppedJet.Pz() }  ) ;
              subjet1.setMass( cand->SoftDroppedJet.M());
              mcp1.addToParticles(subjet1);
              auto subjet2 = subjetcoll->create();
              subjet2.setMomentum( { (float) cand->SoftDroppedSubJet1.Px(), 
                                     (float) cand->SoftDroppedSubJet1.Py(),
                                     (float) cand->SoftDroppedSubJet1.Pz() }  ) ;
              subjet2.setMass( cand->SoftDroppedSubJet1.M());
              mcp1.addToParticles(subjet2);
              auto subjet3 = subjetcoll->create();
              subjet3.setMomentum( { (float) cand->SoftDroppedSubJet2.Px(), 
                                     (float) cand->SoftDroppedSubJet2.Py(),
                                     (float) cand->SoftDroppedSubJet2.Pz() }  ) ;
              subjet3.setMass( cand->SoftDroppedSubJet2.M());
              mcp1.addToParticles(subjet3);
            }

          } else if (className == "Photon" || className == "Electron" || className == "Muon") {
            edm4hep::ReconstructedParticleCollection* mcps =
                static_cast<edm4hep::ReconstructedParticleCollection*>(collmap[name.Data()]);
            edm4hep::ParticleIDCollection* idcoll = 
                static_cast<edm4hep::ParticleIDCollection*>(collmap[_name + "ParticleIDs"]);

            for (int j = 0; j < delphesColl->GetEntries(); j++) {
              auto cand = static_cast<Candidate*>(delphesColl->At(j));
              auto mcp1 = mcps->create();
              if (className == "Photon") {
                auto mcId = findPhotonMC(cand);
                if (mcId > -1) {
                  mcParticleRelations.emplace_back(mcId, mcp1);
                }
              } else {
                auto* genCand = static_cast<Candidate*>(cand->GetCandidates()->At(0));
                mcParticleRelations.emplace_back(genCand->GetUniqueID(), mcp1);
              }

              // const auto& mom = cand->Momentum;
              // std::cout << className << ": UniqueId: " << cand->GetUniqueID()
              //           << "(Px, Py, Pz) = (" << mom.Px() << ", " << mom.Py() << ", " << mom.Pz() << ")"
              //           << " | total related: "  << cand->GetCandidates()->GetEntries() << std::endl;
              // printCandidates(cand->GetCandidates());
              mcp1.setMass( cand->Mass );
              mcp1.setCharge( cand->Charge );
              mcp1.setMomentum( { (float) cand->Momentum.Px(), 
                                  (float) cand->Momentum.Py(),
                                  (float) cand->Momentum.Pz() }  ) ;

              auto ids = idcoll->create();
              ids.addToParameters(cand->DeltaEta);

              double EhadOverEem = cand->Eem > 0.0 ? cand->Ehad / cand->Eem : 999.9;
              ids.addToParameters(EhadOverEem);
              ids.addToParameters(cand->IsolationVar);
              ids.addToParameters(cand->IsolationVarRhoCorr);
              ids.addToParameters(cand->SumPt);
              ids.addToParameters(cand->SumPtCharged);
              ids.addToParameters(cand->SumPtChargedPU);
              ids.addToParameters(cand->SumPtNeutral);
            }
          } else if (className == "ScalarHT") {
            edm4hep::ParticleIDCollection* mcps =
                static_cast<edm4hep::ParticleIDCollection*>(collmap[name.Data()]);
            auto mcp1 = mcps->create();
            auto cand = static_cast<Candidate*>(delphesColl->At(0));
            mcp1.addToParameters(cand->Momentum.Pt());
          } else if (className == "MissingET") {
            edm4hep::ReconstructedParticleCollection* mcps =
                static_cast<edm4hep::ReconstructedParticleCollection*>(collmap[name.Data()]);
            auto cand = static_cast<Candidate*>(delphesColl->At(0));
             auto mcp1 = mcps->create();
             mcp1.setMass( cand->Mass ) ;
              mcp1.setMomentum( { (float) cand->Momentum.Px(), 
                                  (float) cand->Momentum.Py(),
                                  (float) cand->Momentum.Pz() }  ) ;
          } else if (className == "GenParticle") {
              for (int iCand = 0; iCand < delphesColl->GetEntriesFast(); ++iCand) {
              auto* delphesCand = static_cast<Candidate*>(delphesColl->At(iCand));
              auto cand = fromDelphesBase<edm4hep::MCParticle>(delphesCand);
              cand.setVertex({(float) delphesCand->Position.X(),
                              (float) delphesCand->Position.Y(),
                              (float) delphesCand->Position.Z()});
              cand.setPDG(delphesCand->PID); // delphes uses whatever hepevt.idhep provides
              mcParticleCollection.push_back(cand);
              // TODO: - status
              // TODO: - ...

              mcParticleIds.emplace(delphesCand->GetUniqueID(), cand);
              // std::cout << "MC Particle, UniqueID: " << delphesCand->GetUniqueID() <<", "
              //           << "(Px, Py, Pz) = (" << delphesCand->Momentum.Px() << ", "
              //           << delphesCand->Momentum.Py() << ", " << delphesCand->Momentum.Pz() << "), "
              //           << "E = " << delphesCand->Momentum.E() << ", M = " << delphesCand->Momentum.M() << "\n";
            }

          }
        }
             // should technically be a set, but for now we will just emulate that
        std::vector<UInt_t> usedIds;
        std::vector<UInt_t> notFoundIds;

        // now register the MC <-> reco associations
        for (const auto& delphesRelation : mcParticleRelations) {
          const auto mcIt = mcParticleIds.find(delphesRelation.first);
          if (mcIt != mcParticleIds.end()) {
            auto relation = mcRecoAssociationCollection.create();
            relation.setRec(delphesRelation.second);
            relation.setSim(mcIt->second);
            if (std::find(usedIds.cbegin(), usedIds.cend(), delphesRelation.first) == usedIds.cend()) {
              usedIds.push_back(delphesRelation.first);
            }
          } else {
            std::cerr << "WARNING: delphes candidate had relation to candidate that "
                      << "was not part of the GenParticle collection (UniqueId = "
                      << delphesRelation.first << "). Not registering "
                      << "this relation" << std::endl;
            notFoundIds.push_back(delphesRelation.first);
          }

          // // get all Ids here and remove the found ones to arrive at the unused ones
          // std::vector<UInt_t> unusedIds;
          // for (const auto& mcId : mcParticleIds) {
          //   if (std::find(usedIds.cbegin(), usedIds.cend(), mcId.first) == usedIds.cend()) {
          //     unusedIds.push_back(mcId.first);
          //   }
          // }

          // std::cout << "Registered " << mcRecoAssociationCollection.size() << " relations. "
          //           << "used " << usedIds.size() << " unique MC Particles and found "
          //           << notFoundIds.size() << " Delphes candidates that were not in the GenParticle array\n";

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

