#ifndef EDM4HEP_MutableMCRecoTrackParticleAssociation_H
#define EDM4HEP_MutableMCRecoTrackParticleAssociation_H

#include "edm4hep/TrackMCParticleLink.h"

namespace edm4hep {
using MutableMCRecoTrackParticleAssociation [[deprecated("use MutableTrackMCParticleLink instead")]] =
    edm4hep::MutableTrackMCParticleLink;
}

#endif
