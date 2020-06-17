#include "DelphesEDM4HepConverter.h"
#include "delphesHelpers.h" // getAllParticleIds

#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/TrackCollection.h"
#include "edm4hep/ClusterCollection.h"
#include "edm4hep/MCRecoParticleAssociationCollection.h"
#include "edm4hep/RecoParticleRefCollection.h"
#include "edm4hep/ParticleIDCollection.h"

#include <string_view>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <iterator>

template<size_t N>
void sortBranchesProcessingOrder(std::vector<BranchSettings>& branches,
                                 std::array<std::string_view, N> const& processingOrder);

edm4hep::Track convertTrack(Candidate const* cand, const double magFieldBz);

/**
 * Simple helper function to make it easier to refactor later
 */
template<typename Container>
inline bool contains(Container const& container, typename Container::value_type const& value)
{
  return std::find(container.cbegin(), container.cend(), value) != container.cend();
}

DelphesEDM4HepConverter::DelphesEDM4HepConverter(std::string const& outputFile, ExRootConfParam /*const*/& branches,
                                                 ExRootConfReader* confReader) :
  m_writer(outputFile, &m_store),
  m_magneticFieldBz(confReader->GetDouble("ParticlePropagator::Bz", 0))
{
  for (int b = 0; b < branches.GetSize(); b += 3) {
    BranchSettings branch{branches[b].GetString(),
                          branches[b + 1].GetString(),
                          branches[b + 2].GetString()};
    if (contains(PROCESSING_ORDER, branch.className)) {
      m_branches.push_back(branch);
    }

  }

  sortBranchesProcessingOrder(m_branches, PROCESSING_ORDER);

  std::unordered_map<std::string, ProcessFunction> refProcessFunctions = {
    {"Photon", &DelphesEDM4HepConverter::processPhotons},
    {"Muon", &DelphesEDM4HepConverter::processMuonsElectrons},
    {"Electron", &DelphesEDM4HepConverter::processMuonsElectrons}};

  for (const auto& branch : m_branches) {
    if (contains(MCPARTICLE_OUTPUT, branch.className.c_str())) {
      registerCollection<edm4hep::MCParticleCollection>(branch.name);
      m_processFunctions.emplace(branch.name, &DelphesEDM4HepConverter::processParticles);
    }

    if (contains(RECOPARTICLE_OUTPUT, branch.name.c_str()) &&
        contains(RECO_TRACK_OUTPUT, branch.className.c_str())) {
      registerGlobalCollections();
      registerCollection<edm4hep::TrackCollection>(branch.name);
      m_processFunctions.emplace(branch.name, &DelphesEDM4HepConverter::processTracks);
    }

    if (contains(RECOPARTICLE_OUTPUT, branch.name.c_str()) &&
        contains(RECO_CLUSTER_OUTPUT, branch.className.c_str())) {
      registerGlobalCollections();
      registerCollection<edm4hep::ClusterCollection>(branch.name);
      m_processFunctions.emplace(branch.name, &DelphesEDM4HepConverter::processClusters);
    }

    if (contains(JET_COLLECTIONS, branch.name.c_str())) {
      registerCollection<edm4hep::ReconstructedParticleCollection>(branch.name);
      m_processFunctions.emplace(branch.name, &DelphesEDM4HepConverter::processJets);
    }

    if (contains(MUON_COLLECTIONS, branch.name.c_str()) ||
        contains(ELECTRON_COLLECTIONS, branch.name.c_str()) ||
        contains(PHOTON_COLLECTIONS, branch.name.c_str())) {
      registerCollection<edm4hep::RecoParticleRefCollection>(branch.name);
      m_processFunctions.emplace(branch.name, refProcessFunctions[branch.className]);
    }

    if (contains(MISSINGET_COLLECTIONS, branch.name.c_str())) {
      registerCollection<edm4hep::ReconstructedParticleCollection>(branch.name);
      m_processFunctions.emplace(branch.name, &DelphesEDM4HepConverter::processMissingET);
    }

    if (contains(SCALARHT_COLLECTIONS, branch.name.c_str())) {
      registerCollection<edm4hep::ParticleIDCollection>(branch.name);
      m_processFunctions.emplace(branch.name, &DelphesEDM4HepConverter::processScalarHT);
    }
  }
}

void DelphesEDM4HepConverter::process(Delphes *modularDelphes)
{
  m_store.clearCollections();
  m_genParticleIds.clear();
  m_recoParticleGenIds.clear();

  for (const auto& branch : m_branches) {
    const auto* delphesCollection = modularDelphes->ImportArray(branch.input.c_str());
    // at this point is not guaranteed that all entries in branch (which follow
    // the input from the delphes card) are also present in the processing
    // functions. Checking this here, basically allows us to skip these checks
    // in all called processing functions, since whatever is accessed there will
    // also be in the collection map, since that is filled with the same keys as
    // the processing function map
    const auto processFuncIt = m_processFunctions.find(branch.name);
    if (processFuncIt != m_processFunctions.end()) {
      (this->*processFuncIt->second)(delphesCollection, branch.name);
    }
  }
}

void DelphesEDM4HepConverter::processParticles(const TObjArray* delphesCollection, std::string_view const branch)
{
  auto* collection = static_cast<edm4hep::MCParticleCollection*>(m_collections[branch]);
  for (int iCand = 0; iCand < delphesCollection->GetEntriesFast(); ++iCand) {
    auto* delphesCand = static_cast<Candidate*>(delphesCollection->At(iCand));

    auto cand = collection->create();
    cand.setCharge(delphesCand->Charge);
    cand.setMass(delphesCand->Mass);
    cand.setMomentum({
      (float) delphesCand->Momentum.Px(),
      (float) delphesCand->Momentum.Py(),
      (float) delphesCand->Momentum.Pz()
    });
    cand.setVertex({(float) delphesCand->Position.X(),
        (float) delphesCand->Position.Y(),
        (float) delphesCand->Position.Z()});
    cand.setPDG(delphesCand->PID); // delphes uses whatever hepevt.idhep provides
    // TODO: - status
    // TODO: - ...

    if (const auto [it, inserted] = m_genParticleIds.emplace(delphesCand->GetUniqueID(), cand); !inserted) {
      std::cerr << "**** WARNING: UniqueID " << delphesCand->GetUniqueID() << " is already used by MCParticle with id: " << it->second.id() << std::endl;
    }
  }

  // mother-daughter relations
  for (int iCand = 0; iCand < delphesCollection->GetEntriesFast(); ++iCand) {
    auto* delphesCand = static_cast<Candidate*>(delphesCollection->At(iCand));
    auto cand = collection->at(iCand);

    if (delphesCand->M1 > -1) {
      auto mother = collection->at(delphesCand->M1);
      cand.addToParents(mother);
      mother.addToDaughters(cand);
    }
    if (delphesCand->M2 > -1) {
      auto mother = collection->at(delphesCand->M2);
      cand.addToParents(mother);
      mother.addToDaughters(cand);
    }
  }

}

void DelphesEDM4HepConverter::processTracks(const TObjArray* delphesCollection, std::string_view const branch)
{
  auto* particleCollection = static_cast<edm4hep::ReconstructedParticleCollection*>(m_collections[RECOPARTICLE_COLLECTION_NAME]);
  auto* trackCollection = static_cast<edm4hep::TrackCollection*>(m_collections[branch]);
  auto* mcRecoRelations = static_cast<edm4hep::MCRecoParticleAssociationCollection*>(m_collections[MCRECO_ASSOCIATION_COLLECTION_NAME]);

  for (auto iCand = 0; iCand < delphesCollection->GetEntriesFast(); ++iCand) {
    auto* delphesCand = static_cast<Candidate*>(delphesCollection->At(iCand));

    auto track = convertTrack(delphesCand, m_magneticFieldBz);
    trackCollection->push_back(track);

    auto cand = particleCollection->create();
    cand.setCharge(delphesCand->Charge);
    cand.setMomentum({(float) delphesCand->Momentum.Px(),
                      (float) delphesCand->Momentum.Py(),
                      (float) delphesCand->Momentum.Pz()});
    cand.setMass(delphesCand->Mass);

    cand.addToTracks(track);

    UInt_t genId = static_cast<Candidate*>(delphesCand->GetCandidates()->At(0))->GetUniqueID();
    if (const auto genIt = m_genParticleIds.find(genId); genIt != m_genParticleIds.end()) {
      auto relation = mcRecoRelations->create();
      relation.setSim(genIt->second);
      relation.setRec(cand);
    }

    m_recoParticleGenIds.emplace(genId, cand);
  }
}

void DelphesEDM4HepConverter::processClusters(const TObjArray* delphesCollection, std::string_view const branch)
{
  auto* particleCollection = static_cast<edm4hep::ReconstructedParticleCollection*>(m_collections[RECOPARTICLE_COLLECTION_NAME]);
  auto* clusterCollection = static_cast<edm4hep::ClusterCollection*>(m_collections[branch]);
  auto* mcRecoRelations = static_cast<edm4hep::MCRecoParticleAssociationCollection*>(m_collections[MCRECO_ASSOCIATION_COLLECTION_NAME]);

  for (auto iCand = 0; iCand < delphesCollection->GetEntriesFast(); ++iCand) {
    auto* delphesCand = static_cast<Candidate*>(delphesCollection->At(iCand));

    auto cluster = clusterCollection->create();
    cluster.setEnergy(delphesCand->Momentum.E());
    cluster.setPosition({(float) delphesCand->Position.X(),
                         (float) delphesCand->Position.Y(),
                         (float) delphesCand->Position.Z()});
    // TODO: time? (could be stored in a CalorimeterHit)
    // TODO: mc relations? would definitely need a CalorimeterHit for that
    //
    // TODO: Potentially every delphes tower could be split into two
    // edm4hep::clusters, with energies split according to Eem and Ehad. But
    // that would probably make the matching that is done below much harder

    auto cand = particleCollection->create();
    cand.setCharge(delphesCand->Charge);
    cand.setMomentum({(float) delphesCand->Momentum.Px(),
                      (float) delphesCand->Momentum.Py(),
                      (float) delphesCand->Momentum.Pz()});
    cand.setEnergy(delphesCand->Momentum.E());

    cand.addToClusters(cluster);

    for (const auto genId : getAllParticleIDs(delphesCand)) {
      if (const auto genIt = m_genParticleIds.find(genId); genIt != m_genParticleIds.end()) {
        auto relation = mcRecoRelations->create();
        relation.setSim(genIt->second);
        relation.setRec(cand);
      }

      m_recoParticleGenIds.emplace(genId, cand);
    }

  }
}

void DelphesEDM4HepConverter::processJets(const TObjArray* delphesCollection, std::string_view const branch)
{
  auto* jetCollection = static_cast<edm4hep::ReconstructedParticleCollection*>(m_collections[branch]);

  for (auto iCand = 0; iCand < delphesCollection->GetEntriesFast(); ++iCand) {
    auto* delphesCand = static_cast<Candidate*>(delphesCollection->At(iCand));
    auto jet = jetCollection->create();

    // NOTE: Filling the jet with the information delievered by Delphes, which
    // is not necessarily the same as the sum of its constituents (filled below)
    jet.setCharge(delphesCand->Charge);
    jet.setMomentum({(float) delphesCand->Momentum.Px(),
                     (float) delphesCand->Momentum.Py(),
                     (float) delphesCand->Momentum.Pz()});
    jet.setEnergy(delphesCand->Momentum.E());

    const auto* constituents = delphesCand->GetCandidates();
    for (auto iConst = 0; iConst < constituents->GetEntriesFast(); ++iConst) {
      auto* constituent = static_cast<Candidate*>(constituents->At(iConst));

      const auto genIds = getAllParticleIDs(constituent);
      for (const auto genId : genIds) {
        const auto [recoBegin, recoEnd] = m_recoParticleGenIds.equal_range(genId);

        if (recoBegin == m_recoParticleGenIds.end() && recoEnd == m_recoParticleGenIds.end()) {
          std::cerr << "**** WARNING: No reconstructed particles were found for genParticle UniqueID " << genId << ", which is a jet constituent" << std::endl;
        }

        for (auto it = recoBegin; it != recoEnd; ++it) {
          jet.addToParticles(it->second);
        }
      }
    }
  }
}

void DelphesEDM4HepConverter::processPhotons(const TObjArray* delphesCollection, std::string_view const branch)
{
  auto* collection = static_cast<edm4hep::RecoParticleRefCollection*>(m_collections[branch]);

  for (auto iCand = 0; iCand < delphesCollection->GetEntriesFast(); ++iCand) {
    auto* delphesCand = static_cast<Candidate*>(delphesCollection->At(iCand));
    auto photonRef = collection->create();

    const auto genIds = getAllParticleIDs(delphesCand);
    if (genIds.empty() || genIds.size() > 1) {
      std::cerr << "**** WARNING: Delphes algorithm did not return a unique generated particle for a photon" << std::endl;
      continue;
    }

    const auto genId = *genIds.begin();
    const auto [recoBegin, recoEnd] = m_recoParticleGenIds.equal_range(genId);
    if (std::distance(recoBegin, recoEnd) > 1) {
      std::cerr << "**** WARNING: More than one reconstructed particle attached to the genParticle UniqueID " << genId << ", which is supposed to be a photon" << std::endl;
      continue;
    }
    if (recoBegin == m_recoParticleGenIds.end() && recoEnd == m_recoParticleGenIds.end()) {
      std::cerr << "**** WARNING: Could not find a reconstructed particle attached to the genParticle UniqueID " << genId << ", which is supposed to be a photon" << std::endl;
      continue;
    }
    photonRef.setParticle(recoBegin->second);
  }
}

void DelphesEDM4HepConverter::processMuonsElectrons(const TObjArray* delphesCollection, std::string_view const branch)
{
  auto* collection = static_cast<edm4hep::RecoParticleRefCollection*>(m_collections[branch]);

  for (auto iCand = 0; iCand < delphesCollection->GetEntriesFast(); ++iCand) {
    auto* delphesCand = static_cast<Candidate*>(delphesCollection->At(iCand));
    auto particleRef = collection->create();

    const auto genId = delphesCand->GetCandidates()->At(0)->GetUniqueID();
    const auto [recoBegin, recoEnd] = m_recoParticleGenIds.equal_range(genId);
    if (std::distance(recoBegin, recoEnd) > 1) {
      std::cerr << "**** WARNING: More than one reconstructed particle attached to the genParticle UniqueID " << genId << ", which is supposed to be a muon or an electron" << std::endl;
      continue;
    }
    if (recoBegin == m_recoParticleGenIds.end() && recoEnd == m_recoParticleGenIds.end()) {
      std::cerr << "**** WARNING: Could not find a reconstructed particle attached to the genParticle UniqueID " << genId << ", which is supposed to be muon or an electron" << std::endl;
      continue;
    }
    particleRef.setParticle(recoBegin->second);
  }

}

void DelphesEDM4HepConverter::processMissingET(const TObjArray* delphesCollection, std::string_view const branch)
{
  auto* collection = static_cast<edm4hep::ReconstructedParticleCollection*>(m_collections[branch]);
  auto* delphesCand = static_cast<Candidate*>(delphesCollection->At(0));

  auto cand = collection->create();
  cand.setMomentum({(float) delphesCand->Momentum.Px(),
                    (float) delphesCand->Momentum.Px(),
                    (float) delphesCand->Momentum.Px()});
  cand.setMass(delphesCand->Mass);
}

void DelphesEDM4HepConverter::processScalarHT(const TObjArray* delphesCollection, std::string_view const branch)
{
  auto* collection = static_cast<edm4hep::ParticleIDCollection*>(m_collections[branch]);
  auto* delphesCand = static_cast<Candidate*>(delphesCollection->At(0));

  auto cand = collection->create();
  cand.addToParameters(delphesCand->Momentum.Pt());
}


void DelphesEDM4HepConverter::writeEvent()
{
  m_writer.writeEvent();
  m_store.clearCollections();
}

void DelphesEDM4HepConverter::finish()
{
  m_writer.finish();
}

void DelphesEDM4HepConverter::registerGlobalCollections()
{
  // Make sure that these are only registered once
  if (m_collections.find(RECOPARTICLE_COLLECTION_NAME) == m_collections.end()) {
    registerCollection<edm4hep::ReconstructedParticleCollection>(RECOPARTICLE_COLLECTION_NAME);
  }
  if (m_collections.find(MCRECO_ASSOCIATION_COLLECTION_NAME) == m_collections.end()) {
    registerCollection<edm4hep::MCRecoParticleAssociationCollection>(MCRECO_ASSOCIATION_COLLECTION_NAME);
  }
}


template<size_t N>
void sortBranchesProcessingOrder(std::vector<BranchSettings>& branches,
                                 std::array<std::string_view, N> const& processingOrder)
{
  std::unordered_map<std::string_view, size_t> classSortIndices;
  for (size_t i = 0; i < processingOrder.size(); ++i) {
    classSortIndices.emplace(processingOrder[i], i);
  }

  const auto endIt = classSortIndices.end();
  std::sort(branches.begin(), branches.end(),
            [&classSortIndices, endIt] (const auto& left, const auto& right) {
              const auto leftIt = classSortIndices.find(left.className);
              const auto rightIt = classSortIndices.find(right.className);

              // if we have the class defined in the processing order use the
              // corresponding index, otherwise use one definitely not inside
              // the processing order
              return (leftIt != endIt ? leftIt->second : N + 1) < (rightIt != endIt ? rightIt->second : N + 1);
            });
}


edm4hep::Track convertTrack(Candidate const* cand, const double magFieldBz)
{
  edm4hep::Track track;
  // Delphes does not really provide any information that would go into the
  // track itself. But some information can be used to at least partially
  // populate a TrackState
  edm4hep::TrackState trackState{};
  trackState.D0 = cand->D0;
  trackState.Z0 = cand->DZ;

  // Delphes calculates this from the momentum 4-vector at the track
  // perigee so this should be what we want. Note that this value
  // only undergoes smearing in the TrackSmearing module but e.g.
  // not in the MomentumSmearing module
  trackState.phi = cand->Phi;
  // Same thing under different name in Delphes
  trackState.tanLambda = cand->CtgTheta;
  // Only do omega when there is actually a magnetic field.
  double varOmega = 0;
  if (magFieldBz) {
    // conversion to have omega in [1/mm]
    // TODO: define this globally somewhere?
    constexpr double cLight = 299792458;
    constexpr double a = cLight * 1e3 * 1e-15;

    trackState.omega = a * magFieldBz / cand->PT * std::copysign(1.0, cand->Charge);
    // calculate variation using simple error propagation, assuming
    // constant B-field -> relative error on pT is relative error on omega
    varOmega = cand->ErrorPT * cand->ErrorPT / cand->PT / cand->PT * trackState.omega * trackState.omega;
  }

  // fill the covariance matrix. Indices on the diagonal are 0, 5,
  // 9, 12, and 14, corresponding to D0, phi, omega, Z0 and
  // tan(lambda) respectively. Currently Delphes doesn't provide
  // correlations
  auto& covMatrix = trackState.covMatrix;
  covMatrix[0] = cand->ErrorD0 * cand->ErrorD0;
  covMatrix[5] = cand->ErrorPhi * cand->ErrorPhi;
  covMatrix[9] = varOmega;
  covMatrix[12] = cand->ErrorDZ * cand->ErrorDZ;
  covMatrix[14] = cand->ErrorCtgTheta * cand->ErrorCtgTheta;

  track.addToTrackStates(trackState);

  return track;
}
