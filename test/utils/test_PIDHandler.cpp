#include <edm4hep/utils/ParticleIDUtils.h>

#include <edm4hep/ParticleIDCollection.h>
#include <edm4hep/ReconstructedParticleCollection.h>

#include <catch2/catch_test_macros.hpp>

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
  }

  return coll;
}

void checkHandlerValidReco(const edm4hep::utils::PIDHandler& handler, const edm4hep::ReconstructedParticle& reco) {
  const auto pids = handler.getPIDs(reco);

  REQUIRE(pids.size() == 2);
  REQUIRE(pids[0].getParticle() == reco);
  REQUIRE(pids[1].getParticle() == reco);
  REQUIRE(pids[0].getLikelihood() == 1.0f);
  REQUIRE(pids[1].getLikelihood() == 2.0f);
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

  SECTION("Unkown reco") {
    const auto reco = edm4hep::ReconstructedParticle();
    const auto pids = handler.getPIDs(reco);

    REQUIRE(pids.empty());
  }
}
