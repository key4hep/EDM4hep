#include "edm4hep/CalorimeterHitCollection.h"
#include "edm4hep/ClusterCollection.h"
#include "edm4hep/DatamodelDefinition.h"
#include "edm4hep/MCParticle.h"
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/SimCalorimeterHitCollection.h"
#include "edm4hep/SimTrackerHitCollection.h"
#include "edm4hep/TrackCollection.h"
#include "edm4hep/TrackerHit.h"
#include "edm4hep/VertexCollection.h"
#include "edm4hep/schema_evolution/OldLinkData.h"

#include <podio/CollectionBufferFactory.h>
#include <podio/CollectionBuffers.h>
#include <podio/DatamodelRegistry.h>
#include <podio/LinkCollection.h>
#include <podio/SchemaEvolution.h>

#include <string_view>

namespace edm4hep {
namespace {
  using namespace std::string_view_literals;

  /// Evolution function for doing "schema evolution" on generated links to make
  /// them consumable by templated links.
  ///
  /// This is mostly just redefining things from the oldBuffers in the new terms
  /// of the templated links. The most important bit is the re-definition of
  /// createCollection to actually create a templated LinkCollection
  template <typename FromT, typename ToT>
  auto evolveLinks(podio::CollectionReadBuffers buffers, podio::SchemaVersionT) {
    buffers.type = podio::LinkCollection<FromT, ToT>::typeName;
    buffers.schemaVersion = podio::LinkCollection<FromT, ToT>::schemaVersion;

    buffers.createCollection = [](const podio::CollectionReadBuffers& buffs, bool isSubsetColl) {
      podio::LinkCollectionData<FromT, ToT> data(buffs, isSubsetColl);
      return std::make_unique<podio::LinkCollection<FromT, ToT>>(std::move(data), isSubsetColl);
    };

    buffers.recast = [](podio::CollectionReadBuffers& buffs) {
      if (buffs.data) {
        buffs.data = podio::CollectionWriteBuffers::asVector<podio::LinkData>(buffs.data);
      }
    };

    buffers.deleteBuffers = [](podio::CollectionReadBuffers& buffs) {
      if (buffs.data) {
        delete static_cast<std::vector<podio::LinkData>*>(buffs.data);
      }
      delete buffs.references;
      delete buffs.vectorMembers;
    };

    return buffers;
  }

  /// Function factory for stamping out buffer creation functions that can be
  /// registered in the podio::CollectionBufferFactory to create buffers for
  /// collections that have been written with the generated Link collections.
  /// They will hit the buffer factory with the old type name so we have to make
  /// sure that the factory can deal with that. In this case we simply create
  /// the same buffers that we also do for templated Links.
  template <typename FromT, typename ToT>
  auto makeCreateLinkBufferFunction(std::string_view typeName) {
    auto createBuffers = [=](bool subsetColl) {
      auto readBuffers = podio::CollectionReadBuffers{};
      readBuffers.type = typeName;
      readBuffers.schemaVersion = 2;
      readBuffers.data = subsetColl ? nullptr : new podio::LinkDataContainer();

      // Either it is a subset collection or we have two relations
      const auto nRefs = subsetColl ? 1 : 2;
      readBuffers.references = new podio::CollRefCollection(nRefs);
      for (auto& ref : *readBuffers.references) {
        // Make sure to place usable buffer pointers here
        ref = std::make_unique<std::vector<podio::ObjectID>>();
      }

      readBuffers.createCollection = [](podio::CollectionReadBuffers buffers, bool isSubsetColl) {
        podio::LinkCollectionData<FromT, ToT> data(buffers, isSubsetColl);
        return std::make_unique<podio::LinkCollection<FromT, ToT>>(std::move(data), isSubsetColl);
      };

      readBuffers.recast = [](podio::CollectionReadBuffers& buffers) {
        if (buffers.data) {
          buffers.data = podio::CollectionWriteBuffers::asVector<float>(buffers.data);
        }
      };

      readBuffers.deleteBuffers = [](podio::CollectionReadBuffers& buffers) {
        if (buffers.data) {
          // If we have data then we are not a subset collection and we have
          // to clean up all type erased buffers by casting them back to
          // something that we can delete
          delete static_cast<podio::LinkDataContainer*>(buffers.data);
        }
        delete buffers.references;
        delete buffers.vectorMembers;
      };

      return readBuffers;
    };

    return createBuffers;
  }

  /// Register all the functions to the appropriate factories / registries in
  /// podio
  template <typename FromT, typename ToT>
  bool registerTransition(std::string_view collTypeName) {
    const static auto registerLinkReading = [=]() {
      podio::CollectionBufferFactory::mutInstance().registerCreationFunc(
          std::string(collTypeName), 2, makeCreateLinkBufferFunction<FromT, ToT>(collTypeName));

      podio::SchemaEvolution::mutInstance().registerEvolutionFunc(std::string(collTypeName), 2,
                                                                  edm4hep::meta::schemaVersion, evolveLinks<FromT, ToT>,
                                                                  podio::SchemaEvolution::Priority::UserDefined);

      return true;
    }();

    return registerLinkReading;
  }

  /// Small helper macro to avoid some of the cumbersome typing
#define REGISTER_EVOLUTION(NAME, FROM, TO)                                                                             \
  static const auto reg_##NAME = registerTransition<edm4hep::FROM, edm4hep::TO>("edm4hep::" #NAME "Collection"sv);

  REGISTER_EVOLUTION(RecoMCParticleLink, ReconstructedParticle, MCParticle)
  REGISTER_EVOLUTION(CaloHitMCParticleLink, CalorimeterHit, MCParticle)
  REGISTER_EVOLUTION(ClusterMCParticleLink, Cluster, MCParticle)
  REGISTER_EVOLUTION(TrackMCParticleLink, Track, MCParticle)
  REGISTER_EVOLUTION(CaloHitSimCaloHitLink, CalorimeterHit, SimCalorimeterHit)
  REGISTER_EVOLUTION(TrackerHitSimTrackerHitLink, TrackerHit, SimTrackerHit)
  REGISTER_EVOLUTION(VertexRecoParticleLink, Vertex, ReconstructedParticle)

  /// Another registration is necessary to get all the branch names correct
  bool registerRelationNames() {
    const static auto reg = []() {
      static const auto relNames = podio::RelationNameMapping{
          {"edm4hep::RecoMCParticleLink"sv, {"from"sv, "to"sv}, {}},
          {"edm4hep::CaloHitMCParticleLink"sv, {"from"sv, "to"sv}, {}},
          {"edm4hep::ClusterMCParticleLink"sv, {"from"sv, "to"sv}, {}},
          {"edm4hep::TrackMCParticleLink"sv, {"from"sv, "to"sv}, {}},
          {"edm4hep::CaloHitSimCaloHitLink"sv, {"from"sv, "to"sv}, {}},
          {"edm4hep::TrackerHitSimTrackerHitLink"sv, {"from"sv, "to"sv}, {}},
          {"edm4hep::VertexRecoParticleLink"sv, {"from"sv, "to"sv}, {}},
      };
      podio::DatamodelRegistry::mutInstance().registerDatamodel("edm4hep_link_metamodel", "{}", relNames);

      return true;
    }();

    return reg;
  }

  const static auto reg_LinkEvolution = registerRelationNames();
} // namespace

} // namespace edm4hep
