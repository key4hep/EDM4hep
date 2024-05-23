#ifndef EDM4HEP_GENTOOLINFO_H
#define EDM4HEP_GENTOOLINFO_H

#include <string>
#include <vector>
#include "podio/Frame.h"
#include "Constants.h"


namespace edm4hep {

struct GenToolInfo {
  std::string name;
  std::string version;
  std::string description;
};

const std::vector<GenToolInfo> getGenToolInfos(podio::Frame& frame) {
  auto toolInfos = std::vector<GenToolInfo>();
  auto names = frame.getParameter<std::string>("name");
  auto versions = frame.getParameter<std::string>("version");
  auto descriptions = frame.getParameter<std::string>("description");
  for (unsigned int i = 0; i < names.size(); ++i){
    auto toolInfo = GenToolInfo();
    toolInfo.name = names[i];
    toolInfo.version = versions[i];
    toolInfo.description = descriptions[i];
  }
  return toolInfos;
};

void putGenToolInfos(podio::Frame& frame, std::vector<GenToolInfo>& toolInfos) {
  auto names = std::vector<std::string>();
  auto versions = std::vector<std::string>();
  auto descriptions = std::vector<std::string>();
  for (auto& toolInfo : toolInfos){
    names.push_back(toolInfo.name);
    versions.push_back(toolInfo.version);
    descriptions.push_back(toolInfo.description);
  }
  frame.putParameter("name", names);
  frame.putParameter("version", versions);
  frame.putParameter("description", descriptions);
};

} // namespace edm4hep

#endif // EDM4HEP_GENTOOLINFO_H