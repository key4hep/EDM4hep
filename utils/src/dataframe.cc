#include "edm4hep/utils/dataframe.h"
#include "edm4hep/utils/common.h"

#include "edm4hep/CalorimeterHitData.h"
#include "edm4hep/ClusterData.h"
#include "edm4hep/MCParticleData.h"
#include "edm4hep/ReconstructedParticleData.h"
#include "edm4hep/SimCalorimeterHitData.h"
#include "edm4hep/SimTrackerHitData.h"
#include "edm4hep/TrackerHitData.h"
#include "edm4hep/TrackerHitPlaneData.h"
#include "edm4hep/VertexData.h"

#include "Math/Vector3D.h"

namespace edm4hep::utils {

template <typename T>
ROOT::VecOps::RVec<float> pt(ROOT::VecOps::RVec<T> const& in) {
  return ROOT::VecOps::Map(
      in, [](const auto& p) { return std::sqrt(p.momentum.x * p.momentum.x + p.momentum.y + p.momentum.y); });
}

template <typename T>
ROOT::VecOps::RVec<float> eta(ROOT::VecOps::RVec<T> const& in) {
  return ROOT::VecOps::Map(in, [](const auto& p) {
    ROOT::Math::XYZVector lv{p.momentum.x, p.momentum.y, p.momentum.z};
    return lv.Eta();
  });
}

template <typename T>
ROOT::VecOps::RVec<float> cos_theta(ROOT::VecOps::RVec<T> const& in) {
  return ROOT::VecOps::Map(in, [](const auto& p) {
    ROOT::Math::XYZVector lv{p.momentum.x, p.momentum.y, p.momentum.z};
    return std::cos(lv.Theta());
  });
}

template <typename T>
ROOT::VecOps::RVec<float> r(ROOT::VecOps::RVec<T> const& in) {
  return ROOT::VecOps::Map(in, [](const auto& p) {
    return std::sqrt(p.position.x * p.position.x + p.position.y * p.position.y + p.position.z + p.position.z);
  });
}

template <typename T>
ROOT::VecOps::RVec<edm4hep::LorentzVectorM> p4M(ROOT::VecOps::RVec<T> const& in) {
  return ROOT::VecOps::Map(in, [](const auto& p) {
    return edm4hep::LorentzVectorM{p.momentum.x, p.momentum.y, p.momentum.z, p.mass};
  });
}

template <typename T>
ROOT::VecOps::RVec<edm4hep::LorentzVectorE> p4E(ROOT::VecOps::RVec<T> const& in) {
  return ROOT::VecOps::Map(in, [](const auto& p) {
    return edm4hep::LorentzVectorE{p.momentum.x, p.momentum.y, p.momentum.z, p.energy};
  });
}

template <typename T>
ROOT::VecOps::RVec<float> E(ROOT::VecOps::RVec<T> const& fourMom) {
  return ROOT::VecOps::Map(fourMom, [](const auto& p) { return p.E(); });
}

template <typename T>
ROOT::VecOps::RVec<float> M(ROOT::VecOps::RVec<T> const& fourMom) {
  return ROOT::VecOps::Map(fourMom, [](const auto& p) { return p.M(); });
}

// Explicitly instantiate the template functions here to have them available in
// the shared library and the dictionaries that will be compiled from this

// Macro to instantiate a function taking a vector<XYZData> and returning a vector<float>
#define INST_DATA_TO_FLOAT_VEC_FUNC(FUNC, DATATYPE)                                                                    \
  template ROOT::VecOps::RVec<float> FUNC(ROOT::VecOps::RVec<DATATYPE> const&)

// Macro to instantiate all momentum related functions for a datatype
#define INST_MOMENTUM_FUNCS(DATATYPE)                                                                                  \
  INST_DATA_TO_FLOAT_VEC_FUNC(pt, DATATYPE);                                                                           \
  INST_DATA_TO_FLOAT_VEC_FUNC(eta, DATATYPE);                                                                          \
  INST_DATA_TO_FLOAT_VEC_FUNC(cos_theta, DATATYPE)

INST_MOMENTUM_FUNCS(edm4hep::MCParticleData);
INST_MOMENTUM_FUNCS(edm4hep::ReconstructedParticleData);
INST_MOMENTUM_FUNCS(edm4hep::SimTrackerHitData);

#undef INST_MOMENTUM_FUNCS

// Macro to instantiate all position related functions for a datatype
#define INST_POSITION_FUNCS(DATATYPE) INST_DATA_TO_FLOAT_VEC_FUNC(r, DATATYPE)

INST_POSITION_FUNCS(edm4hep::SimTrackerHitData);
INST_POSITION_FUNCS(edm4hep::TrackerHitData);
INST_POSITION_FUNCS(edm4hep::TrackerHitPlaneData);
INST_POSITION_FUNCS(edm4hep::SimCalorimeterHitData);
INST_POSITION_FUNCS(edm4hep::CalorimeterHitData);
INST_POSITION_FUNCS(edm4hep::ClusterData);
INST_POSITION_FUNCS(edm4hep::VertexData);

#undef INST_POSITION_FUNCS

INST_DATA_TO_FLOAT_VEC_FUNC(E, edm4hep::LorentzVectorE);
INST_DATA_TO_FLOAT_VEC_FUNC(E, edm4hep::LorentzVectorM);
INST_DATA_TO_FLOAT_VEC_FUNC(M, edm4hep::LorentzVectorE);
INST_DATA_TO_FLOAT_VEC_FUNC(M, edm4hep::LorentzVectorM);

#undef INST_DATA_TO_FLOAT_VEC

#define INST_4MOM_MASS_FUNCS(DATATYPE)                                                                                 \
  template ROOT::VecOps::RVec<edm4hep::LorentzVectorM> p4M(ROOT::VecOps::RVec<DATATYPE> const&)

#define INST_4MOM_ENERGY_FUNCS(DATATYPE)                                                                               \
  template ROOT::VecOps::RVec<edm4hep::LorentzVectorE> p4E(ROOT::VecOps::RVec<DATATYPE> const&)

INST_4MOM_ENERGY_FUNCS(edm4hep::ReconstructedParticleData);
INST_4MOM_MASS_FUNCS(edm4hep::ReconstructedParticleData);
INST_4MOM_MASS_FUNCS(edm4hep::MCParticleData);

#undef INST_4MOM_ENERGY_FUNCS
#undef INST_4MOM_MASS_FUNCS

} // namespace edm4hep::utils
