///  make sure that downstream projects can include and link edm4hep properly



#include "edm4hep/MCParticle.h"
#include "edm4hep/utils/kinematics.h"

#include "edm4dis/InclusiveKinematics.h"

int main() {

  auto a = edm4hep::MCParticle();
  const auto p4 = edm4hep::utils::p4(a);

  auto k = edm4dis::InclusiveKinematics();

  return 0;
}
