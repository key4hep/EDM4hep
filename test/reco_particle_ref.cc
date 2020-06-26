#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/RecoParticleRefCollection.h"

#include "podio/EventStore.h"
#include "podio/ROOTWriter.h"
#include "podio/ROOTReader.h"

#include <stdexcept>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>

#define LOCATION()                                                      \
  std::string(__FILE__) + std::string(":") + std::to_string(__LINE__) + \
  std::string(" in ") + std::string(__PRETTY_FUNCTION__)

#define ASSERT_CONDITION( condition,  message )                              \
{                                                                            \
  if ( !(condition) ) {                                                      \
    throw std::runtime_error( LOCATION() + std::string(": ") + message);     \
  }                                                                          \
}

#define ASSERT_EQUAL( left, right, message )                              \
{                                                                         \
  std::stringstream msg;                                                  \
  msg << message << " | expected: " << right << " - actual: " << left;    \
  ASSERT_CONDITION(left == right, msg.str())                              \
}

constexpr int NEVENTS = 50;

/**
 * Helper class to hold some state between writing and reading.
 *
 * For each event a random number of entries in the
 * ReconstructedParticleCollection and another random number of entries in the
 * RecoParticleRefCollection is generated. The number of RecoParticleRefs is not
 * limited by the number of ReconstructedParticles, other then if the latter is
 * 0.
 *
 * To simulate "real world usage", the particles that are referenced by the
 * RecoParticleRefs, are chosen randomly from the ReconstructedParticles and it
 * is possible that more than one RecoParticleRef points to the same
 * ReconstructedParticle.
 */
struct TestState {
  TestState(int nEvents) :
        nEntries(std::vector<int>(nEvents)), nRefs(std::vector<int>(nEvents)) {
    std::default_random_engine eng{std::random_device{}()};

    // randomly generate the number of entries for each event to be between 0
    // and 20
    std::uniform_int_distribution<> nEntriesDist{0, 20};
    std::generate(nEntries.begin(), nEntries.end(),
                  [&eng, &nEntriesDist]() {return nEntriesDist(eng);});

    // randomly generate the number of references
    // NOTE: The range is deliberately chosen to be the same as above to have
    // cases where there are more references then entries
    std::generate(nRefs.begin(), nRefs.end(),
                  [&eng, &nEntriesDist]() {return nEntriesDist(eng);});

    // generate the indices for the references in the original array
    for (int i = 0; i < nEvents; ++i) {
      if (!nEntries[i]) {
        // If there are no entries, then there is also nothing the relations
        // could point to
        relationIndices.emplace_back();
        nRefs[i] = 0;
        continue;
      }

      std::vector<int> entryIndices(nEntries[i]);
      std::iota(entryIndices.begin(), entryIndices.end(), 0);

      // since std::sample always returns the elements in the same order as they
      // are in the input range and additonally can only return at most as many
      // elements as are in the input range, simply run it enough times to get
      // indices for all planned references
      std::vector<int> relIndices;
      for (int j = 0; j < nRefs[i]; ++j) {
        std::sample(entryIndices.begin(), entryIndices.end(), std::back_inserter(relIndices), 1, eng);
      }

      ASSERT_EQUAL((int)relIndices.size(), nRefs[i], "Sampled wrong number of relation indices");

      relationIndices.push_back(relIndices);
    }

    // Make a quick test to see if the generation worked as intended
    ASSERT_EQUAL(nEntries.size(), nRefs.size(), "Reference and Entries sizes are different");
    ASSERT_EQUAL(relationIndices.size(), nRefs.size(), "Reference and relation indices sizes are different");
    ASSERT_EQUAL((int)nEntries.size(), nEvents, "Generated wrong number of entries");

    for (int i = 0; i < nEvents; ++i) {
      ASSERT_EQUAL((int)relationIndices[i].size(), nRefs[i], "Generated wrong number of relation indices");
    }
  }

  std::vector<int> nEntries;
  std::vector<int> nRefs;
  std::vector<std::vector<int>> relationIndices;
};

void writeRecoParticleRef(const TestState& testState) {
  podio::EventStore store;
  podio::ROOTWriter writer("reco_particle_ref.root", &store);

  auto& recoColl = store.create<edm4hep::ReconstructedParticleCollection>("RecoParticles");
  writer.registerForWrite("RecoParticles");

  auto& refColl = store.create<edm4hep::RecoParticleRefCollection>("RecoRefs");
  writer.registerForWrite("RecoRefs");

  for (int iEvent = 0; iEvent < NEVENTS; ++iEvent) {
    for (int iEntry = 0; iEntry < testState.nEntries[iEvent]; ++iEntry) {
      auto reco = recoColl.create();
      reco.setMomentum({iEntry * 100.f, iEntry * 150.f, iEntry * 200.f});
    }

    ASSERT_EQUAL(testState.nRefs[iEvent], (int)testState.relationIndices[iEvent].size(), "Wrong number of relation indices");

    for (const int index : testState.relationIndices[iEvent]) {
      auto ref = refColl.create();
      ref.setParticle(recoColl[index]);
    }

    // Check that what we write out is as expected
    ASSERT_EQUAL(recoColl.size(), testState.nEntries[iEvent], "Created wrong number of entries");
    ASSERT_EQUAL(refColl.size(), testState.nRefs[iEvent], "Created wrong number of references");

    int i = 0;
    for (const auto& ref : refColl) {
      auto expectedParticle = recoColl[testState.relationIndices[iEvent][i]];
      ASSERT_EQUAL(ref.getParticle(), expectedParticle, "Wrong particle reference during creation");
      i++;
    }

    writer.writeEvent();
    store.clearCollections();
  }

  writer.finish();
}

void readRecoParticleRef(const TestState& testState) {
  podio::EventStore store;
  podio::ROOTReader reader;
  reader.openFile("reco_particle_ref.root");
  store.setReader(&reader);

  for (int iEvent = 0; iEvent < NEVENTS; ++iEvent) {
    auto& recoColl = store.get<edm4hep::ReconstructedParticleCollection>("RecoParticles");
    ASSERT_CONDITION(recoColl.isValid(), "'RecoParticles' collection is not present");
    ASSERT_EQUAL(recoColl.size(), testState.nEntries[iEvent],
                 "'RecoParticles' collection has the wrong number of entries");

    auto& refColl = store.get<edm4hep::RecoParticleRefCollection>("RecoRefs");
    ASSERT_CONDITION(refColl.isValid(), "'RecoRefs' collection is not present");
    ASSERT_EQUAL(refColl.size(), testState.nRefs[iEvent],
                 "'RecoRefs' collection has the wrong number of entries");

    int i = 0;
    for (const auto& reco : recoColl) {
      ASSERT_EQUAL(reco.getMomentum(), edm4hep::Vector3f(i * 100.f, i * 150.f, i * 200.f), "ReconstructedParticle has the wrong momentum");
      i++;
    }

    i = 0;
    for (const auto& ref : refColl) {
      const auto expectedReco = recoColl[testState.relationIndices[iEvent][i]];
      ASSERT_EQUAL(ref.getParticle(), expectedReco, "Wrong particle in reference after reading");
      i++;
    }

    store.clear();
    reader.endOfEvent();
  }
}


int main(int, char**) {
  try {
    const TestState testState(NEVENTS);
    writeRecoParticleRef(testState);
    readRecoParticleRef(testState);
  } catch (std::runtime_error const& ex) {
    std::cerr << ex.what() << std::endl;
    return 1;
  }

  return 0;
}
