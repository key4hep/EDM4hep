#ifndef EDM4HEP_TEST_READ_EVENTS_H__
#define EDM4HEP_TEST_READ_EVENTS_H__

// test data model
#include "edm4hep/CaloHitContributionCollection.h"
#include "edm4hep/GeneratorEventParametersCollection.h"
#include "edm4hep/GeneratorToolInfo.h"
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/RawTimeSeriesCollection.h"
#include "edm4hep/SimCalorimeterHitCollection.h"
#include "edm4hep/SimTrackerHitCollection.h"
#include "edm4hep/TrackerHitPlaneCollection.h"

// podio specific includes
#include "podio/Frame.h"

// STL
#include <iostream>

void processRun(const podio::Frame& run) {
  //===============================================================================
  // get generator tool info from the run
  auto toolInfos = edm4hep::utils::getGenToolInfos(run);
  auto toolinfo = toolInfos[0];
  if (toolinfo.name != "something")
    throw std::runtime_error("toolinfo.name != 'something'");
  if (toolinfo.version != "v1")
    throw std::runtime_error("toolinfo.version != 'v1'");
  if (toolinfo.description != "some tool")
    throw std::runtime_error("toolinfo.description != 'some tool'");

  //===============================================================================
  // get generator weight names
  auto weightNames = run.getParameter<std::vector<std::string>>(edm4hep::labels::GeneratorWeightNames).value();
  if (weightNames[0] != "oneWeight")
    throw std::runtime_error("weightNames[0] != 'oneWeight'");
  if (weightNames[1] != "anotherWeight")
    throw std::runtime_error("weightNames[1] != 'anotherWeight'");
}

void processEvent(const podio::Frame& event) {
  auto& mcps = event.get<edm4hep::MCParticleCollection>("MCParticles");
  auto& sths = event.get<edm4hep::SimTrackerHitCollection>("SimTrackerHits");
  auto& schs = event.get<edm4hep::SimCalorimeterHitCollection>("SimCalorimeterHits");
  auto& sccons = event.get<edm4hep::CaloHitContributionCollection>("SimCalorimeterHitContributions");
  auto& tpchs = event.get<edm4hep::RawTimeSeriesCollection>("TPCHits");
  auto& thps = event.get<edm4hep::TrackerHitPlaneCollection>("TrackerHitPlanes");

  if (mcps.isValid()) {

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

    std::cout << "\n collection:  "
              << "MCParticles"
              << " of type " << mcps.getValueTypeName() << "\n\n"
              << mcps << std::endl;
    //-------------------------------

    // check a few things (to be completed ...)
    auto mcp1 = mcps[0];
    if (mcp1.getPDG() != 2212)
      throw std::runtime_error("wrong PDG for first particle - should be 2212 ");

    auto d0 = mcp1.getDaughters(0);
    auto d1 = mcp1.getDaughters(1);
    auto d2 = mcp1.getDaughters(2);

    if (d0 != mcps[2])
      throw std::runtime_error(" error: 1. daughter of particle 0 is not particle 2 ");
    if (d1 != mcps[4])
      throw std::runtime_error(" error: 2. daughter of particle 0 is not particle 4 ");
    if (d2 != mcps[5])
      throw std::runtime_error(" error: 3. daughter of particle 0 is not particle 5 ");

    auto mcp2 = mcps[1];
    if (mcp2.getGeneratorStatus() != 3)
      throw std::runtime_error("wrong genStat for 2. particle - should be 3");
    // and so on ...

  } else {
    throw std::runtime_error("Collection 'MCParticles' should be present");
  }

  //===============================================================================
  if (sths.isValid()) {

    std::cout << "\n collection:  "
              << "SimTrackerHits"
              << " of type " << sths.getValueTypeName() << "\n\n"
              << sths << std::endl;

    int nsh = 5;
    for (int j = 0; j < nsh; ++j) {

      auto sth1 = sths[2 * j];
      if (sth1.getCellID() != 0xabadcaffee)
        throw std::runtime_error("cellID != 0xabadcaffee");
      if (sth1.getEDep() != j * 0.000001f)
        throw std::runtime_error("e_dep != j * 0.000001");
      if (sth1.getPosition() != edm4hep::Vector3d(j * 10., j * 20., j * 5.))
        throw std::runtime_error("position != ( j*10. , j*20., j*5. )");
      if (sth1.getParticle() != mcps[6])
        throw std::runtime_error("mcp != mcps[6]");

      auto sth2 = sths[2 * j + 1];
      if (sth2.getCellID() != 0xcaffeebabe)
        throw std::runtime_error("cellID != 0xcaffeebabe");
      if (sth2.getEDep() != j * 0.001f)
        throw std::runtime_error("e_dep != j * 0.001");
      if (sth2.getPosition() != edm4hep::Vector3d(-j * 10., -j * 20., -j * 5.))
        throw std::runtime_error("position != ( -j*10. , -j*20., -j*5. )");
      if (sth2.getParticle() != mcps[7])
        throw std::runtime_error("mcp != mcps[7]");
    }

  } else {
    throw std::runtime_error("Collection 'SimTrackerHits' should be present");
  }

  //===============================================================================
  if (sccons.isValid()) {

    std::cout << "\n collection:  "
              << "SimCalorimeterHitContributions"
              << " of type " << sccons.getValueTypeName() << "\n\n"
              << sccons << std::endl;

  } else {
    throw std::runtime_error("Collection 'SimCalorimeterHitContributions' should be present");
  }
  //===============================================================================
  if (schs.isValid()) {

    std::cout << "\n collection:  "
              << "SimCalorimeterHit"
              << " of type " << schs.getValueTypeName() << "\n\n"
              << schs << std::endl;

    int nch = 5;
    for (int j = 0; j < nch; ++j) {

      auto sch1 = schs[2 * j];
      if (sch1.getCellID() != 0xabadcaffee)
        throw std::runtime_error("cellID != 0xabadcaffee");
      if (sch1.getEnergy() != j * 0.1f)
        throw std::runtime_error("energy != j*0.1f");
      if (sch1.getPosition() != edm4hep::Vector3f(j * 100.f, j * 200.f, j * 50.f))
        throw std::runtime_error("position != ( j*100. , j*200., j*50. )");

      auto cont1 = sch1.getContributions(0);

      if (cont1.getPDG() != 11 || cont1.getEnergy() != j * 0.1f || cont1.getTime() != j * 1e-9f ||
          !(cont1.getStepPosition() == edm4hep::Vector3f(j * 100.01f, j * 200.01f, j * 50.01f)) ||
          cont1.getParticle() != mcps[6]) {
        throw std::runtime_error("contribution1 does not match ");
      }

      auto sch2 = schs[2 * j + 1];
      if (sch2.getCellID() != 0xcaffeebabe)
        throw std::runtime_error("cellID != 0xcaffeebabe");
      if (sch2.getEnergy() != j * 0.2f)
        throw std::runtime_error("energy != j*0.2");
      if (sch2.getPosition() != edm4hep::Vector3f(-j * 100.f, -j * 200.f, -j * 50.f))
        throw std::runtime_error("position != ( -j*100.f , -j*200.f, -j*50.f )");

      auto cont2 = sch2.getContributions(0);

      if (cont2.getPDG() != -11 || cont2.getEnergy() != j * 0.2f || cont2.getTime() != j * 1e-9f ||
          !(cont2.getStepPosition() == edm4hep::Vector3f(-j * 100.01f, -j * 200.01f, -j * 50.01f)) ||
          cont2.getParticle() != mcps[7]) {
        throw std::runtime_error("contribution2 does not match ");
      }
    }

  } else {
    throw std::runtime_error("Collection 'SimCalorimeterHits' should be present");
  }

  //===============================================================================
  if (tpchs.isValid()) {

    std::cout << "\n collection: "
              << "Time Projection Chamber Hit"
              << " of type " << tpchs.getValueTypeName() << "\n\n"
              << tpchs << std::endl;
    int ntpch = 5;
    for (int j = 0; j < ntpch; ++j) {

      auto tpch1 = tpchs[2 * j];
      if (tpch1.getCellID() != 0xabadcaffee)
        throw std::runtime_error("cellID != 0xabadcaffee");
      if (tpch1.getTime() != j * 0.3f)
        throw std::runtime_error("time != j*0.3");
      if (tpch1.getCharge() != j * 2.f)
        throw std::runtime_error("charge != j*2");

      auto tpch2 = tpchs[2 * j + 1];
      if (tpch2.getCellID() != 0xcaffeebabe)
        throw std::runtime_error("cellID != 0xcaffeebabe ");
      if (tpch2.getTime() != j * 0.3f)
        throw std::runtime_error("time != j*0.3");
      if (tpch2.getCharge() != -j * 2.f)
        throw std::runtime_error("charge != -j*2");
    }

  } else {
    throw std::runtime_error("Collection 'TCPHits' should be present");
  }

  //===============================================================================
  if (thps.isValid()) {
    std::cout << "\n collection: "
              << "Tracker Hit Plane"
              << " of type " << thps.getValueTypeName() << "\n\n"
              << thps << std::endl;
    int nthp = 5;
    for (int j = 0; j < nthp; ++j) {
      auto thp1 = thps[2 * j];

      if (thp1.getCellID() != 0xabadcaffee)
        throw std::runtime_error("cellID != 0xabadcaffee");
      if (thp1.getTime() != j * 0.3f)
        throw std::runtime_error("time != j*0.3");
      if (thp1.getEDep() != j * 0.000001f)
        throw std::runtime_error("eDep != j * 0.000001");
      if (!(thp1.getPosition() == edm4hep::Vector3d(j * 2., j * 3., j * 5.)))
        throw std::runtime_error("position != ( j*2. , j*3. , j*5. )");

      auto thp2 = thps[2 * j + 1];

      if (thp2.getCellID() != 0xcaffeebabe)
        throw std::runtime_error("cellID != 0xcaffeebabe");
      if (thp2.getTime() != j * 0.3f)
        throw std::runtime_error("time != j*0.3");
      if (thp2.getEDep() != j * 0.0000031f)
        throw std::runtime_error("eDep != j * 0.0000031 ");
      if (!(thp2.getPosition() == edm4hep::Vector3d(-j * 2., -j * 3., -j * 5.)))
        throw std::runtime_error("position != ( -j*2. , -j*3. , -j*5. )");
    }
  } else {
    throw std::runtime_error("Collection 'TrackerHitPlanes' should be present");
  }

  //===============================================================================
  // check the generator meta data
  auto& genParametersCollection =
      event.get<edm4hep::GeneratorEventParametersCollection>(edm4hep::labels::GeneratorEventParameters);
  auto genParam = genParametersCollection[0];
  if (genParam.getSqrts() != 90)
    throw std::runtime_error("Event_scale != 23");

  const auto evtType = event.getParameter<std::string>("EventType").value();
  std::cout << "Event Type: " << evtType << std::endl;
}

template <typename ReaderT>
void read_events(const std::string& filename) {
  ReaderT reader;
  reader.openFile(filename);

  const auto run = podio::Frame(reader.readNextEntry("runs"));
  processRun(run);

  unsigned nEvents = reader.getEntries("events");
  for (unsigned i = 0; i < nEvents; ++i) {
    std::cout << "reading event " << i << std::endl;
    const auto event = podio::Frame(reader.readNextEntry("events"));
    processEvent(event);
  }
}

#endif
