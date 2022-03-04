#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "edm4hep/utils/kinematics.h"

#include "edm4hep/MutableMCParticle.h"
#include "edm4hep/MutableReconstructedParticle.h"

#include "Math/Vector4D.h"

#include <tuple>

using ParticleTypes = std::tuple<edm4hep::MutableMCParticle, edm4hep::MutableReconstructedParticle>;

TEMPLATE_LIST_TEST_CASE( "pT: transverse momentum", "[pT][kinematics]", ParticleTypes)
{
  using namespace edm4hep;
  TestType particle;

  particle.setMomentum({3.0f, 4.0f, 0.0f});
  REQUIRE(utils::pT(particle) == 5.0f);

  particle.setMomentum({3.0f, 4.0f, 125.0f});
  REQUIRE(utils::pT(particle) == 5.0f);

  particle.setMomentum({-3.0f, 4.0f, 10.0f});
  REQUIRE(utils::pt(particle) == 5.0f);

  particle.setMomentum({-4.0f, -3.0f, std::nanf("")});
  REQUIRE(utils::pt(particle) == 5.0f);

  particle.setMomentum({std::nanf(""), -3.0f, 0});
  REQUIRE(std::isnan(utils::pt(particle)));
}

TEMPLATE_LIST_TEST_CASE( "p: momentum", "[p][kinematics]", ParticleTypes )
{
  using namespace edm4hep;
  TestType particle;

  particle.setMomentum({1.0f, 2.0f, 3.0f});
  REQUIRE(utils::p(particle) == Catch::Approx(std::sqrt(14)));

  particle.setMomentum({1.0f, -2.0f, 3.0f});
  REQUIRE(utils::p(particle) == Catch::Approx(std::sqrt(14)));

  particle.setMomentum({1.0f, 2.0f, -3.0f});
  REQUIRE(utils::p(particle) == Catch::Approx(std::sqrt(14)));

  particle.setMomentum({-12.0f, 0.0f, 0.0f});
  REQUIRE(utils::p(particle) == 12.0f);

  particle.setMomentum({0.0f, -10.0f, -10.f});
  REQUIRE(utils::p(particle) == Catch::Approx(std::sqrt(200.0)));

  particle.setMomentum({std::nanf(""), 2.0f, 3.0f});
  REQUIRE(std::isnan(utils::p(particle)));
}

TEST_CASE( "p4: four momentum - MCParticle", "[p4][kinematics][MCParticle]" )
{
  using namespace edm4hep;
  MutableMCParticle particle;
  particle.setMomentum({1.0f, 2.0f, 3.0f});
  particle.setMass(42);
  REQUIRE(utils::p4(particle) == LorentzVectorM{1, 2, 3, 42});

  // this basically just tests that the internal calculation of the energy works as expected
  REQUIRE(utils::p4(particle, utils::UseEnergy) == LorentzVectorE{1, 2, 3, std::sqrt(14 + 42*42)});
}

TEST_CASE( "p4: four momentum - ReconstructedParticle", "[p4][kinematics][ReconstructedParticle]" )
{
  using namespace edm4hep;
  MutableReconstructedParticle particle;
  particle.setMomentum({1.0f, 2.0f, 3.0f});
  particle.setMass(125);

  // By default we use the mass
  REQUIRE(utils::p4(particle) == LorentzVectorM{1, 2, 3, 125});

  // In the case of ReconstructedParticle, the 4-momentum state is not kept
  // consistent internally! So, when using the energy we will have a different 4
  // momentum vector
  REQUIRE(utils::p4(particle, utils::UseEnergy) == LorentzVectorE{1, 2, 3, 0});

  // Setting the Energy does not affect the mass
  particle.setEnergy(42);
  REQUIRE(utils::p4(particle, utils::UseMass) == LorentzVectorM{1, 2, 3, 125});
  REQUIRE(utils::p4(particle, utils::UseEnergy) == LorentzVectorE{1, 2, 3, 42});
}

TEST_CASE( "p4 with user set values", "[p4][kinematics][user set values]" ) {
  using namespace edm4hep;
  MutableReconstructedParticle particle;
  particle.setMomentum({1.0f, 2.0f, 3.0f});
  particle.setMass(125.0f);
  particle.setEnergy(42.0f);

  // Requiring a dedicated mass value gives us a 4-vector with that mass value
  REQUIRE(utils::p4(particle, utils::SetMass{3.096f}) == LorentzVectorM{1.0f, 2.0f, 3.0f, 3.096f});
  // The mass of the particle itself remains unchanged!
  REQUIRE(particle.getMass() == 125.f);

  // Similar with the energy, if we want a dedicated value we get it in the 4 vector
  REQUIRE(utils::p4(particle, utils::SetEnergy{1.23f}) == LorentzVectorE{1.0f, 2.0f, 3.0f, 1.23f});
  // But the underlying particle energy remains unchanged
  REQUIRE(particle.getEnergy() == 42.f);

  // Make sure everything still works with the MC particle
  MutableMCParticle mcPart;
  mcPart.setMomentum({-1.0f, -2.0f, -3.0f});
  mcPart.setMass(125.0f);

  REQUIRE(utils::p4(mcPart, utils::SetMass{3.096f}) == LorentzVectorM{-1.0f, -2.0f, -3.0f, 3.096f});
  REQUIRE(mcPart.getMass() == 125.0f); // mass remains unchanged

  // everything needs to work with const classes as well
  const auto mcPart2 = mcPart;
  REQUIRE(utils::p4(mcPart2, utils::SetMass{3.096f}) == LorentzVectorM{-1.0f, -2.0f, -3.0f, 3.096f});
}
