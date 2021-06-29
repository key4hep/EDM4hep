#ifndef EDM4HEP_TEST_WRITE_EVENTS_H__
#define EDM4HEP_TEST_WRITE_EVENTS_H__

// Data model
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/SimTrackerHitCollection.h"
#include "edm4hep/CaloHitContributionCollection.h"
#include "edm4hep/SimCalorimeterHitCollection.h"
#include "edm4hep/SimCalorimeterHitCollection.h"
#include "edm4hep/UserFloatCollection.h"
#include "edm4hep/UserExtCollection.h"

// STL
#include <iostream>
#include <vector>

// podio specific includes
#include "podio/EventStore.h"

template<class WriterT>
void write(std::string outfilename) {
  std::cout<<"start processing"<<std::endl;

  podio::EventStore store ;
  WriterT writer(outfilename, &store);

  // create the collections to be written
  auto& mcps  = store.create<edm4hep::MCParticleCollection>("MCParticles");
  writer.registerForWrite("MCParticles");

  auto& sths  = store.create<edm4hep::SimTrackerHitCollection>("SimTrackerHits");
  writer.registerForWrite("SimTrackerHits");

  auto& schs = store.create<edm4hep::SimCalorimeterHitCollection>("SimCalorimeterHits");
  writer.registerForWrite("SimCalorimeterHits");

  auto& sccons = store.create<edm4hep::CaloHitContributionCollection>("SimCalorimeterHitContributions");
  writer.registerForWrite("SimCalorimeterHitContributions");

  auto& usrflts  = store.create<edm4hep::UserFloatCollection>("UserFloats");
  writer.registerForWrite("UserFloats");

  auto& usrexts  = store.create<edm4hep::UserExtCollection>("UserExts");
  writer.registerForWrite("UserExts");

  auto& usrexts2  = store.create<edm4hep::UserExtCollection>("SecondUserExts");
  writer.registerForWrite("SecondUserExts");


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
    mcp3.addToParents( mcp1 ) ;

    auto mcp4 = mcps.create();
    mcp4.setPDG( -2) ;
    mcp4.setMass(0.0) ;
    mcp4.setMomentum( { -3.047, -19.000, -54.629 }  ) ;
    mcp4.setGeneratorStatus( 3 ) ;
    mcp4.addToParents( mcp2 ) ;

    auto mcp5 = mcps.create();
    mcp5.setPDG( -24 ) ;
    mcp5.setMass(80.799) ;
    mcp5.setMomentum( { 1.517, -20.68, -20.605 }  ) ;
    mcp5.setGeneratorStatus( 3 ) ;
    mcp5.addToParents( mcp1 ) ;
    mcp5.addToParents( mcp2 ) ;

    auto mcp6 = mcps.create();
    mcp6.setPDG( 22 ) ;
    mcp6.setMass(0.0) ;
    mcp6.setMomentum( { -3.813, 0.113, -1.833  }  ) ;
    mcp6.setGeneratorStatus( 1 ) ;
    mcp6.addToParents( mcp1 ) ;
    mcp6.addToParents( mcp2 ) ;

    auto mcp7 = mcps.create();
    mcp7.setPDG( 1 ) ;
    mcp7.setMass(0.0) ;
    mcp7.setMomentum( {  -2.445, 28.816, 6.082  }  ) ;
    mcp7.setGeneratorStatus( 1 ) ;
    mcp7.addToParents( mcp5 ) ;

    auto mcp8 = mcps.create();
    mcp8.setPDG( -2) ;
    mcp8.setMass(0.0) ;
    mcp8.setMomentum( { 3.962, -49.498, -26.687 }  ) ;
    mcp8.setGeneratorStatus( 1 ) ;
    mcp8.addToParents( mcp5 ) ;


    //--- now fix the daughter relations -------------------------
    for( auto p : mcps ){
      for(auto it = p.parents_begin(), end = p.parents_end() ; it!=end ; ++it ){
        int momIndex = it->getObjectID().index ;
        auto pmom = mcps[ momIndex ] ;
        pmom.addToDaughters( p ) ;
      }
    }
    //fixme: should this become a utility function ?
    //-------------------------------------------------------------


    //----  add a user float value per MCParticle as 'user data'

    for( auto p : mcps ){

      auto ufv =  usrflts.create();

      auto pv = p.getMomentum() ;

      float pp = sqrt( pv[0] * pv[0] +  pv[1] * pv[1] +  pv[2] * pv[2] );

      ufv.setValue( pp  ) ;
    }

    //-------- print particles for debugging:
    std::cout << "\n collection:  " << "MCParticles" <<  " of type " <<  mcps.getValueTypeName() << "\n\n"
        << mcps << std::endl ;


    //===============================================================================
    // write some SimTrackerHits
    int nsh = 5 ;
    for(int j=0 ; j< nsh ; ++j){
      auto sth1 = sths.create() ;
      sth1.setCellID( 0xabadcaffee ) ;
      sth1.setEDep( j * 0.000001 );
      sth1.setPosition( { j*10. , j*20., j*5. } ) ;
      sth1.setMCParticle( mcp7 ) ;

      auto sth2 = sths.create() ;
      sth2.setCellID( 0xcaffeebabe ) ;
      sth2.setPosition( { -j*10. , -j*20., -j*5. } ) ;
      sth2.setEDep( j * .001 );
      sth2.setMCParticle( mcp8 ) ;
    }

    std::cout << "\n collection:  " << "SimTrackerHits" <<  " of type " <<  sths.getValueTypeName() << "\n\n"
        << sths << std::endl ;

    //===============================================================================
    // write some SimCalorimeterHits
    int nch = 5 ;
    for(int j=0 ; j< nch ; ++j){
      auto sch1 = schs.create() ;
      sch1.setCellID( 0xabadcaffee ) ;
      sch1.setEnergy( j * 0.1 );
      sch1.setPosition( { j*100.f , j*200.f, j*50.f } ) ;

      auto cont1 =  edm4hep::CaloHitContribution(11, j * 0.1f ,j*1e-9f, { j*100.01f , j*200.01f, j*50.01f } );
      sccons.push_back( cont1 ) ;
      cont1.setParticle( mcp7 ) ;
      sch1.addToContributions( cont1 ) ;

      auto sch2 = schs.create() ;
      sch2.setCellID( 0xcaffeebabe ) ;
      sch2.setPosition( { -j*100.f , -j*200.f, -j*50.f } ) ;
      sch2.setEnergy( j * .2 );

      auto cont2 =  edm4hep::CaloHitContribution(-11, j*0.2f , j*1e-9f, { -j*100.01f , -j*200.01f, -j*50.01f } );
      sccons.push_back( cont2 ) ;
      cont2.setParticle( mcp8 ) ;
      sch2.addToContributions( cont2 ) ;
    }

    std::cout << "\n collection:  " << "SimCalorimeterHitContributionss" <<  " of type "
        <<  sccons.getValueTypeName() << "\n\n"
        << sccons << std::endl ;

    std::cout << "\n collection:  " << "SimCalorimeterHits" <<  " of type " <<  schs.getValueTypeName() << "\n\n"
        << schs << std::endl ;



    //===============================================================================
    // Usage 1: each element is a user ext data
    // write some User Defined Type Data
    // - save 10 elements
    // - each element contains x/y/z (float) and i (int)
    for (int i = 0; i < 10; ++i) {
        float x = 1.0;
        float y = 2.0;
        float z = 3.0;
        
        auto udv = usrexts.create();

        // push some value to this x/y/z/i
        udv.addToValF(x);
        udv.addToValF(y);
        udv.addToValF(z);
        udv.addToValI(i);
    }

    // Usage 2: only create one user defined type data
    //          then push the values into this container
    auto udv2 = usrexts2.create();


    for (int i = 0; i < 10; ++i) {
        float x = 1.0;
        float y = 2.0;
        float z = 3.0;
        
        // push some value to this x/y/z/i
        udv2.addToValF(x);
        udv2.addToValF(y);
        udv2.addToValF(z);
        udv2.addToValI(i);
    }


    //===============================================================================

    writer.writeEvent();
    store.clearCollections();
  }

  writer.finish();
}

#endif
