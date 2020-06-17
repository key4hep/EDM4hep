// Make sure these can all be found on LD_LIBRARY_PATH
#ifdef __CLING__
R__LOAD_LIBRARY(libpodio)
R__LOAD_LIBRARY(libpodioDict)
R__LOAD_LIBRARY(libpodioRootIO)
R__LOAD_LIBRARY(libedm4hep)
R__LOAD_LIBRARY(libedm4hepDict)
#endif

#include "AssociationHelper.h"
#include "common_helpers.h"

// make sure that the EDM4hep include directory is in ROOT_INCLUDE_PATH
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/MCRecoParticleAssociationCollection.h"
#include "edm4hep/RecoParticleRefCollection.h"

// also make sure that the podio include directory is in ROOT_INCLUDE_PATH
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

#include "Math/Vector4D.h"

#include <string>
#include <iostream>
#include <cmath>

using LorentzVectorT = ROOT::Math::PxPyPzEVector;

struct BasicInfo {
    double pt;
    double E;
};


LorentzVectorT getGen4Momentum(edm4hep::ConstReconstructedParticle recoCand,
                            MCRecoParticleAssociationNavigator const& relationNavigator)
{
    auto mcParticles = relationNavigator.getSim(recoCand);

    // for Jets the MC Particles are not attached to the jet
    // ReconstructedParticle but instead to the constituents
    if (mcParticles.empty()) {
        for (auto constIt = recoCand.particles_begin(); constIt != recoCand.particles_end(); ++constIt) {
            const auto constMCParticles = relationNavigator.getSim(*constIt);
            for (const auto& mcParticle : constMCParticles) {
                mcParticles.push_back(mcParticle);
            }
        }
    }

    LorentzVectorT momentum{};
    // NOTE: for the moment using all the related particles here. This should
    // not make a difference for the current implementation of the conversion
    // since Jets should be the only ones which possibly have more then one
    // related MCParticle
    for (const auto& mcParticle : mcParticles) {
        const auto& partMom = mcParticle.getMomentum();
        LorentzVectorT tmp;
        tmp.SetCoordinates(partMom[0], partMom[1], partMom[2], mcParticle.getEnergy());
        momentum += tmp;
    }

    return momentum;
}

BasicInfo getDeltaInfo(edm4hep::ConstReconstructedParticle recoCand,
                       MCRecoParticleAssociationNavigator const& relationNavigator) {

    const auto gen4Momentum = getGen4Momentum(recoCand, relationNavigator);
    if (gen4Momentum == LorentzVectorT()) {
        return {-1, 1};
    }

    const auto& recoMomentum = recoCand.getMomentum();
    const double pt = std::sqrt(recoMomentum[0] * recoMomentum[0] + recoMomentum[1] * recoMomentum[1]);

    return BasicInfo{pt - gen4Momentum.Pt(), recoCand.getEnergy() - gen4Momentum.E()};
}


void fillHists(TH1F* hDeltaPt, TH1F* hDeltaE, edm4hep::ConstReconstructedParticle recoCand,
               MCRecoParticleAssociationNavigator const& relationNavigator) {
    const auto deltaInfo = getDeltaInfo(recoCand, relationNavigator);
    hDeltaPt->Fill(deltaInfo.pt);
    hDeltaE->Fill(deltaInfo.E);
}


void read_edm4hep(std::string&& inputfile) {
    gSystem->Load("libpodio.so");
    gSystem->Load("libpodioDict.so");
    gSystem->Load("libpodioRootIO.so");
    gSystem->Load("libedm4hep.so");
    gSystem->Load("libedm4hepDict.so");

    auto reader = podio::ROOTReader();
    reader.openFile(inputfile);
    auto store = podio::EventStore();
    store.setReader(&reader);

    defineHists();

    const auto nEntries = reader.getEntries();
    for (int entry = 0; entry < nEntries; ++entry) {
        auto& genParticles = store.get<edm4hep::MCParticleCollection>("Particle");
        auto& recoParticles = store.get<edm4hep::ReconstructedParticleCollection>("ReconstructedParticles");
        auto& jets = store.get<edm4hep::ReconstructedParticleCollection>("Jet");

        auto& muons = store.get<edm4hep::RecoParticleRefCollection>("Muon");
        auto& electrons = store.get<edm4hep::RecoParticleRefCollection>("Electron");
        auto& photons = store.get<edm4hep::RecoParticleRefCollection>("Photon");
       
        // auto& tracks = store.get<edm4hep::TrackCollection>("EFlowTrack");
        // auto& ecalClusters = store.get<edm4hep::ClusterCollection>("EFlowPhoton");
        // auto& hcalClusters = store.get<edm4hep::ClusterCollection>("EFlowNeutralHadron");

        auto& recoMCAssociations = store.get<edm4hep::MCRecoParticleAssociationCollection>("MCRecoAssociations");
        MCRecoParticleAssociationNavigator mcRecoNavigator(recoMCAssociations);


        for (const auto& muonRef : muons) {
            const auto& muon = muonRef.getParticle();
            fillHists(muonDeltaPt, muonDeltaE, muon, mcRecoNavigator);
        }

        for (const auto& electronRef : electrons) {
            const auto& electron = electronRef.getParticle();
            fillHists(electronDeltaPt, electronDeltaE, electron, mcRecoNavigator);
        }

        for (const auto& photonRef : photons) {
            const auto& photon = photonRef.getParticle();
            fillHists(photonDeltaPt, photonDeltaE, photon, mcRecoNavigator);
        }

        for (const auto& jet : jets) {
            // TODO: Jets can now no longer as easily get their generated
            // particles. Need to work on that first before these examples work
            // again
            fillHists(jetDeltaPt, jetDeltaE, jet, mcRecoNavigator);

            const auto gen4Momentum = getGen4Momentum(jet, mcRecoNavigator);
            jetRecoE->Fill(jet.getEnergy());
            jetGenE->Fill(gen4Momentum.E());
            jetRecoM->Fill(jet.getMass());
            jetGenM->Fill(gen4Momentum.M());

            int nTracks = 0, nClusters = 0;
            for (auto constIt = jet.particles_begin(); constIt != jet.particles_end(); ++constIt) {
                nTracks += std::distance(constIt->tracks_begin(), constIt->tracks_end());
                nClusters += std::distance(constIt->clusters_begin(), constIt->clusters_end());
            }
            jetNTracks->Fill(nTracks);
            jetNClusters->Fill(nClusters);

            jetNConstituents->Fill(std::distance(jet.particles_begin(), jet.particles_end()));
        }

        // std::unordered_map<int, int> countMap;
        // for (const auto& mcPart : genParticles) {
        //     const auto recoParticles = mcRecoNavigator.getRec(mcPart);
        //     for (const auto& recoPart : recoParticles) {
        //         countMap[recoPart.getType()]++;
        //     }
        // }

        // std::cout << "jets: " << jets.size() << ", "
        //           << " photons: " << photons.size()
        //           << " electrons: " << electrons.size()
        //           << " muons: " << muons.size()
        //           << " relations: " << recoMCAssociations.size() << "\n";

        // for (const auto& types : countMap) {
        //     std::cout << types.first << ": " << types.second << "\n";
        //     typeCountMap[types.first]->Fill(types.second);
        // }


        store.clear();
        reader.endOfEvent();
    }

    storeHists("histograms_edm4hep.root");
}
