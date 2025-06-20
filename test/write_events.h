#ifndef EDM4HEP_TEST_WRITE_EVENTS_H
#define EDM4HEP_TEST_WRITE_EVENTS_H

// Data model

#include "edm4hep/CaloHitContributionCollection.h"
#include "edm4hep/GeneratorEventParametersCollection.h"
#include "edm4hep/GeneratorPdfInfoCollection.h"
#include "edm4hep/GeneratorToolInfo.h"
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/RawTimeSeriesCollection.h"
#include "edm4hep/SimCalorimeterHitCollection.h"
#include "edm4hep/SimTrackerHitCollection.h"
#include "edm4hep/TrackerHitPlaneCollection.h"

// STL
#include <iostream>
#include <vector>

// podio specific includes
#include "podio/Frame.h"

template <class WriterT>
void write(std::string outfilename) {
  std::cout << "start processing" << std::endl;

  WriterT writer(outfilename);

  unsigned nevents = 10;

  // =============== event loop ================================
  for (unsigned i = 0; i < nevents; ++i) {
    std::cout << " --- processing event " << i << std::endl;
    auto event = podio::Frame();
    auto run = podio::Frame();

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

    auto mcps = edm4hep::MCParticleCollection();
    auto mcp1 = mcps.create();
    mcp1.setPDG(2212);
    mcp1.setMass(0.938f);
    mcp1.setMomentum({0.000, 0.000, 7000.000});
    mcp1.setGeneratorStatus(3);

    auto mcp2 = mcps.create();
    mcp2.setPDG(2212);
    mcp2.setMass(0.938f);
    mcp2.setMomentum({0.000, 0.000, -7000.000});
    mcp2.setGeneratorStatus(3);

    auto mcp3 = mcps.create();
    mcp3.setPDG(1);
    mcp3.setMass(0.0f);
    mcp3.setMomentum({0.750, -1.569, 32.191});
    mcp3.setGeneratorStatus(3);
    mcp3.addToParents(mcp1);

    auto mcp4 = mcps.create();
    mcp4.setPDG(-2);
    mcp4.setMass(0.0f);
    mcp4.setMomentum({-3.047, -19.000, -54.629});
    mcp4.setGeneratorStatus(3);
    mcp4.addToParents(mcp2);

    auto mcp5 = mcps.create();
    mcp5.setPDG(-24);
    mcp5.setMass(80.799f);
    mcp5.setMomentum({1.517, -20.68, -20.605});
    mcp5.setGeneratorStatus(3);
    mcp5.addToParents(mcp1);
    mcp5.addToParents(mcp2);

    auto mcp6 = mcps.create();
    mcp6.setPDG(22);
    mcp6.setMass(0.0f);
    mcp6.setMomentum({-3.813, 0.113, -1.833});
    mcp6.setGeneratorStatus(1);
    mcp6.addToParents(mcp1);
    mcp6.addToParents(mcp2);

    auto mcp7 = mcps.create();
    mcp7.setPDG(1);
    mcp7.setMass(0.0f);
    mcp7.setMomentum({-2.445, 28.816, 6.082});
    mcp7.setGeneratorStatus(1);
    mcp7.addToParents(mcp5);

    auto mcp8 = mcps.create();
    mcp8.setPDG(-2);
    mcp8.setMass(0.0f);
    mcp8.setMomentum({3.962, -49.498, -26.687});
    mcp8.setGeneratorStatus(1);
    mcp8.addToParents(mcp5);

    //--- now fix the daughter relations -------------------------
    for (auto p : mcps) {
      for (auto it = p.parents_begin(), end = p.parents_end(); it != end; ++it) {
        int momIndex = it->getObjectID().index;
        auto pmom = mcps[momIndex];
        pmom.addToDaughters(p);
      }
    }

    //===============================================================================
    // write some generator event data
    auto genParametersCollection = edm4hep::GeneratorEventParametersCollection();
    auto genParam = genParametersCollection.create();
    genParam.setEventScale(23);
    genParam.setAlphaQED(1 / 127);
    genParam.setAlphaQCD(0.1);
    genParam.setSignalProcessId(42);
    genParam.setSqrts(90);
    genParam.addToCrossSections(10);
    genParam.addToCrossSectionErrors(3);
    genParam.addToSignalVertex(mcp1);
    genParam.addToSignalVertex(mcp2);
    event.put(std::move(genParametersCollection), edm4hep::labels::GeneratorEventParameters);

    auto genPdfInfoCollection = edm4hep::GeneratorPdfInfoCollection();
    auto genPdfInfo = genPdfInfoCollection.create();
    genPdfInfo.setPartonId(1, 2);
    genPdfInfo.setLhapdfId({20, 20});
    genPdfInfo.setX({0.5, 0.5});
    genPdfInfo.setXf({0.5, 0.5});
    genPdfInfo.setScale(23);
    event.put(std::move(genPdfInfoCollection), edm4hep::labels::GeneratorPdfInfo);

    //===============================================================================
    // write some generator tool info into the run
    auto toolInfos = std::vector<edm4hep::GeneratorToolInfo>();
    auto toolInfo = edm4hep::GeneratorToolInfo();
    toolInfo.name = "something";
    toolInfo.version = "v1";
    toolInfo.description = "some tool";
    toolInfos.emplace_back(std::move(toolInfo));
    edm4hep::utils::putGenToolInfos(run, toolInfos);

    //===============================================================================
    // write some generator weightname info into the run
    auto weightNames = std::vector<std::string>();
    weightNames.emplace_back("oneWeight");
    weightNames.emplace_back("anotherWeight");
    run.putParameter(edm4hep::labels::GeneratorWeightNames, std::move(weightNames));

    // fixme: should this become a utility function ?
    //-------------------------------------------------------------

    //-------- print particles for debugging:
    std::cout << "\n collection:  MCParticles of type " << mcps.getValueTypeName() << "\n\n" << mcps << std::endl;

    event.put(std::move(mcps), "MCParticles");

    //===============================================================================
    // write some SimTrackerHits
    auto sths = edm4hep::SimTrackerHitCollection();
    int nsh = 5;
    for (int j = 0; j < nsh; ++j) {
      auto sth1 = sths.create();
      sth1.setCellID(0xabadcaffee);
      sth1.setEDep(j * 0.000001f);
      sth1.setPosition({j * 10., j * 20., j * 5.});
      sth1.setParticle(mcp7);

      auto sth2 = sths.create();
      sth2.setCellID(0xcaffeebabe);
      sth2.setPosition({-j * 10., -j * 20., -j * 5.});
      sth2.setEDep(j * .001f);
      sth2.setParticle(mcp8);
    }

    std::cout << "\n collection: SimTrackerHits of type " << sths.getValueTypeName() << "\n\n" << sths << std::endl;

    event.put(std::move(sths), "SimTrackerHits");

    //===============================================================================
    // write some SimCalorimeterHits
    auto schs = edm4hep::SimCalorimeterHitCollection();
    auto sccons = edm4hep::CaloHitContributionCollection();
    int nch = 5;
    for (int j = 0; j < nch; ++j) {
      auto sch1 = schs.create();
      sch1.setCellID(0xabadcaffee);
      sch1.setEnergy(j * 0.1f);
      sch1.setPosition({j * 100.f, j * 200.f, j * 50.f});

      auto cont1 =
          edm4hep::MutableCaloHitContribution(11, j * 0.1f, j * 1e-9f, {j * 100.01f, j * 200.01f, j * 50.01f}, 1e-5f);
      sccons.push_back(cont1);
      cont1.setParticle(mcp7);
      sch1.addToContributions(cont1);

      auto sch2 = schs.create();
      sch2.setCellID(0xcaffeebabe);
      sch2.setPosition({-j * 100.f, -j * 200.f, -j * 50.f});
      sch2.setEnergy(j * .2f);

      auto cont2 = edm4hep::MutableCaloHitContribution(-11, j * 0.2f, j * 1e-9f,
                                                       {-j * 100.01f, -j * 200.01f, -j * 50.01f}, 1e-5f);
      sccons.push_back(cont2);
      cont2.setParticle(mcp8);
      sch2.addToContributions(cont2);
    }

    std::cout << "\n collection: SimCalorimeterHitContributionss " << sccons.getValueTypeName() << "\n\n"
              << sccons << std::endl;

    std::cout << "\n collection: SimCalorimeterHits of type " << schs.getValueTypeName() << "\n\n" << schs << std::endl;

    event.put(std::move(schs), "SimCalorimeterHits");
    event.put(std::move(sccons), "SimCalorimeterHitContributions");

    //===============================================================================
    // write some RawTimeSeries:
    auto tpchs = edm4hep::RawTimeSeriesCollection();
    int ntpch = 5;
    for (int j = 0; j < ntpch; ++j) {
      auto tpch1 = tpchs.create();
      tpch1.setCellID(0xabadcaffee);
      tpch1.setTime(j * 0.3f);
      tpch1.setCharge(j * 2.f);

      auto tpch2 = tpchs.create();
      tpch2.setCellID(0xcaffeebabe);
      tpch2.setTime(j * 0.3f);
      tpch2.setCharge(-j * 2.f);
    }

    std::cout << "\n collection: Time Projection Chamber Hits of type " << tpchs.getValueTypeName() << "\n\n"
              << tpchs << std::endl;
    event.put(std::move(tpchs), "TPCHits");

    //===============================================================================
    // write some TrackerHitPlanes:
    auto thps = edm4hep::TrackerHitPlaneCollection();
    int nthp = 5;
    for (int j = 0; j < nthp; ++j) {
      auto thp1 = thps.create();
      thp1.setCellID(0xabadcaffee);
      thp1.setTime(j * 0.3f);
      thp1.setEDep(j * 0.000001f);
      thp1.setPosition({j * 2., j * 3., j * 5.});

      auto thp2 = thps.create();
      thp2.setCellID(0xcaffeebabe);
      thp2.setTime(j * 0.3f);
      thp2.setEDep(j * 0.0000031f);
      thp2.setPosition({-j * 2., -j * 3., -j * 5.});
    }

    std::cout << "\n collection: Tracker Hit Planes of type " << thps.getValueTypeName() << "\n\n" << thps << std::endl;

    event.put(std::move(thps), "TrackerHitPlanes");

    //===============================================================================

    event.putParameter("EventType", "test");

    writer.writeFrame(event, "events");
    writer.writeFrame(run, "runs");
  }

  writer.finish();
}

#endif
