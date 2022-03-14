#ifndef EDM4HEP_UTILS_KINEMATICS_H
#define EDM4HEP_UTILS_KINEMATICS_H

#include "Math/Vector4D.h"

#include <cmath>

namespace edm4hep {
/**
 * A LorentzVector with (px, py, pz) and M
 */
using LorentzVectorM = ROOT::Math::PxPyPzMVector;

/**
 * A LorentzVector with (px, py, pz) and E
 */
using LorentzVectorE = ROOT::Math::PxPyPzEVector;

namespace utils {

/**
 * Get the transverse momentum from a Particle
 */
template<typename ParticleT>
inline float pT(ParticleT const& p) {
  return std::sqrt(p.getMomentum()[0] * p.getMomentum()[0] + p.getMomentum()[1] * p.getMomentum()[1]);
}

/**
 * Get the transverse momentum from a Particle
 */
template<typename ParticleT>
inline float pt(ParticleT const& p) {
  return pT(p);
}

/**
 * Get the total momentum from a Particle
 */
template<typename ParticleT>
inline float p(ParticleT const& part) {
  const auto mom = part.getMomentum();
  return std::sqrt(mom[0]*mom[0] + mom[1]*mom[1] + mom[2]*mom[2]);
}

namespace detail {
/**
 * Tag struct for getting 4-momenta with momentum + mass
 */
struct UseMassTag {
  using type = ::edm4hep::LorentzVectorM;
  static constexpr bool has_value = false;
};

/**
 * Tag struct for getting 4-momenta with momentum + energy
 */
struct UseEnergyTag {
  using type = ::edm4hep::LorentzVectorE;
  static constexpr bool has_value = false;
};

/**
 * Tag struct holding an additional value for getting 4 momenta with arbitrary
 * values instead of the one from the Particle.
 */
template<typename T, typename TagT>
struct TaggedUserValue {
  using type = typename TagT::type;
  static constexpr bool has_value = true;
  T value;
};

/**
 * Tag-dispatched implementation for getting the 4-momentum from a particle
 * according to the desired type.
 */
template<typename ParticleT, typename LorentzVectorTypeTag>
inline typename LorentzVectorTypeTag::type p4(ParticleT const& part,
                                              [[maybe_unused]] LorentzVectorTypeTag* tag) {
  const auto mom = part.getMomentum();
  // Either the user wants to set a specific value
  if constexpr(LorentzVectorTypeTag::has_value) {
    return typename LorentzVectorTypeTag::type{mom[0], mom[1], mom[2], tag->value};
  }

  // Or we take the one from the underying particle
  if constexpr(std::is_same_v<typename LorentzVectorTypeTag::type, LorentzVectorM>) {
    return LorentzVectorM{mom[0], mom[1], mom[2], part.getMass()};
  }
  if constexpr(std::is_same_v<typename LorentzVectorTypeTag::type, LorentzVectorE>) {
    return LorentzVectorE{mom[0], mom[1], mom[2], part.getEnergy()};
  }
}
} // namespace detail

using UseMassTag = detail::UseMassTag;
/**
 * Static tag to select the mass in 4 momentum vectors
 */
constexpr static UseMassTag UseMass;

using UseEnergyTag = detail::UseEnergyTag;
/**
 * Static tag to select the energy in 4 momentum vectors
 */
constexpr static UseEnergyTag UseEnergy;

/**
 * Class to inject a user-defined mass into 4 momentum vectors in the call to
 * p4
 */
using SetMass = detail::TaggedUserValue<float, UseMassTag>;

/**
 * Class to inject a user-defined energy into 4 momentum vectors in the call to
 * p4
 */
using SetEnergy = detail::TaggedUserValue<float, UseEnergyTag>;

/**
 * Get the 4 momentum vector from a Particle. By default using the momentum and
 * the mass, but can be switched to using the energy when using the UseEnergy as
 * second argument. Additionally it is possible to take the momentum from the
 * particle but set a specific mass or energy value by using SetMass or
 * SetEnergy as the second argument. The underlying particle will not be changed
 * in this case.
 */
template<typename ParticleT, typename LorentzVectorTag=UseMassTag>
inline typename LorentzVectorTag::type p4(ParticleT const& part, LorentzVectorTag tag=UseMass) {
  return detail::p4(part, &tag);
}


}} // namespace edm4hep::utils


#endif
