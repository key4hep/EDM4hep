#ifndef EDM4HEP_CLUSTERRECONSTRUCTEDPARTICLELINKCOLLECTION_H
#define EDM4HEP_CLUSTERRECONSTRUCTEDPARTICLELINKCOLLECTION_H

#include <podio/LinkCollection.h>

#include <edm4hep/ReconstructedParticleCollection.h>
#include <edm4hep/VertexCollection.h>

namespace edm4hep {
using VertexRecoParticleLinkCollection = podio::LinkCollection<edm4hep::Vertex, edm4hep::ReconstructedParticle>;
} // namespace edm4hep

#endif // EDM4HEP_CLUSTERRECONSTRUCTEDPARTICLELINKCOLLECTION_H
