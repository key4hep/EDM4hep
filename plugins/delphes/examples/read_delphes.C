// make sure that libDelphes can be found in LD_LIBARARY_PATH
#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#endif

#include "common_helpers.h"

// make sure that the Delphes include directory is in ROOT_INCLUDE_PATH
#include "classes/DelphesClasses.h"
#include "ExRootAnalysis/ExRootTreeReader.h"

#include "TClonesArray.h"
#include "TChain.h"
#include "TH1F.h"
#include "Math/Vector4D.h"

#include <cmath>
#include <vector>

using LorentzVectorT = ROOT::Math::PtEtaPhiEVector;

struct BasicInfo {
    double pt;
    double E;
};

template<typename T>
BasicInfo getBasicInfo(const T* cand) {
    return {cand->PT, cand->P4().E()};
}

template<typename RecoT>
BasicInfo getDeltaInfo(const RecoT* recoCand, const GenParticle* genCand) {
    const auto recoInfo = getBasicInfo(recoCand);
    const auto mcInfo = getBasicInfo(genCand);
    return {recoInfo.pt - mcInfo.pt, recoInfo.E - mcInfo.E};
}

template<typename RecoT>
BasicInfo getDeltaInfo(const RecoT* recoCand) {
    auto* mcCand = dynamic_cast<GenParticle*>(recoCand->Particle.GetObject());
    if (mcCand) {
        return getDeltaInfo(recoCand, mcCand);
    }

    return {-1, -1};
}

BasicInfo getDeltaInfo(const Photon* recoCand) {
    // don't handle cases where there is more than 1 related particle
    if (recoCand->Particles.GetEntriesFast() == 1) {
        auto* mcCand = dynamic_cast<GenParticle*>(recoCand->Particles.At(0));
        return getDeltaInfo(recoCand, mcCand);
    }
    return {-1, -1};
}

LorentzVectorT getJetGen4Momentum(const Jet* jet) {
    LorentzVectorT momentum{};

    // sum up all of the associated particles
    for (int i = 0; i < jet->Particles.GetEntriesFast(); ++i) {
        const auto* particle = static_cast<GenParticle*>(jet->Particles.At(i));
        LorentzVectorT tmpMomentum{};
        tmpMomentum.SetCoordinates(particle->PT, particle->Eta, particle->Phi, particle->P4().E());
        momentum += tmpMomentum;
    }
    return momentum;
}


BasicInfo getDeltaInfo(const Jet* recoCand) {
    const auto momentum = getJetGen4Momentum(recoCand);
    return {recoCand->P4().Pt() - momentum.Pt(), recoCand->P4().E() - momentum.E()};
}

template<typename RecoT>
void fillHists(TH1F* hDeltaPt, TH1F* hDeltaE, RecoT* recoCand) {
    const auto deltaInfo = getDeltaInfo(recoCand);
    hDeltaPt->Fill(deltaInfo.pt);
    hDeltaE->Fill(deltaInfo.E);
}


template<typename RecoT>
int getNType(const Jet* jet) {
    int nType = 0;
    for (int i = 0; i < jet->Constituents.GetEntriesFast(); ++i) {
        auto* object = jet->Constituents.At(i);
        // if (!object) std::cout << "NULL\n";
        if (object && object->IsA() == RecoT::Class()) {
            nType++;
        }
    }

    return nType;
}


// Following delphes/examples/Example3.C (more or less)
void read_delphes(const char* inputfile) {
    gSystem->Load("libDelphes.so");

    auto* chain = new TChain("Delphes");
    chain->Add(inputfile);
    auto* treeReader = new ExRootTreeReader(chain);
    const auto nEntries = treeReader->GetEntries();

    auto* genParticles = treeReader->UseBranch("Particle");
    auto* electrons = treeReader->UseBranch("Electron");
    auto* muons = treeReader->UseBranch("Muon");
    auto* photons = treeReader->UseBranch("Photon");
    auto* jets = treeReader->UseBranch("Jet");
    auto* tracks = treeReader->UseBranch("EFlowTrack");
    auto* ecalClusters = treeReader->UseBranch("EFlowPhoton");
    auto* hcalClusters = treeReader->UseBranch("EFlowNeutralHadron");

    defineHists();


    for (int entry = 0; entry < nEntries; ++entry) {
        treeReader->ReadEntry(entry);

        // std::cout << genParticles->GetEntries() << "\n";

        for (int i = 0; i < muons->GetEntries(); ++i) {
            fillHists(muonDeltaPt, muonDeltaE, static_cast<Muon*>(muons->At(i)));
        }

        for (int i = 0; i < electrons->GetEntries(); ++i) {
            fillHists(electronDeltaPt, electronDeltaE, static_cast<Electron*>(electrons->At(i)));
        }

        for (int i = 0; i < photons->GetEntries(); ++i) {
            fillHists(photonDeltaPt, photonDeltaE, static_cast<Photon*>(photons->At(i)));
        }

        for (int i = 0; i < jets->GetEntries(); ++i) {
            const auto* jet = static_cast<Jet*>(jets->At(i));
            fillHists(jetDeltaPt, jetDeltaE, jet);

            const auto gen4momentum = getJetGen4Momentum(jet);
            jetRecoE->Fill(jet->P4().E());
            jetGenE->Fill(gen4momentum.E());
            jetRecoM->Fill(jet->Mass);
            jetGenM->Fill(gen4momentum.M());

            jetNTracks->Fill(getNType<Track>(jet));
            jetNClusters->Fill(getNType<Tower>(jet));
            jetNConstituents->Fill(jet->Constituents.GetEntriesFast());
        }
    }

    storeHists("histograms_delphes.root");
}
