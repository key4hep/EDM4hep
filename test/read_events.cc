// STL
#include <vector>
#include <iostream>
#include <exception>
#include <cassert>

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

// test data model
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/SimTrackerHitCollection.h"
#include "edm4hep/CaloHitContributionCollection.h"
#include "edm4hep/SimCalorimeterHitCollection.h"

int glob = 0;


void processEvent(podio::EventStore& store, bool verboser, unsigned eventNum) {


  auto& mcps =  store.get<edm4hep::MCParticleCollection>("MCParticles");

  if( mcps.isValid() ){
    // check that we can retrieve the correct parent daughter relation
    // set in write.cpp :

    // particle 0 has particles 2,3,4 and 5 as daughters:
    auto p = mcps[0] ;

    //-------- print relations for debugging:
    for( auto p : mcps ){
      std::cout << " particle " << p.getObjectID().index << " has daughters: " ;
      for(auto it = p.daughters_begin(), end = p.daughters_end() ; it!=end ; ++it ){
        std::cout << " " << it->getObjectID().index ;
      }
      std::cout << "  and parents: " ;
      for(auto it = p.parents_begin(), end = p.parents_end() ; it!=end ; ++it ){
        std::cout << " " << it->getObjectID().index ;
      }
      std::cout << std::endl ;
    }

    auto d0 = p.getDaughters(0) ;
    auto d1 = p.getDaughters(1) ;
    auto d2 = p.getDaughters(2) ;
    auto d3 = p.getDaughters(3) ;

    if( ! ( d0 == mcps[2] ) )  throw std::runtime_error(" error: 1. daughter of particle 0 is not particle 2 ");
    if( ! ( d1 == mcps[3] ) )  throw std::runtime_error(" error: 2. daughter of particle 0 is not particle 3 ");
    if( ! ( d2 == mcps[4] ) )  throw std::runtime_error(" error: 3. daughter of particle 0 is not particle 4 ");
    if( ! ( d3 == mcps[5] ) )  throw std::runtime_error(" error: 4. daughter of particle 0 is not particle 5 ");


    // particle 3 has particles 6,7,8 and 9 as daughters:
    p = mcps[3] ;

    d0 = p.getDaughters(0) ;
    d1 = p.getDaughters(1) ;
    d2 = p.getDaughters(2) ;
    d3 = p.getDaughters(3) ;

    if( ! ( d0 == mcps[6] ) )  throw std::runtime_error(" error: 1. daughter of particle 3 is not particle 6 ");
    if( ! ( d1 == mcps[7] ) )  throw std::runtime_error(" error: 2. daughter of particle 3 is not particle 7 ");
    if( ! ( d2 == mcps[8] ) )  throw std::runtime_error(" error: 3. daughter of particle 3 is not particle 8 ");
    if( ! ( d3 == mcps[9] ) )  throw std::runtime_error(" error: 4. daughter of particle 3 is not particle 9 ");

  } else {
    throw std::runtime_error("Collection 'mcparticles' should be present");
  }

}

int main(){
  auto reader = podio::ROOTReader();
  auto store = podio::EventStore();
  reader.openFile("edm4hep_events.root");
  store.setReader(&reader);

  bool verbose = true;

  unsigned nEvents = reader.getEntries();
  for(unsigned i=0; i<nEvents; ++i) {
    if(i%1000==0)
      std::cout<<"reading event "<<i<<std::endl;
    processEvent(store, true, i);
    store.clear();
    reader.endOfEvent();
  }
  reader.closeFile();
  return 0;
}
