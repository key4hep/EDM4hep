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
#include "edm4hep/TrackCollection.h"

#include "DelphesRootReader.h"
#include "delphesHelpers.h"

using std::cout;
using std::cerr;
using std::endl;

// gracefully handle ctrl+c
static bool interrupted = false;
void SignalHandler(int sig) {
  interrupted = true;
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
  if(!inputReader.init(modularDelphes,argc, argv, outputfile)) {
    return 1;
  }
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

    // Need to get the magnetic field value for calculating the omega track
    // parameter.
    const double magFieldBz = confReader->GetDouble("ParticlePropagator::Bz", 0);

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
      if (className == "Jet" || className == "Photon" || className == "Electron" || className == "Muon") {
        // convert TString to std::string
        _name = name.Data();
        store.create<edm4hep::ReconstructedParticleCollection>(_name);
        writer.registerForWrite(_name);
        store.get(_name, _col);
        collmap.insert({_name, _col});

        _name = name + "MCAssociation";
        store.create<edm4hep::MCRecoParticleAssociationCollection>(_name);
        writer.registerForWrite(_name);
        edm4hep::MCRecoParticleAssociationCollection* col;
        store.get(_name, _col);
        collmap.insert({_name, _col});

        _name = (name + "ParticleIDs").Data();
        store.create<edm4hep::ParticleIDCollection>(_name);
        writer.registerForWrite(_name);
        store.get(_name, _col);
        collmap.insert({_name, _col});

        if (className == "Jet") {
          _name = (name + "SubJets").Data();
          store.create<edm4hep::ReconstructedParticleCollection>(_name);
          writer.registerForWrite(_name);
          store.get(_name, _col);
          collmap.insert({_name, _col});
        }


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
      } else if (className == "Track") {
        store.create<edm4hep::TrackCollection>(name.Data());
        writer.registerForWrite(name.Data());
        edm4hep::TrackCollection* col;
        store.get(name.Data(), col);
        collmap.emplace(name.Data(), col);
      }

    }

    auto& mcParticleCollection = store.create<edm4hep::MCParticleCollection>("MCParticles");
    writer.registerForWrite("MCParticles");
    store.get("MCParticles", _col);
    collmap.insert({"MCParticles", _col});


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
         inputReader.finished() && (maxEvents > 0 ?  entry < maxEvents : true) && !interrupted;
         ++entry) {
      
      bool success = inputReader.readEvent(modularDelphes, 
                                           allParticleOutputArray, 
                                           stableParticleOutputArray, 
                                           partonOutputArray);
      if (!success) {
        break;
      }

      modularDelphes->ProcessTask();

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
              mcp1.setCharge( cand->Charge );
              mcp1.setMomentum( { (float) cand->Momentum.Px(), 
                                  (float) cand->Momentum.Py(),
                                  (float) cand->Momentum.Pz() }  ) ;
              // set mass after momentum has been set to ensure consistent
              // storage of energy
              //
              // NOTE: In the case of the jets, Delphes has the Candidate::Mass
              // member set to 0 (in contrast to Muons, Electrons, ...) and
              // instead calculates the mass from the 4-momentum in the
              // TreeWriter
              mcp1.setMass( cand->Momentum.M() ) ;

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

              // const auto& mom = cand->Momentum;
              // std::cout << className << ": UniqueId: " << cand->GetUniqueID()
              //           << "(Px, Py, Pz) = (" << mom.Px() << ", " << mom.Py() << ", " << mom.Pz() << ")"
              //           << " | total related: "  << cand->GetCandidates()->GetEntries() << std::endl;
              // printCandidates(cand->GetCandidates());
              mcp1.setCharge( cand->Charge );
              mcp1.setMomentum( { (float) cand->Momentum.Px(), 
                                  (float) cand->Momentum.Py(),
                                  (float) cand->Momentum.Pz() }  ) ;
              // need to set the mass after the momentum in order to calculate
              // the energy correctly internally
              mcp1.setMass( cand->Mass );


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
              auto cand = mcParticleCollection.create();
              cand.setCharge(delphesCand->Charge);
              cand.setMass(delphesCand->Mass);
              cand.setMomentum({
                (float) delphesCand->Momentum.Px(),
                (float) delphesCand->Momentum.Py(),
                (float) delphesCand->Momentum.Pz()
              });
              cand.setVertex({(float) delphesCand->Position.X(),
                              (float) delphesCand->Position.Y(),
                              (float) delphesCand->Position.Z()});
              cand.setPDG(delphesCand->PID); // delphes uses whatever hepevt.idhep provides
              // TODO: - status
              // TODO: - ...

              // std::cout << "MC Particle, UniqueID: " << delphesCand->GetUniqueID() <<", "
              //           << "(Px, Py, Pz) = (" << delphesCand->Momentum.Px() << ", "
              //           << delphesCand->Momentum.Py() << ", " << delphesCand->Momentum.Pz() << "), "
              //           << "E = " << delphesCand->Momentum.E() << ", M = " << delphesCand->Momentum.M() << "\n";
            }
          } else if (className == "Track") {
            for (int iCand = 0; iCand < delphesColl->GetEntriesFast(); ++iCand) {
              auto* trackCollection = static_cast<edm4hep::TrackCollection*>(collmap[name.Data()]);
              auto* delphesCand = static_cast<Candidate*>(delphesColl->At(iCand));
              // Delphes does not really provide any information that would go
              // into the track itself
              auto track = trackCollection->create();
              // But some information can be used to at least partially populate
              // a TrackState
              edm4hep::TrackState trackState{};
              trackState.D0 = delphesCand->D0;
              trackState.Z0 = delphesCand->DZ;

              // Delphes calculates this from the momentum 4-vector at the track
              // perigee so this should be what we want. Note that this value
              // only undergoes smearing in the TrackSmearing module but e.g.
              // not in the MomentumSmearing module
              trackState.phi = delphesCand->Phi;
              // Same thing under different name in Delphes
              trackState.tanLambda = delphesCand->CtgTheta;
              // Only do omega when there is actually a magnetic field.
              double varOmega = 0;
              if (magFieldBz) {
                // conversion to have omega in [1/mm]
                // TODO: define this globally somewhere?
                constexpr double cLight = 299792458;
                constexpr double a = cLight * 1e3 * 1e-15;

                trackState.omega = a * magFieldBz / delphesCand->PT * std::copysign(1.0, delphesCand->Charge);
                // calculate variation using simple error propagation, assuming
                // constant B-field -> relative error on pT is relative error on omega
                varOmega = delphesCand->ErrorPT * delphesCand->ErrorPT / delphesCand->PT / delphesCand->PT * trackState.omega * trackState.omega;
              }

              // fill the covariance matrix. Indices on the diagonal are 0, 5,
              // 9, 12, and 14, corresponding to D0, phi, omega, Z0 and
              // tan(lambda) respectively. Currently Delphes doesn't provide
              // correlations
              auto& covMatrix = trackState.covMatrix;
              covMatrix[0] = delphesCand->ErrorD0 * delphesCand->ErrorD0;
              covMatrix[5] = delphesCand->ErrorPhi * delphesCand->ErrorPhi;
              covMatrix[9] = varOmega;
              covMatrix[12] = delphesCand->ErrorDZ * delphesCand->ErrorDZ;
              covMatrix[14] = delphesCand->ErrorCtgTheta * delphesCand->ErrorCtgTheta;

              track.addToTrackStates(trackState);
            }
          }
        }



        // TODO: When and how do we decide which of the delphes track arrays
        // that we use?
        DelphesUniqueIDGenMatcher delphesTrackMatcher(
          modularDelphes->ImportArray("HCal/eflowTracks"), trivialGenID);
        auto* trackCollection = static_cast<edm4hep::TrackCollection*>(collmap["EFlowTrack"]);

        // muon tracks
        const auto muonTrkMatchIndices = delphesTrackMatcher.getMatchingIndices(modularDelphes->ImportArray("UniqueObjectFinder/muons"));
        auto* muonCollection = static_cast<edm4hep::ReconstructedParticleCollection*>(collmap["Muon"]);

        for (const auto indices : muonTrkMatchIndices) {
          auto track = trackCollection->at(indices.first);
          auto muon = muonCollection->at(indices.second);
          muon.addToTracks(track);
        }

        // electron tracks
        const auto eleTrkMatchIndices = delphesTrackMatcher.getMatchingIndices(modularDelphes->ImportArray("UniqueObjectFinder/electrons"));
        auto* electronCollection = static_cast<edm4hep::ReconstructedParticleCollection*>(collmap["Electron"]);

        for (const auto indices : eleTrkMatchIndices) {
          auto track = trackCollection->at(indices.first);
          auto electron = electronCollection->at(indices.second);
          electron.addToTracks(track);
        }

        auto* delphesJetCollection = modularDelphes->ImportArray("UniqueObjectFinder/jets");
        auto* jetCollection = static_cast<edm4hep::ReconstructedParticleCollection*>(collmap["Jet"]);

        for (int iJet = 0; iJet < delphesJetCollection->GetEntriesFast(); ++iJet) {
          auto* delphesJet = static_cast<Candidate*>(delphesJetCollection->At(iJet));
          auto jet = jetCollection->at(iJet);

          const auto jetTrkMatchIndices = delphesTrackMatcher.getMatchingIndices(delphesJet->GetCandidates());
          for (const auto indices : jetTrkMatchIndices) {
            auto track = trackCollection->at(indices.first);
            jet.addToTracks(track);
          }
        }

        // TODO: Get the array name from the Delphes card?
        DelphesUniqueIDGenMatcher genParticleMatcher(modularDelphes->ImportArray("Delphes/allParticles"));

        // TODO: this should be defined outside of the event loop
        const std::map<TString, AllUniqueIdsF> uniqueIdGetterF = {
        {"Jet", getAllParticleIDs},
        {"Photon", getAllParticleIDs},
        {"Muon", getTrivialGenID},
        {"Electron", getTrivialGenID}
        };

        for (int b = 0; b < nParams; b += 3) {
          TString input = branches[b].GetString();
          TString name = branches[b + 1].GetString();
          TString className = branches[b + 2].GetString();

          const auto it = uniqueIdGetterF.find(className);
          if (it != uniqueIdGetterF.end()) {
            auto* edm4hepColl = static_cast<edm4hep::ReconstructedParticleCollection*>(collmap[name.Data()]);
            auto* associationColl = static_cast<edm4hep::MCRecoParticleAssociationCollection*>(collmap[(name + "MCAssociation").Data()]);
            const auto matchIndices = genParticleMatcher.getMatchingIndices(
              modularDelphes->ImportArray(input), it->second);

            for (const auto& indices : matchIndices) {
              auto relation = associationColl->create();
              relation.setSim(mcParticleCollection.at(indices.first));
              relation.setRec(edm4hepColl->at(indices.second));
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

