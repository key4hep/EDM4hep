#ifndef EDM4HEP_TEST_READ_EVENTS_H__
#define EDM4HEP_TEST_READ_EVENTS_H__

#include "dimensions.h"

// test data model
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/SimTrackerHitCollection.h"
#include "edm4hep/CaloHitContributionCollection.h"
#include "edm4hep/SimCalorimeterHitCollection.h"
#include "edm4hep/Measurement2DCollection.h"

// podio specific includes
#include "podio/EventStore.h"

// STL
#include <vector>
#include <iostream>
#include <exception>
#include <cassert>


bool checkMeasurements(const edm4hep::Measurement2DCollection& coll) {
  const auto m1 = coll[0];
  const auto [m1d1, m1d2] = m1.getDimensions<Cartesian>();
  if (m1d1 != Cartesian::X) return false;
  if (m1d2 != Cartesian::Z) return false;

  if (m1.getValue(Cartesian::Z) != 3.14f) return false;
  if (m1.getValue(Cartesian::X) != 1.234f) return false;
  if (m1.getCov(Cartesian::X, Cartesian::X) != 42.0f) return false;
  // Check that also transposition works
  if (m1.getCov(Cartesian::Z, Cartesian::X) != 3.14f) return false;
  if (m1.getCov(Cartesian::Z, Cartesian::Z) != 2.34f) return false;


  const auto m2 = coll[1];
  // We can also work with the array here directly
  const auto& m2Dims = m2.getDimensions<Polar>();
  if (m2Dims[0] != Polar::PHI) return false;
  if (m2Dims[1] != Polar::THETA) return false;

  if (m2.getValue(Polar::THETA) != 200.f) return false;
  if (m2.getValue(Polar::PHI) != 100.f) return false;

  if (m2.getCov(Polar::THETA, Polar::THETA) != 30.f) return false;
  if (m2.getCov(Polar::THETA, Polar::PHI) != 20.f) return false;
  if (m2.getCov(Polar::PHI, Polar::PHI) != 10.f) return false;


  const auto m3 = coll[2];

  // As is the case with all instances we can access this with arbitrary enums
  const auto m3DimsPolar = m3.getDimensions<Polar>();
  if (m3DimsPolar[0] != Polar::THETA) return false;
  if (m3DimsPolar[1] != Polar::PHI) return false;

  const auto m3DimsCart = m3.getDimensions<Cartesian>();
  if (m3DimsCart[0] != Cartesian::Z) return false;
  if (m3DimsCart[1] != Cartesian::Y) return false;

  if (m3.getValue(Polar::THETA) != 1.234f) return false;
  if (m3.getValue(Cartesian::Y) != 5.678f) return false;

  if (m3.getCov(Polar::THETA, Polar::THETA) != 1.1f) return false;
  if (m3.getCov(Cartesian::Y, Cartesian::Y) != 3.3f) return false;
  if (m3.getCov(Polar::THETA, Polar::PHI) != m3.getCov(Cartesian::Z, Cartesian::Y)) return false;

  return true;
}

void processEvent(podio::EventStore& store, bool verboser, unsigned eventNum) {
  auto& mcps   = store.get<edm4hep::MCParticleCollection>("MCParticles");
  auto& sths   = store.get<edm4hep::SimTrackerHitCollection>("SimTrackerHits");
  auto& schs   = store.get<edm4hep::SimCalorimeterHitCollection>("SimCalorimeterHits");
  auto& sccons = store.get<edm4hep::CaloHitContributionCollection>("SimCalorimeterHitContributions");


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
    throw std::runtime_error("Collection 'MCParticles' should be present");
  }

  //===============================================================================
  if( sths.isValid() ){

    std::cout << "\n collection:  " << "SimTrackerHits" <<  " of type " <<  sths.getValueTypeName() << "\n\n"
        << sths << std::endl ;

    int nsh = 5 ;
    for(int j=0 ; j< nsh ; ++j){

      auto sth1 = sths[2*j] ;
      if( sth1.getCellID() !=  0xabadcaffee )  throw std::runtime_error("cellID != 0xabadcaffee") ;
      if( sth1.getEDep() !=  j * 0.000001f ) throw std::runtime_error("e_dep != j * 0.000001") ;
      if( !( sth1.getPosition() == edm4hep::Vector3d( j*10. , j*20., j*5. )) )
        throw std::runtime_error("position != ( j*10. , j*20., j*5. )") ;
      if( !(sth1.getMCParticle() == mcps[6]) )  throw std::runtime_error("mcp != mcps[6]")  ;

      auto sth2 = sths[2*j+1] ;
      if( sth2.getCellID() !=   0xcaffeebabe )  throw std::runtime_error("cellID != 0xcaffeebabe") ;
      if( sth2.getEDep() !=  j * 0.001f ) throw std::runtime_error("e_dep != j * 0.001") ;
      if( !( sth2.getPosition() == edm4hep::Vector3d( -j*10. , -j*20., -j*5. )) )
        throw std::runtime_error("position != ( -j*10. , -j*20., -j*5. )") ;
      if( !(sth2.getMCParticle() == mcps[7]) )  throw std::runtime_error("mcp != mcps[7]")  ;
    }

  } else {
    throw std::runtime_error("Collection 'SimTrackerHits' should be present");
  }

  //===============================================================================
  if( sccons.isValid() ){

    std::cout << "\n collection:  " << "SimCalorimeterHitContributions" <<  " of type "
        <<  sccons.getValueTypeName() << "\n\n"
        << sccons << std::endl ;

  } else {
    throw std::runtime_error("Collection 'SimCalorimeterHitContributions' should be present");
  }
  //===============================================================================
  if( schs.isValid() ){

    std::cout << "\n collection:  " << "SimCalorimeterHit" <<  " of type "
        <<  schs.getValueTypeName() << "\n\n"
        << schs << std::endl ;


    int nch = 5 ;
    for(int j=0 ; j< nch ; ++j){

      auto sch1 = schs[2*j] ;
      if( sch1.getCellID() !=  0xabadcaffee )  throw std::runtime_error("cellID != 0xabadcaffee") ;
      if( sch1.getEnergy() !=  j*0.1f ) throw std::runtime_error("energy != j*0.1f") ;
      if( !( sch1.getPosition() == edm4hep::Vector3f( j*100.f , j*200.f, j*50.f )) )
        throw std::runtime_error("position != ( j*100. , j*200., j*50. )") ;

      auto cont1 = sch1.getContributions(0) ;

      if( !( cont1.getPDG() == 11 )  ||
          !( cont1.getEnergy() ==  j*0.1f ) ||
          !( cont1.getTime() ==  j*1e-9f )  ||
          !( cont1.getStepPosition() == edm4hep::Vector3f( j*100.01f , j*200.01f, j*50.01f ) ) ||
          !( cont1.getParticle() == mcps[6] )
  ){
  throw std::runtime_error("contribution1 does not match ") ;
      }

      auto sch2 = schs[2*j+1] ;
      if( sch2.getCellID() !=   0xcaffeebabe )  throw std::runtime_error("cellID != 0xcaffeebabe") ;
      if( sch2.getEnergy() !=  j*0.2f ) throw std::runtime_error("energy != j*0.2") ;
      if( !( sch2.getPosition() == edm4hep::Vector3f( -j*100.f , -j*200.f, -j*50.f )) )
          throw std::runtime_error("position != ( -j*100.f , -j*200.f, -j*50.f )") ;

     auto cont2 = sch2.getContributions(0) ;

      if( !( cont2.getPDG() == -11 )  ||
          !( cont2.getEnergy() ==  j*0.2f ) ||
          !( cont2.getTime() ==  j*1e-9f )  ||
          !( cont2.getStepPosition() == edm4hep::Vector3f( -j*100.01f , -j*200.01f, -j*50.01f ) ) ||
          !( cont2.getParticle() == mcps[7] )
        ){
          throw std::runtime_error("contribution2 does not match ") ;
      }
    }

  } else {
    throw std::runtime_error("Collection 'SimCalorimeterHits' should be present");
  }

 // //===============================================================================
 //  if( sccons.isValid() ){
 //  } else {
 //    throw std::runtime_error("Collection 'SimCalorimeterHitContributions' should be present");
 //  }

  auto& measurements = store.get<edm4hep::Measurement2DCollection>("measurement2D");
  if (!checkMeasurements(measurements)) {
    throw std::runtime_error("Measurement not as expected");
  }

}

template<typename ReaderT>
void read_events(const std::string& filename) {
  ReaderT reader;
  reader.openFile(filename);

  auto store = podio::EventStore();
  store.setReader(&reader);

  unsigned nEvents = reader.getEntries();
  for(unsigned i=0; i<nEvents; ++i) {
    if(i%1000==0)
      std::cout<<"reading event "<<i<<std::endl;
    processEvent(store, true, i);
    store.clear();
    reader.endOfEvent();
  }
  reader.closeFile();
}

#endif
