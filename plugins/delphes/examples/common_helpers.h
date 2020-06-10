#include "TH1F.h"
#include "TH1I.h"

#include <cmath>
#include <vector>
#include <string>

// declared globally in this header, but it's just a small demonstrator for now
// and it saves some duplication
TH1F* muonDeltaPt;
TH1F* muonDeltaE;
TH1F* electronDeltaPt;
TH1F* electronDeltaE;
TH1F* photonDeltaPt;
TH1F* photonDeltaE;
TH1F* jetDeltaPt;
TH1F* jetDeltaE;
TH1F* jetRecoE;
TH1F* jetGenE;
TH1F* jetRecoM;
TH1F* jetGenM;

TH1I *jetNTracks;

TH1F* logXScaleBinned(const char* name, const int nBins, const float min, const float max)
{
    std::vector<double> xBinning;
    xBinning.reserve(nBins + 1);

    const float lmin = std::log10(min);
    const float lmax = std::log10(max);
    const float dx = (lmax - lmin) / nBins;
    const float l10 = std::log(10);

    for (int i = 0; i < nBins + 1; ++i) {
        xBinning.push_back(lmin + std::exp(l10 * i * dx));
    }

    return new TH1F(name, name, nBins, xBinning.data());
}

void defineHists() {
    muonDeltaPt = new TH1F("muonDeltaPt", "muonDeltaPt", 100, -10, 10);
    muonDeltaE = new TH1F("muonDeltaE", "muonDeltaE", 100, -10, 10);
    electronDeltaPt = new TH1F("electronDeltaPt", "electronDeltaPt", 100, -10, 10);
    electronDeltaE = new TH1F("electronDeltaE", "electronDeltaE", 100, -10, 10);
    photonDeltaPt = new TH1F("photonDeltaPt", "photonDeltaPt", 100, -10, 10);
    photonDeltaE = new TH1F("photonDeltaE", "photonDeltaE", 100, -10, 10);
    jetDeltaPt = new TH1F("jetDeltaPt", "jetDeltaPt", 100, -10, 10);
    jetDeltaE = new TH1F("jetDeltaE", "jetDeltaE", 100, -10, 10);
    jetRecoE = logXScaleBinned("jetRecoE", 100, 2, 2000);
    jetGenE = logXScaleBinned("jetGenE", 100, 2, 2000);
    jetRecoM = new TH1F("jetRecoM", "jetRecoM", 100, 2, 150);
    jetGenM = new TH1F("jetGenM", "jetGenM", 100, 2, 150);
    jetNTracks = new TH1I("jetNTracks", "jetNTracks", 30, 0, 30);
}

void storeHists(const char* filename) {
    auto* file = new TFile(filename, "recreate");
    file->cd();

    muonDeltaPt->Write();
    muonDeltaE->Write();
    electronDeltaPt->Write();
    electronDeltaE->Write();
    photonDeltaPt->Write();
    photonDeltaE->Write();
    jetDeltaPt->Write();
    jetDeltaE->Write();
    jetRecoE->Write();
    jetGenE->Write();
    jetRecoM->Write();
    jetGenM->Write();
    jetNTracks->Write();

    file->Close();
}
