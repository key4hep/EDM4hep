#include "edm4hep/utils/dataframe.h"

#include "edm4hep/MCParticleData.h"
#include "edm4hep/SimTrackerHitData.h"
#include "edm4hep/TrackerHitPlaneData.h"

#include <ROOT/RDataFrame.hxx>

#include <iostream>

int main(int, char*[]) {

  ROOT::EnableImplicitMT();

  std::cout << "Create RDataFrame ..." << std::endl;

  ROOT::RDataFrame df("events", "edm4hep_events.root");

  std::cout << "Apply selectors and define new branches ..." << std::endl;
  auto df2 = df.Define("MCParticles_pt", edm4hep::utils::pt<edm4hep::MCParticleData>, {"MCParticles"})
                 .Define("MCParticles_eta", edm4hep::utils::eta<edm4hep::MCParticleData>, {"MCParticles"})
                 .Define("MCParticles_cosTheta", edm4hep::utils::cos_theta<edm4hep::MCParticleData>, {"MCParticles"})
                 .Define("SimTrackerHits_r", edm4hep::utils::r<edm4hep::SimTrackerHitData>, {"SimTrackerHits"})
                 .Define("SimTrackerHit_pt", edm4hep::utils::pt<edm4hep::SimTrackerHitData>, {"SimTrackerHits"})
                 .Define("TrackerHits_r", edm4hep::utils::r<edm4hep::TrackerHitPlaneData>, {"TrackerHitPlanes"});

  std::string outfilename = "edm4hep_events_rdf.root";
  std::cout << "Writing snapshot to disk ... \t" << outfilename << std::endl;

  df2.Snapshot("events", outfilename,
               {"MCParticles_pt", "MCParticles_eta", "MCParticles_cosTheta", "SimTrackerHits_r", "SimTrackerHit_pt",
                "TrackerHits_r"});

  return 0;
}
