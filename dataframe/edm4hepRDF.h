#ifndef  RDF_UTILITYLAMBDAS_H
#define  RDF_UTILITYLAMBDAS_H

#include <cmath>
#include <vector>

#include "edm4hep/MCParticleData.h"
#include "edm4hep/SimTrackerHitData.h"
#include "edm4hep/Vector3f.h"
#include "edm4hep/Vector3d.h"


namespace edm4hep {

/// get transverse momenta (GeV) of a MCParticle collection
std::vector<float> pt (std::vector<MCParticleData> const& in);

/// get pseudorapidities of a MCParticle collection
std::vector<float> eta(std::vector<MCParticleData> const& in);

/// get cosines of theta of a MCParticle collection
std::vector<float> cos_theta(std::vector<MCParticleData> const& in);

/// get transverse radial distances of the coordinates of a SimTrackerHit Collection
std::vector<double> r (std::vector<SimTrackerHitData> const& in); 

}

#endif
