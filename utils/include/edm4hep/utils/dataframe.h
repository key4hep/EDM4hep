#ifndef EDM4HEP_UTILS_DATAFRAME_H
#define EDM4HEP_UTILS_DATAFRAME_H

#include "ROOT/RVec.hxx"

namespace edm4hep::utils {

/// Get the pt of the passed particle / datatypes
template <typename T>
ROOT::VecOps::RVec<float> pt(ROOT::VecOps::RVec<T> const& in);

/// Get eta of the passed particles / datatype momenta
template <typename T>
ROOT::VecOps::RVec<float> eta(ROOT::VecOps::RVec<T> const& in);

/// Get cos(theta) of the passed particles / datatype momenta
template <typename T>
ROOT::VecOps::RVec<float> cos_theta(ROOT::VecOps::RVec<T> const& in);

/// Get r of the passed particle / datatype positions
template <typename T>
ROOT::VecOps::RVec<float> r(ROOT::VecOps::RVec<T> const& in);

} // namespace edm4hep::utils

#endif // EDM4HEP_UTILS_DATAFRAME_H
