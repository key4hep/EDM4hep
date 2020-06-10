#ifndef DELPHESEDM4HEP_DELPHESHELPERS
#define DELPHESEDM4HEP_DELPHESHELPERS

#include "classes/DelphesClasses.h"

#include "TObjArray.h"

#include <vector>
#include <iostream>
#include <utility>
#include <map>

UInt_t getGenParticleUniqueID(const Candidate* genCand) {
  return genCand->GetUniqueID();
}

UInt_t getCandidateUniqueGenID(Candidate* cand) {
  return static_cast<Candidate*>(cand->GetCandidates()->At(0))->GetUniqueID();
}


class DelphesUniqueIDGenMatcher {
public:
  DelphesUniqueIDGenMatcher() = default;

  template<typename UniqueIdF>
  DelphesUniqueIDGenMatcher(const TObjArray* matchToArray, UniqueIdF getUniqueID) {
    const auto genSize = matchToArray->GetEntriesFast();
    for (int i = 0; i < genSize; ++i) {
      auto* genCand = static_cast<Candidate*>(matchToArray->At(i));
      addCandidate(genCand, i, getUniqueID);
    }
  }

  template<typename UniqueIdF>
  void addCandidate(Candidate* cand, const int index, UniqueIdF getUniqueID) {
    const auto it = m_genIdIndices.emplace(getUniqueID(cand), index);
      if (it.second == false) {
        std::cerr << "genCand already in the matching array (by UniqueID)" << std::endl;
    }
  }

  template<typename UniqueIdF>
  int getMatchingGenIndex(Candidate* recoCand, UniqueIdF getUniqueID) const {
    const auto genUID = getUniqueID(recoCand);

    const auto it = m_genIdIndices.find(genUID);
    if (it == m_genIdIndices.cend()) {
      std::cerr << "recoCand does not point to a known generated particle in the matching collection" << std::endl;
      return -1;
    }

    return it->second;
  }

  template<typename UniqueIdF>
  std::vector<std::pair<int, int>> getMatchingIndices(const TObjArray* recoArray,
                                                      UniqueIdF getUniqueID) const {
    std::vector<std::pair<int, int>> matchingIndices;
    for (int i = 0; i < recoArray->GetEntriesFast(); ++i) {
      matchingIndices.emplace_back(getMatchingGenIndex(static_cast<Candidate*>(recoArray->At(i)),
                                                       getUniqueID), i);
    }

    return matchingIndices;
  }

private:
  // the indices in the genParticle Array and the corresponding UniqueID
  std::map<UInt_t, int> m_genIdIndices; // use unordered_map?
};


#endif
