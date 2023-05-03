///  make sure that downstream projects can include and link edm4hep properly

#include "edm4hep/EDM4hepVersion.h"
#include "edm4hep/MCParticle.h"
#include "edm4hep/utils/kinematics.h"

#include "edm4dis/InclusiveKinematics.h"

#if EDM4HEP_BUILD_VERSION < EDM4HEP_VERSION(0, 0, 0)
This is just a dummy test to make sure that the preprocessor macro can be used in a preprocessor context;
#endif

int main() {

  auto a = edm4hep::MCParticle();
  const auto p4 = edm4hep::utils::p4(a);

  auto k = edm4dis::InclusiveKinematics();

  return 0;
}
