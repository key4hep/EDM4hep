#include "DelphesEDM4HepConverter.h"

#include <string_view>
#include <algorithm>
#include <unordered_map>
#include <iostream>

template<size_t N>
void sortBranchesProcessingOrder(std::vector<BranchSettings>& branches,
                                 std::array<const char*, N> const& processingOrder);

DelphesEDM4HepConverter::DelphesEDM4HepConverter(std::string const& outputFile, ExRootConfParam /*const*/& branches) :
  m_writer(outputFile, &m_store)
{
  for (int b = 0; b < branches.GetSize(); b += 3) {
    m_branches.push_back({branches[b].GetString(),
                          branches[b + 1].GetString(),
                          branches[b + 2].GetString()});
  }

  sortBranchesProcessingOrder(m_branches, PROCESSING_ORDER);




}


void DelphesEDM4HepConverter::process(Delphes *modularDelphes)
{
   
  writeEvent();
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





template<size_t N>
void sortBranchesProcessingOrder(std::vector<BranchSettings>& branches,
                                 std::array<const char*, N> const& processingOrder)
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
