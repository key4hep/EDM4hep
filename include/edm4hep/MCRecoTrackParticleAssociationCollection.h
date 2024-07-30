#ifndef EDM4HEP_MCRecoTrackParticleAssociationCollection_H
#define EDM4HEP_MCRecoTrackParticleAssociationCollection_H

#include "edm4hep/MCRecoTrackParticleAssociation.h"
#include "edm4hep/MutableMCRecoTrackParticleAssociation.h"
#include "edm4hep/TrackMCParticleLinkCollection.h"

namespace edm4hep {
using MCRecoTrackParticleAssociationCollection [[deprecated("use TrackMCParticleLinkCollection instead")]] =
    edm4hep::TrackMCParticleLinkCollection;
}

#endif
