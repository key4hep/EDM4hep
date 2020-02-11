#ifndef  RDF_UTILITYLAMBDAS_H
#define  RDF_UTILITYLAMBDAS_H

#include <cmath>
#include <vector>

#include "edm4hep/MCParticleData.h"
#include "edm4hep/SimTrackerHitData.h"
#include "edm4hep/Vector3f.h"
#include "edm4hep/Vector3d.h"


namespace edm4hep {

std::vector<float> pt (std::vector<MCParticleData> const& in);

std::vector<float> eta(std::vector<MCParticleData> const& in);

std::vector<float> cos_theta(std::vector<MCParticleData> const& in);

std::vector<double> r (std::vector<SimTrackerHitData> const& in); 

}

#endif
