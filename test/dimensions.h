#ifndef EDM4HEP_TEST_DIMENSIONS_H
#define EDM4HEP_TEST_DIMENSIONS_H

#include "edm4hep/detail/enum_access.h"

enum Cartesian : edm4hep::DimType { X = 0, Y = 1, Z = 2 };

enum class Polar : edm4hep::DimType { R = 0, PHI, THETA };

#endif // EDM4HEP_TEST_DIMENSIONS_H
