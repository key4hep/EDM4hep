#ifndef EDM4HEP_UTILS_COMMON_H
#define EDM4HEP_UTILS_COMMON_H

#include "Math/Vector4D.h"

namespace edm4hep {
/**
 * A LorentzVector with (px, py, pz) and M
 */
using LorentzVectorM = ROOT::Math::PxPyPzMVector;

/**
 * A LorentzVector with (px, py, pz) and E
 */
using LorentzVectorE = ROOT::Math::PxPyPzEVector;

} // namespace edm4hep

#endif
