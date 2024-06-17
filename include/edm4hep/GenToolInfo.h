#ifndef EDM4HEP_GENTOOLINFO_H
#define EDM4HEP_GENTOOLINFO_H

#include "edm4hep/Constants.h"
#include "podio/Frame.h"
#include <string>
#include <vector>

namespace edm4hep {

struct GeneratorToolInfo {
  std::string name;
  std::string version;
  std::string description;

  GeneratorToolInfo(){};
  GeneratorToolInfo(const std::string& name, const std::string& version, const std::string& description) :
      name(name), version(version), description(description){};
};

namespace utils {

  const inline std::vector<GeneratorToolInfo> getGenToolInfos(const podio::Frame& frame) {
    using namespace edm4hep::labels;
    auto toolInfos = std::vector<GeneratorToolInfo>();
    const auto names =
        frame.getParameter<std::vector<std::string>>(GeneratorToolName).value_or(std::vector<std::string>{});
    const auto versions =
        frame.getParameter<std::vector<std::string>>(GeneratorToolVersion).value_or(std::vector<std::string>{});
    const auto descriptions =
        frame.getParameter<std::vector<std::string>>(GeneratorToolDescription).value_or(std::vector<std::string>{});
    for (unsigned int i = 0; i < names.size(); ++i) {
      toolInfos.emplace_back(names[i], versions[i], descriptions[i]);
    }
    return toolInfos;
  };

  void inline putGenToolInfos(podio::Frame& frame, std::vector<GeneratorToolInfo>& toolInfos) {
    auto names = std::vector<std::string>();
    auto versions = std::vector<std::string>();
    auto descriptions = std::vector<std::string>();
    for (auto& toolInfo : toolInfos) {
      names.push_back(toolInfo.name);
      versions.push_back(toolInfo.version);
      descriptions.push_back(toolInfo.description);
    }

    using namespace edm4hep::labels;
    frame.putParameter(GeneratorToolName, std::move(names));
    frame.putParameter(GeneratorToolVersion, std::move(versions));
    frame.putParameter(GeneratorToolDescription, std::move(descriptions));
  };
} // namespace utils
} // namespace edm4hep

#endif // EDM4HEP_GENTOOLINFO_H
