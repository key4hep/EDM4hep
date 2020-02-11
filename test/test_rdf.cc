
#include <iostream>
#include <ROOT/RDataFrame.hxx>
#include "edm4hepRDF.h"


int main(int argc, char* argv[]){

   ROOT::EnableImplicitMT();
   
   std::cout << "Create RDataFrame ..." << std::endl;
   ROOT::RDataFrame df("events", "edm4hep_events.root");
   std::cout << "Apply selectors and define new branches ..." << std::endl;
   auto df2 = df
                .Define("MCParticles_pt", edm4hep::pt, {"MCParticles"})
                .Define("MCParticles_eta", edm4hep::eta, {"MCParticles"})
                .Define("MCParticles_cosTheta", edm4hep::cos_theta, {"MCParticles"})
                .Define("SimTrackerHits_r", edm4hep::r, {"SimTrackerHits"})
              ;
  std::string outfilename = "edm4hep_events_rdf.root";
  std::cout << "Writing snapshot to disk ... \t" << outfilename << std::endl;
  df2.Snapshot("events", outfilename, {
              "MCParticles_pt",
              "MCParticles_eta",
              "MCParticles_cosTheta",
              "SimTrackerHits_r",
              });
  return 0;
}
