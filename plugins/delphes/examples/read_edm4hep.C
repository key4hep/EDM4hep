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


LorentzVectorT getGen4Momentum(edm4hep::ReconstructedParticle const& recoCand,
                            MCRecoParticleAssociationNavigator const& relationNavigator)
{
    const auto mcParticles = relationNavigator.getSim(recoCand);

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

BasicInfo getDeltaInfo(edm4hep::ReconstructedParticle const& recoCand,
                       MCRecoParticleAssociationNavigator const& relationNavigator) {

    const auto gen4Momentum = getGen4Momentum(recoCand, relationNavigator);
    if (gen4Momentum == LorentzVectorT()) {
        return {-1, 1};
    }

    const auto& recoMomentum = recoCand.getMomentum();
    const double pt = std::sqrt(recoMomentum[0] * recoMomentum[0] + recoMomentum[1] * recoMomentum[1]);

    return BasicInfo{pt - gen4Momentum.Pt(), recoCand.getEnergy() - gen4Momentum.E()};
}


void fillHists(TH1F* hDeltaPt, TH1F* hDeltaE, edm4hep::ReconstructedParticle const& recoCand,
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

        auto& genParticles = store.get<edm4hep::MCParticleCollection>("MCParticles");
        auto& muons = store.get<edm4hep::ReconstructedParticleCollection>("Muon");
        auto& electrons = store.get<edm4hep::ReconstructedParticleCollection>("Electron");
        auto& photons = store.get<edm4hep::ReconstructedParticleCollection>("Photon");
        auto& jets = store.get<edm4hep::ReconstructedParticleCollection>("Jet");
        auto& muonRecoMCAssocs = store.get<edm4hep::MCRecoParticleAssociationCollection>("MuonMCAssociation");
        auto& electronRecoMCAssocs = store.get<edm4hep::MCRecoParticleAssociationCollection>("ElectronMCAssociation");
        auto& photonRecoMCAssocs = store.get<edm4hep::MCRecoParticleAssociationCollection>("PhotonMCAssociation");
        auto& jetRecoMCAssocs = store.get<edm4hep::MCRecoParticleAssociationCollection>("JetMCAssociation");

        MCRecoParticleAssociationNavigator muonMCRecoNavigator(muonRecoMCAssocs);
        MCRecoParticleAssociationNavigator electronMCRecoNavigator(electronRecoMCAssocs);
        MCRecoParticleAssociationNavigator photonMCRecoNavigator(photonRecoMCAssocs);
        MCRecoParticleAssociationNavigator jetMCRecoNavigator(jetRecoMCAssocs);

        for (const auto& muon : muons) {
            fillHists(muonDeltaPt, muonDeltaE, muon, muonMCRecoNavigator);
        }

        for (const auto& electron : electrons) {
            fillHists(electronDeltaPt, electronDeltaE, electron, electronMCRecoNavigator);
        }

        for (const auto& photon : photons) {
            fillHists(photonDeltaPt, photonDeltaE, photon, photonMCRecoNavigator);
        }

        for (const auto& jet : jets) {
            fillHists(jetDeltaPt, jetDeltaE, jet, jetMCRecoNavigator);

            const auto gen4Momentum = getGen4Momentum(jet, jetMCRecoNavigator);
            jetRecoE->Fill(jet.getEnergy());
            jetGenE->Fill(gen4Momentum.E());
            jetRecoM->Fill(jet.getMass());
            jetGenM->Fill(gen4Momentum.M());

            jetNTracks->Fill(jet.tracks_size());
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
