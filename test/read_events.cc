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

    // check that we can retrieve the correct information written in write_event.cpp :
    //
    //     name status pdg_id  parent Px       Py    Pz       Energy      Mass
    //  1  !p+!    3   2212    0,0    0.000    0.000 7000.000 7000.000    0.938
    //  2  !p+!    3   2212    0,0    0.000    0.000-7000.000 7000.000    0.938
    //=========================================================================
    //  3  !d!     3      1    1,1    0.750   -1.569   32.191   32.238    0.000
    //  4  !u~!    3     -2    2,2   -3.047  -19.000  -54.629   57.920    0.000
    //  5  !W-!    3    -24    1,2    1.517   -20.68  -20.605   85.925   80.799
    //  6  !gamma! 1     22    1,2   -3.813    0.113   -1.833    4.233    0.000
    //  7  !d!     1      1    5,5   -2.445   28.816    6.082   29.552    0.010
    //  8  !u~!    1     -2    5,5    3.962  -49.498  -26.687   56.373    0.006

    //-------- print particles for debugging:

    std::cout << "\n collection:  " << "MCParticles" <<  " of type " <<  mcps.getValueTypeName() << "\n\n"
	      << mcps << std::endl ;
    //-------------------------------

    // check a few things (to be completed ...)
    auto mcp1 = mcps[0] ;
    if( mcp1.getPDG() != 2212 ) throw std::runtime_error("wrong PDG for first particle - should be 2212 " );

    auto d0 = mcp1.getDaughters(0) ;
    auto d1 = mcp1.getDaughters(1) ;
    auto d2 = mcp1.getDaughters(2) ;

    if( ! ( d0 == mcps[2] ) )  throw std::runtime_error(" error: 1. daughter of particle 0 is not particle 2 ");
    if( ! ( d1 == mcps[4] ) )  throw std::runtime_error(" error: 2. daughter of particle 0 is not particle 4 ");
    if( ! ( d2 == mcps[5] ) )  throw std::runtime_error(" error: 3. daughter of particle 0 is not particle 5 ");


    auto mcp2 = mcps[1] ;
    if( mcp2.getGeneratorStatus() != 3 ) throw std::runtime_error("wrong genStat for 2. particle - should be 3" );
    // and so on ...

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
