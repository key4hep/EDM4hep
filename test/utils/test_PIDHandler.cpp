#include <edm4hep/utils/ParticleIDUtils.h>

#include <edm4hep/ParticleIDCollection.h>
#include <edm4hep/ReconstructedParticleCollection.h>

#include <podio/Frame.h>

#include <catch2/catch_test_macros.hpp>

#include <tuple>

edm4hep::ReconstructedParticleCollection createRecos() {
  edm4hep::ReconstructedParticleCollection coll;

  for (size_t i = 1; i < 10; ++i) {
    auto reco = coll.create();
    reco.setPDG(i);
  }

  return coll;
}

edm4hep::ParticleIDCollection createParticleIDs(const edm4hep::ReconstructedParticleCollection& recos,
                                                float likelihood) {
  edm4hep::ParticleIDCollection coll;
  for (const auto reco : recos) {
    auto pid = coll.create();
    pid.setLikelihood(likelihood);
    pid.setParticle(reco);

    pid.addToParameters(1.23f);
    pid.addToParameters(3.14f);
  }

  return coll;
}

/// Create an event with some ParticleID collections and a metadata Frame that
/// holds the corresponding metadata
std::tuple<podio::Frame, podio::Frame> createEventAndMetadata() {
  auto retTuple = std::make_tuple(podio::Frame{}, podio::Frame{});
  auto& [event, metadata] = retTuple;

  const auto& recos = event.put(createRecos(), "reco_particles");
  auto pidColl1 = createParticleIDs(recos, 1.0f);
  auto pidColl2 = createParticleIDs(recos, 2.0f);

  edm4hep::utils::PIDHandler::setAlgoInfo(metadata, pidColl1, "particleIds_1",
                                          {"pidAlgo_1", 42, {"first_param", "second_param"}});
  edm4hep::utils::PIDHandler::setAlgoInfo(metadata, pidColl2, "particleIds_2", {"algo_2", 123, {"1", "2"}});

  event.put(std::move(pidColl1), "particleIds_1");
  event.put(std::move(pidColl2), "particleIds_2");

  return retTuple;
}

void checkHandlerValidReco(const edm4hep::utils::PIDHandler& handler, const edm4hep::ReconstructedParticle& reco) {
  const auto pids = handler.getPIDs(reco);

  REQUIRE(pids.size() == 2);
  REQUIRE(pids[0].getParticle() == reco);
  REQUIRE(pids[1].getParticle() == reco);
  REQUIRE(pids[0].getParameters()[0] == 1.23f);
  REQUIRE(pids[0].getParameters()[1] == 3.14f);

  // Cannot guarantee an order if the handler is constructed from a Frame
  const auto llh1 = pids[0].getLikelihood();
  const auto llh2 = pids[1].getLikelihood();
  if (llh1 == 1.0f) {
    REQUIRE(llh2 == 2.0f);
  } else {
    REQUIRE(llh2 == 1.0f);
  }
}

TEST_CASE("PIDHandler basics", "[pid_utils]") {
  using namespace edm4hep;

  const auto recoColl = createRecos();
  const auto pidColl1 = createParticleIDs(recoColl, 1.0f);
  const auto pidColl2 = createParticleIDs(recoColl, 2.0f);

  auto handler = utils::PIDHandler();
  handler.addColl(pidColl1);
  handler.addColl(pidColl2);

  SECTION("Valid PID for reco") {
    auto reco = recoColl[0];
    checkHandlerValidReco(handler, reco);
  }

  SECTION("Unknown reco") {
    const auto reco = edm4hep::ReconstructedParticle();
    const auto pids = handler.getPIDs(reco);

    REQUIRE(pids.empty());
  }
}

TEST_CASE("PIDHandler from variadic list of collections", "[pid_utils]") {
  using namespace edm4hep;

  const auto recoColl = createRecos();
  const auto pidColl1 = createParticleIDs(recoColl, 1.0f);
  const auto pidColl2 = createParticleIDs(recoColl, 2.0f);

  const auto handler = utils::PIDHandler::from(pidColl1, pidColl2);

  SECTION("Valid PID for reco") {
    auto reco = recoColl[3];
    checkHandlerValidReco(handler, reco);
  }

  SECTION("Unknown reco") {
    const auto reco = edm4hep::ReconstructedParticle();
    const auto pids = handler.getPIDs(reco);

    REQUIRE(pids.empty());
  }
}

TEST_CASE("PIDHandler w/ addMetaInfo", "[pid_utils]") {
  using namespace edm4hep;
  auto handler = utils::PIDHandler();

  const auto recoColl = createRecos();
  auto pidColl1 = createParticleIDs(recoColl, 1.0f);
  for (auto pid : pidColl1) {
    pid.setAlgorithmType(42);
  }
  const auto pidInfo1 = utils::ParticleIDMeta{"fancyAlgo", 42, {"p1", "p2"}};

  handler.addColl(pidColl1, pidInfo1);

  REQUIRE(handler.getAlgoType("fancyAlgo").value_or(0) == 42);
  REQUIRE(handler.getParamIndex(42, "p2").value_or(-1) == 1);
  REQUIRE(handler.getPID(recoColl[0], 42).value() == pidColl1[0]);

  // Technically, we can even just add meta data without having a corresponding
  // ParticleID collection to match
  handler.addMetaInfo(utils::ParticleIDMeta{"anotherAlgo", 123, {}});
  REQUIRE(handler.getAlgoType("anotherAlgo").value() == 123);

  // Expected exceptions also get thrown
  REQUIRE_THROWS_AS(handler.addMetaInfo(utils::ParticleIDMeta{"anotherAlgo", 321, {"param"}}), std::runtime_error);
  // No information about this meta data can be obtained
  REQUIRE_FALSE(handler.getParamIndex(321, "param").has_value());

  REQUIRE_THROWS_AS(handler.addMetaInfo(utils::ParticleIDMeta{"newAlgo", 42, {"PARAM"}}), std::runtime_error);
  // Existing meta info is unchanged
  REQUIRE_FALSE(handler.getParamIndex(42, "PARAM").has_value());
  REQUIRE(handler.getParamIndex(42, "p2").value_or(-1) == 1);
}

TEST_CASE("PIDHandler from Frame w/ metadata", "[pid_utils]") {
  using namespace edm4hep;
  const auto& [event, metadata] = createEventAndMetadata();

  const auto handler = utils::PIDHandler::from(event, metadata);

  const auto pidAlgo1 = handler.getAlgoType("pidAlgo_1").value();
  const auto pidAlgo2 = handler.getAlgoType("algo_2").value();
  REQUIRE(pidAlgo1 == 42);
  REQUIRE(pidAlgo2 == 123);
  REQUIRE_FALSE(handler.getAlgoType("non-existant-algo").has_value());

  // Check that getting a ParticleID object for a reconstructed particle via the
  // algorithmType works
  const auto& recos = event.get<edm4hep::ReconstructedParticleCollection>("reco_particles");
  const auto& pidColl1 = event.get<edm4hep::ParticleIDCollection>("particleIds_1");
  const auto& pidColl2 = event.get<edm4hep::ParticleIDCollection>("particleIds_2");
  const auto pid1 = handler.getPID(recos[0], pidAlgo1).value();
  REQUIRE(pid1 == pidColl1[0]);
  const auto pid2 = handler.getPID(recos[0], pidAlgo2).value();
  REQUIRE(pid2 == pidColl2[0]);
  REQUIRE_FALSE(handler.getPID(recos[0], -1).has_value()); // empty optional for non-existant algoType

  // Check that parameter handling works as well
  const auto parIndex1 = handler.getParamIndex(pidAlgo1, "first_param").value();
  REQUIRE(parIndex1 == 0);
  const auto parIndex2 = handler.getParamIndex(pidAlgo2, "2").value();
  REQUIRE(parIndex2 == 1);
  // Valid algo but invalid parameter name
  REQUIRE_FALSE(handler.getParamIndex(pidAlgo1, "non-existant-param").has_value());
  // Invalid algorithm, the parameter name is not even checked in this case
  REQUIRE_FALSE(handler.getParamIndex(-1, "doesn't matter").has_value());

  const auto pidInfo = utils::PIDHandler::getAlgoInfo(metadata, "particleIds_1").value();
  REQUIRE(pidInfo.algoName == "pidAlgo_1");
  REQUIRE(pidInfo.algoType == 42);
  REQUIRE(pidInfo.paramNames.size() == 2);
  REQUIRE(pidInfo.paramNames[0] == "first_param");
  REQUIRE(pidInfo.paramNames[1] == "second_param");
}

TEST_CASE("PIDHandler from Frame w/o metadata", "[pid_utils]") {
  using namespace edm4hep;
  const auto& [event, _] = createEventAndMetadata();

  const auto handler = utils::PIDHandler::from(event);
  // No metadata available info available in this case
  REQUIRE_FALSE(handler.getAlgoType("pidAlgo_1").has_value());

  // But the rest should still work as expected
  const auto& recoColl = event.get<edm4hep::ReconstructedParticleCollection>("reco_particles");

  SECTION("Valid PID for reco") {
    auto reco = recoColl[0];
    checkHandlerValidReco(handler, reco);
  }

  SECTION("Unknown reco") {
    const auto reco = edm4hep::ReconstructedParticle();
    const auto pids = handler.getPIDs(reco);

    REQUIRE(pids.empty());
  }
}
