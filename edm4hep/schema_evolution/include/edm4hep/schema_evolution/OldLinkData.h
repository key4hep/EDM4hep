#ifndef EDM4HEP_SCHEMA_EVOLUTION_OLDLINKDATA_H
#define EDM4HEP_SCHEMA_EVOLUTION_OLDLINKDATA_H

#define MAKE_DATA_STRUCT(name)                                                                                         \
  struct name {                                                                                                        \
    float weight{};                                                                                                    \
  };

namespace edm4hep {

MAKE_DATA_STRUCT(RecoMCParticleLinkData)
MAKE_DATA_STRUCT(CaloHitMCParticleLinkData)
MAKE_DATA_STRUCT(ClusterMCParticleLinkData)
MAKE_DATA_STRUCT(TrackMCParticleLinkData)
MAKE_DATA_STRUCT(CaloHitSimCaloHitLinkData)
MAKE_DATA_STRUCT(TrackerHitSimTrackerHitLinkData)
MAKE_DATA_STRUCT(VertexRecoParticleLinkData)

} // namespace edm4hep

#undef MAKE_DATA_STRUCT

#endif // EDM4HEP_SCHEMA_EVOLUTION_OLDLINKDATA_H
