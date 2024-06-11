#ifndef EDM4HEP_GENTOOLINFO_H
#define EDM4HEP_GENTOOLINFO_H

#include "edm4hep/Constants.h"
#include "podio/Frame.h"
#include <string>
#include <vector>

namespace edm4hep {

struct GenToolInfo {
  std::string name;
  std::string version;
  std::string description;

  GenToolInfo(){};
  GenToolInfo(const std::string& name, const std::string& version, const std::string& description) :
      name(name), version(version), description(description){};
};

namespace utils {

  const std::vector<GenToolInfo> getGenToolInfos(const podio::Frame& frame) {
    auto toolInfos = std::vector<GenToolInfo>();
    const auto names = frame.getParameter<std::vector<std::string>>(generatorToolName).value();
    const auto versions = frame.getParameter<std::vector<std::string>>(generatorToolVersion).value();
    const auto descriptions = frame.getParameter<std::vector<std::string>>(generatorToolDescription).value();
    for (unsigned int i = 0; i < names.size(); ++i) {
      toolInfos.emplace_back(names[i], versions[i], descriptions[i]);
    }
    return toolInfos;
  };

  void putGenToolInfos(podio::Frame& frame, std::vector<GenToolInfo>& toolInfos) {
    auto names = std::vector<std::string>();
    auto versions = std::vector<std::string>();
    auto descriptions = std::vector<std::string>();
    for (auto& toolInfo : toolInfos) {
      names.push_back(toolInfo.name);
      versions.push_back(toolInfo.version);
      descriptions.push_back(toolInfo.description);
    }
    frame.putParameter(generatorToolName, std::move(names));
    frame.putParameter(generatorToolVersion, std::move(versions));
    frame.putParameter(generatorToolDescription, std::move(descriptions));
  };
} // namespace utils
} // namespace edm4hep

#endif // EDM4HEP_GENTOOLINFO_H