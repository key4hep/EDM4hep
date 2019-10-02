// Data model
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/SimTrackerHitCollection.h"
#include "edm4hep/CaloHitContributionCollection.h"
#include "edm4hep/SimCalorimeterHitCollection.h"

// STL
#include <iostream>
#include <vector>

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTWriter.h"


void write(std::string outfilename) {
  std::cout<<"start processing"<<std::endl;

  podio::EventStore store ;
  podio::ROOTWriter  writer(outfilename, &store);

  // create the collections to be written
  auto& mcps  = store.create<edm4hep::MCParticleCollection>("MCParticles");
  writer.registerForWrite("MCParticles");

  unsigned nevents = 10 ;

  // =============== event loop ================================
  for(unsigned i=0; i<nevents; ++i) {
    std::cout << " --- processing event " << i << std::endl;


    // place the following generator event to the MCParticle collection
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

    auto mcp1 = mcps.create();
    mcp1.setPDG( 2212 ) ;
    mcp1.setMass( 0.938 ) ;
    mcp1.setMomentum( { 0.000, 0.000, 7000.000 }  ) ;
    mcp1.setGeneratorStatus( 3 ) ;

    auto mcp2 = mcps.create();
    mcp2.setPDG( 2212 ) ;
    mcp2.setMass( 0.938 ) ;
    mcp2.setMomentum( { 0.000, 0.000, -7000.000 }  ) ;
    mcp2.setGeneratorStatus( 3 ) ;

    auto mcp3 = mcps.create();
    mcp3.setPDG( 1 ) ;
    mcp3.setMass(0.0) ;
    mcp3.setMomentum( {  0.750, -1.569, 32.191 }  ) ;
    mcp3.setGeneratorStatus( 3 ) ;
    mcp3.addParent( mcp1 ) ;

    auto mcp4 = mcps.create();
    mcp4.setPDG( -2) ;
    mcp4.setMass(0.0) ;
    mcp4.setMomentum( { -3.047, -19.000, -54.629 }  ) ;
    mcp4.setGeneratorStatus( 3 ) ;
    mcp4.addParent( mcp2 ) ;

    auto mcp5 = mcps.create();
    mcp5.setPDG( -24 ) ;
    mcp5.setMass(80.799) ;
    mcp5.setMomentum( { 1.517, -20.68, -20.605 }  ) ;
    mcp5.setGeneratorStatus( 3 ) ;
    mcp5.addParent( mcp1 ) ;
    mcp5.addParent( mcp2 ) ;

    auto mcp6 = mcps.create();
    mcp6.setPDG( 22 ) ;
    mcp6.setMass(0.0) ;
    mcp6.setMomentum( { -3.813, 0.113, -1.833  }  ) ;
    mcp6.setGeneratorStatus( 1 ) ;
    mcp6.addParent( mcp1 ) ;
    mcp6.addParent( mcp2 ) ;

    auto mcp7 = mcps.create();
    mcp7.setPDG( 1 ) ;
    mcp7.setMass(0.0) ;
    mcp7.setMomentum( {  -2.445, 28.816, 6.082  }  ) ;
    mcp7.setGeneratorStatus( 1 ) ;
    mcp7.addParent( mcp5 ) ;

    auto mcp8 = mcps.create();
    mcp8.setPDG( -2) ;
    mcp8.setMass(0.0) ;
    mcp8.setMomentum( { 3.962, -49.498, -26.687 }  ) ;
    mcp8.setGeneratorStatus( 1 ) ;
    mcp8.addParent( mcp5 ) ;


    //--- now fix the daughter relations -------------------------
    for( auto p : mcps ){
      for(auto it = p.parents_begin(), end = p.parents_end() ; it!=end ; ++it ){
	int momIndex = it->getObjectID().index ;
	auto pmom = mcps[ momIndex ] ;
	pmom.addDaughter( p ) ;
      }
    }
    //fixme: should this become a utility function ?
    //-------------------------------------------------------------


    //-------- print particles for debugging:

    std::cout << "\n collection:  " << "MCParticles" <<  " of type " <<  mcps.getValueTypeName() << "\n\n"
	      << mcps << std::endl ;
    //-------------------------------

    writer.writeEvent();
    store.clearCollections();
  }

  writer.finish();
}


int main(int argc, char* argv[]){

  write("edm4hep_events.root");
}
