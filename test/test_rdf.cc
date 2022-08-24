
#include "edm4hep/utils/dataframe.h"

#include "edm4hep/MCParticleData.h"
#include "edm4hep/SimTrackerHitData.h"

#include <ROOT/RDataFrame.hxx>

#include <iostream>

int main(int argc, char* argv[]) {
  using namespace edm4hep::utils;

  ROOT::EnableImplicitMT();

  std::cout << "Create RDataFrame ..." << std::endl;

  ROOT::RDataFrame df("events", "edm4hep_events.root");

  std::cout << "Apply selectors and define new branches ..." << std::endl;
  auto df2 = df.Define("MCParticles_pt", pt<edm4hep::MCParticleData>, {"MCParticles"})
                 .Define("MCParticles_eta", eta<edm4hep::MCParticleData>, {"MCParticles"})
                 .Define("MCParticles_cosTheta", cos_theta<edm4hep::MCParticleData>, {"MCParticles"})
                 .Define("SimTrackerHits_r", r<edm4hep::SimTrackerHitData>, {"SimTrackerHits"});

  std::string outfilename = "edm4hep_events_rdf.root";
  std::cout << "Writing snapshot to disk ... \t" << outfilename << std::endl;

  // df2.Display({"MCParticles_pt"})->Print();

  df2.Snapshot("events", outfilename,
               {
                   "MCParticles_pt",
                   "MCParticles_eta",
                   "MCParticles_cosTheta",
                   "SimTrackerHits_r",
               });

  return 0;
}
