
#include <cmath>
#include <vector>
#include <functional>

#include "tricktrack/HitChainMaker.h"
#include "tricktrack/HitDoublets.h"
#include "tricktrack/TripletFilter.h"
#include "tricktrack/CMGraphUtils.h"

#include "edm4hep/SimTrackerHitCollection.h"







int main() {

  tricktrack::CMGraph g = tricktrack::createGraph({{"Layer1", "Layer2", "Layer3", "Layer4"}});

  std::vector<edm4hep::SimTrackerHit> v1;
  std::vector<edm4hep::SimTrackerHit> v2;
  std::vector<edm4hep::SimTrackerHit> v3;
  std::vector<edm4hep::SimTrackerHit> v4;


  // fill vectors with some dummy data

  v1.emplace_back(0, 0, 0, 0, 0, edm4hep::Vector3d(10, 0, 0), edm4hep::Vector3f(0, 0, 0));
  v2.emplace_back(0, 0, 0, 0, 0, edm4hep::Vector3d(20, 0, 0), edm4hep::Vector3f(0, 0, 0));
  v3.emplace_back(0, 0, 0, 0, 0, edm4hep::Vector3d(30, 0, 0), edm4hep::Vector3f(0, 0, 0));
  v4.emplace_back(0, 0, 0, 0, 0, edm4hep::Vector3d(40, 0, 0), edm4hep::Vector3f(0, 0, 0));


  std::vector<tricktrack::HitDoublets<edm4hep::SimTrackerHit>*> doublets;
  auto doublet1 = new tricktrack::HitDoublets<edm4hep::SimTrackerHit>(v1, v2);
  auto doublet2 = new tricktrack::HitDoublets<edm4hep::SimTrackerHit>(v2, v3);
  auto doublet3 = new tricktrack::HitDoublets<edm4hep::SimTrackerHit>(v3, v4);

  doublets.push_back(doublet1);
  doublets.push_back(doublet2);
  doublets.push_back(doublet3);

  for (size_t i = 0; i < v1.size(); i++) {
    for (size_t j = 0; j < v2.size(); j++) {
      doublets[0]->add(i, j);
    }
  }
  for (size_t i = 0; i < v2.size(); i++) {
    for (size_t j = 0; j < v3.size(); j++) {
      doublets[1]->add(i, j);
    }
  }
  for (size_t i = 0; i < v3.size(); i++) {
    for (size_t j = 0; j < v4.size(); j++) {
      doublets[2]->add(i, j);
    }
  }

  auto automaton = new tricktrack::HitChainMaker<edm4hep::SimTrackerHit>(g);

  // create 
  tricktrack::TripletFilter<edm4hep::SimTrackerHit> tripletfilter = std::bind(tricktrack::defaultGeometricFilter<edm4hep::SimTrackerHit>, std::placeholders::_1, std::placeholders::_2,  
                                    0.8, // ptmin 
                                    0.,  // region_origin_x
                                    0.,  // region_origin_y
                                    0.002, // region_origin_radius
                                    0.2, // phiCut
                                    0.8, // hardPtCut
                                    0.2  // thetaCut
                                    );

  std::vector<tricktrack::CMCell<edm4hep::SimTrackerHit>::CMntuplet> foundTracklets;
  automaton->createAndConnectCells(doublets, tripletfilter);
  automaton->evolve(3);
  automaton->findNtuplets(foundTracklets, 3);

  std::cout << "Found " << foundTracklets.size() << "track seed(s)" << std::endl;



  

}
