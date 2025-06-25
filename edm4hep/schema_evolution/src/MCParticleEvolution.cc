#include "edm4hep/MCParticleData.h"
#include <edm4hep/DatamodelDefinition.h>
#include <edm4hep/MCParticleCollection.h>
#include <edm4hep/MCParticleCollectionData.h>
#include <edm4hep/schema_evolution/OldMCParticleData.h>

#include <podio/CollectionBufferFactory.h>
#include <podio/CollectionBuffers.h>
#include <podio/SchemaEvolution.h>

#include <iostream>
#include <memory>
#include <string_view>

namespace edm4hep {
namespace {
  auto evolveSpinHelicity(podio::CollectionReadBuffers buffers, podio::SchemaVersionT fromVersion) {
    if (fromVersion > 2) {
      return buffers;
    }
    buffers.schemaVersion = edm4hep::meta::schemaVersion;

    // We only have to evolve the data, all other members are already created
    // correctly for working after schema evolution below
    auto* oldData = podio::CollectionWriteBuffers::asVector<edm4hep::v2::MCParticleData>(buffers.data);
    auto* newData = new std::vector<edm4hep::MCParticleData>();
    newData->reserve(oldData->size());
    for (const auto& oldMC : *oldData) {
      newData->emplace_back(oldMC.PDG, oldMC.generatorStatus, oldMC.simulatorStatus, oldMC.charge, oldMC.time,
                            oldMC.mass, oldMC.vertex, oldMC.endpoint, oldMC.momentum, oldMC.momentumAtEndpoint,
                            int(oldMC.spin.z), oldMC.parents_begin, oldMC.parents_end, oldMC.daughters_begin,
                            oldMC.daughters_end);
    }
    delete oldData;
    buffers.data = newData;

    return buffers;
  }

  auto createOldBufferFunction(bool isSubset) {
    auto readBuffers = podio::CollectionReadBuffers{};
    readBuffers.type = "edm4hep::MCParticleCollection";
    readBuffers.schemaVersion = 2;
    readBuffers.data = isSubset ? nullptr : new std::vector<edm4hep::v2::MCParticleData>;
    // The number of ObjectID vectors is either 1 or the sum of OneToMany and
    // OneToOne relations
    const auto nRefs = isSubset ? 1 : 2 + 0;
    readBuffers.references = new podio::CollRefCollection(nRefs);
    for (auto& ref : *readBuffers.references) {
      // Make sure to place usable buffer pointers here
      ref = std::make_unique<std::vector<podio::ObjectID>>();
    }

    readBuffers.vectorMembers = new podio::VectorMembersInfo();
    if (!isSubset) {
      readBuffers.vectorMembers->reserve(0);
    }

    readBuffers.createCollection = [](const podio::CollectionReadBuffers& buffers, bool isSubsetColl) {
      MCParticleCollectionData data(buffers, isSubsetColl);
      return std::make_unique<MCParticleCollection>(std::move(data), isSubsetColl);
    };

    readBuffers.recast = [](podio::CollectionReadBuffers& buffers) {
      // We only have any of these buffers if this is not a subset collection
      if (buffers.data) {
        buffers.data = podio::CollectionWriteBuffers::asVector<edm4hep::v2::MCParticleData>(buffers.data);
      }
    };

    readBuffers.deleteBuffers = [](podio::CollectionReadBuffers& buffers) {
      if (buffers.data) {
        // If we have data then we are not a subset collection and we have to
        // clean up all type erased buffers by casting them back to something that
        // we can delete. We have also not undergone schema evolution
        delete static_cast<std::vector<edm4hep::v2::MCParticleData>*>(buffers.data);
      }
      delete buffers.references;
      delete buffers.vectorMembers;
    };

    return readBuffers;
  };

  bool registerTransition() {
    const static auto registerEvoFuncs = []() {
      std::cout << "HERE I AM REGISTERING FUNCTIONS AND STUFF\n";
      podio::CollectionBufferFactory::mutInstance().registerCreationFunc("edm4hep::MCParticleCollection", 2,
                                                                         createOldBufferFunction);

      podio::SchemaEvolution::mutInstance().registerEvolutionFunc("edm4hep::MCParticleCollection", 2,
                                                                  edm4hep::meta::schemaVersion, evolveSpinHelicity,
                                                                  podio::SchemaEvolution::Priority::UserDefined);
      return true;
    }();

    return registerEvoFuncs;
  }

  const static auto reg_MCParticleSpinHelicityEvo = registerTransition();

} // namespace
} // namespace edm4hep
