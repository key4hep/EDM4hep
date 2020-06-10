#ifndef DELPHESEDM4HEP_DELPHESHELPERS
#define DELPHESEDM4HEP_DELPHESHELPERS

#include "classes/DelphesClasses.h"

#include "TObjArray.h"

#include <vector>
#include <iostream>
#include <utility>
#include <map>
#include <functional>

UInt_t uniqueID(Candidate* genCand) {
  return genCand->GetUniqueID();
}

UInt_t trivialGenID(Candidate* cand) {
  return static_cast<Candidate*>(cand->GetCandidates()->At(0))->GetUniqueID();
}

std::vector<UInt_t> getTrivialGenID(Candidate* cand) {
  return { trivialGenID(cand) };
}

// Following what is done in TreeWriter::FillParticles
std::vector<UInt_t> getAllParticleIDs(Candidate* candidate) {
  std::vector<UInt_t> relatedParticles;
  TIter it1(candidate->GetCandidates());
  it1.Reset();

  while((candidate = static_cast<Candidate*>(it1.Next()))) {
    TIter it2(candidate->GetCandidates());

    // particle
    if (candidate->GetCandidates()->GetEntriesFast() == 0) {
      relatedParticles.push_back(candidate->GetUniqueID());
      continue;
    }

    // track
    candidate = static_cast<Candidate*>(candidate->GetCandidates()->At(0));
    if (candidate->GetCandidates()->GetEntriesFast() == 0) {
      relatedParticles.push_back(candidate->GetUniqueID());
      continue;
    }

    // tower
    it2.Reset();
    while((candidate = static_cast<Candidate*>(it2.Next()))) {
      relatedParticles.push_back(candidate->GetCandidates()->At(0)->GetUniqueID());
    }
  }

  return relatedParticles;
}

using MatchingIndices = std::vector<std::pair<int, int>>;
using UniqueIdF = std::function<UInt_t(Candidate*)>;
using AllUniqueIdsF = std::function<std::vector<UInt_t>(Candidate*)>;

class DelphesUniqueIDGenMatcher {
public:
  DelphesUniqueIDGenMatcher() = default;

  DelphesUniqueIDGenMatcher(const TObjArray* matchToArray, UniqueIdF getUniqueID=uniqueID) {
    const auto genSize = matchToArray->GetEntriesFast();
    for (int i = 0; i < genSize; ++i) {
      auto* genCand = static_cast<Candidate*>(matchToArray->At(i));
      addCandidate(genCand, i, getUniqueID);
    }
  }

  void addCandidate(Candidate* cand, const int index, UniqueIdF getUniqueID=uniqueID) {
    const auto it = m_genIdIndices.emplace(getUniqueID(cand), index);
      if (it.second == false) {
        std::cerr << "genCand already in the matching array (by UniqueID)" << std::endl;
    }
  }

  MatchingIndices getMatchingIndices(const TObjArray* toArray, AllUniqueIdsF getUniqueIds=getTrivialGenID) const {
    MatchingIndices matchingIndices;

    for (int i = 0; i < toArray->GetEntriesFast(); ++i) {
      auto* cand = static_cast<Candidate*>(toArray->At(i));
      for (const auto index : getMatchingIndices(cand, getUniqueIds)) {
        matchingIndices.emplace_back(index, i);
      }
    }

    return matchingIndices;
  }

  std::vector<int> getMatchingIndices(Candidate* cand, AllUniqueIdsF getUniqueIds=getTrivialGenID) const {
    std::vector<int> matchingIndices;

    for (const auto uid : getUniqueIds(cand)) {
      auto matchIdx = getMatchingGenIndex(uid);
        if (matchIdx > -1) {
          matchingIndices.push_back(matchIdx);
        }
      }

    return matchingIndices;
  }
 

private:
  int getMatchingGenIndex(UInt_t uid) const {
    const auto it = m_genIdIndices.find(uid);
    if (it == m_genIdIndices.cend()) {
      // std::cerr << "Passed UniqueID does not point to a known generated particle in the matching  collection" << std::endl;
      return -1;
    }

    return it->second;
  }
  
  // the indices in the genParticle Array and the corresponding UniqueID
  std::map<UInt_t, int> m_genIdIndices; // use unordered_map?
};


#endif
