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

  auto& mcps  = store.create<edm4hep::MCParticleCollection>("MCParticles");
  writer.registerForWrite("MCParticles");

  unsigned nevents = 1000 ;

  for(unsigned i=0; i<nevents; ++i) {
    if(i % 100 == 0) {
      std::cout << "processing event " << i << std::endl;
    }


    // ---- add some MC particles ----
    auto mcp0 = edm4hep::MCParticle();
    auto mcp1 = edm4hep::MCParticle();
    auto mcp2 = edm4hep::MCParticle();
    auto mcp3 = edm4hep::MCParticle();
    auto mcp4 = edm4hep::MCParticle();
    auto mcp5 = edm4hep::MCParticle();
    auto mcp6 = edm4hep::MCParticle();
    auto mcp7 = edm4hep::MCParticle();
    auto mcp8 = edm4hep::MCParticle();
    auto mcp9 = edm4hep::MCParticle();

    mcps.push_back( mcp0 ) ;
    mcps.push_back( mcp1 ) ;
    mcps.push_back( mcp2 ) ;
    mcps.push_back( mcp3 ) ;
    mcps.push_back( mcp4 ) ;
    mcps.push_back( mcp5 ) ;
    mcps.push_back( mcp6 ) ;
    mcps.push_back( mcp7 ) ;
    mcps.push_back( mcp8 ) ;
    mcps.push_back( mcp9 ) ;

    // --- add some daughter relations
    auto p = edm4hep::MCParticle();
    auto d = edm4hep::MCParticle();

    p = mcps[0] ;
    p.addDaughter( mcps[2] ) ;
    p.addDaughter( mcps[3] ) ;
    p.addDaughter( mcps[4] ) ;
    p.addDaughter( mcps[5] ) ;
    p = mcps[1] ;
    p.addDaughter( mcps[2] ) ;
    p.addDaughter( mcps[3] ) ;
    p.addDaughter( mcps[4] ) ;
    p.addDaughter( mcps[5] ) ;
    p = mcps[2] ;
    p.addDaughter( mcps[6] ) ;
    p.addDaughter( mcps[7] ) ;
    p.addDaughter( mcps[8] ) ;
    p.addDaughter( mcps[9] ) ;
    p = mcps[3] ;
    p.addDaughter( mcps[6] ) ;
    p.addDaughter( mcps[7] ) ;
    p.addDaughter( mcps[8] ) ;
    p.addDaughter( mcps[9] ) ;

    //--- now fix the parent relations
    for( unsigned j=0,N=mcps.size();j<N;++j){
      p = mcps[j] ;
      for(auto it = p.daughters_begin(), end = p.daughters_end() ; it!=end ; ++it ){
	int dIndex = it->getObjectID().index ;
	d = mcps[ dIndex ] ;
	d.addParent( p ) ;
      }
    }
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
    //-------------------------------

    writer.writeEvent();
    store.clearCollections();
  }

  writer.finish();
}


int main(int argc, char* argv[]){

  write("edm4hep_events.root");
}
