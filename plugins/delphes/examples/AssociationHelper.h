#ifndef EDM4HEP__ASSOCIATIONHELPER_H_
#define EDM4HEP__ASSOCIATIONHELPER_H_

#include "edm4hep/MCRecoParticleAssociationCollection.h"
#include "edm4hep/MCParticle.h"
#include "edm4hep/ReconstructedParticle.h"

#include <vector>

class MCRecoParticleAssociationNavigator {
    public:
        MCRecoParticleAssociationNavigator(edm4hep::MCRecoParticleAssociationCollection const& assocColl)
            : m_associationColl(assocColl) {}

        std::vector<edm4hep::ConstMCParticle> getSim(edm4hep::ConstReconstructedParticle const& recoParticle) const;

    private:
        const edm4hep::MCRecoParticleAssociationCollection& m_associationColl;
        // TODO: cache already looked up particles
        // TODO: reverse relations
};


std::vector<edm4hep::ConstMCParticle> MCRecoParticleAssociationNavigator::getSim(edm4hep::ConstReconstructedParticle const& recoParticle) const
{
    std::vector<edm4hep::ConstMCParticle> relatedParticles;
    for (const auto& relation : m_associationColl) {
        if (relation.getRec() == recoParticle) {
            relatedParticles.push_back(relation.getSim());
        }
    }

    return relatedParticles;
}

#endif // EDM4HEP__ASSOCIATIONHELPER_H_
