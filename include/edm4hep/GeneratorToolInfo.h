#ifndef EDM4HEP_GENERATORTOOLINFO_H
#define EDM4HEP_GENERATORTOOLINFO_H

#include "edm4hep/Constants.h"
#include "podio/Frame.h"
#include <string>
#include <vector>

namespace edm4hep {

/// Meta information class to group information about the used generator (tools)
/// to create a file
///
/// @note Since this is all rather loosely coupled and stored via podio Frame
/// parameters, use of the @ref getGenToolInfos and @ref putGenToolInfos utility
/// functions for retrieval, resp. storage of this information is crucial to
/// ensure consistent information.
struct GeneratorToolInfo {
  std::string name{};        ///< The name of the tool
  std::string version{};     ///< The version of the tool
  std::string description{}; ///< A brief description of the tool

  /// Construct a generator tool info object with all empty fields
  GeneratorToolInfo() = default;

  /// Construct a complete tool info object from all ingredients
  ///
  /// @param nameTool        The name of the tool
  /// @param versionTool     The version of the tool
  /// @param descrTool       The brief description of the tool
  GeneratorToolInfo(const std::string& nameTool, const std::string& versionTool, const std::string& descrTool)
      : name(nameTool), version(versionTool), description(descrTool) {}
};

namespace utils {

  /// Get all the generator tool infos that are available from the passed
  /// (metadata) frame.
  ///
  /// Tries to retrieve all meta information that are available and that have
  /// been stored via the @ref putGenToolInfos function.
  ///
  /// @param frame The (metadata) frame that should be queried for the
  ///              information
  ///
  /// @returns The GeneratorToolInfo that were found in the Frame. If none ar
  ///          found an empty vector will be returned.
  const inline std::vector<GeneratorToolInfo> getGenToolInfos(const podio::Frame& frame) {
    using namespace edm4hep::labels;
    auto toolInfos = std::vector<GeneratorToolInfo>();
    const auto names =
        frame.getParameter<std::vector<std::string>>(GeneratorToolNames).value_or(std::vector<std::string>{});
    const auto versions =
        frame.getParameter<std::vector<std::string>>(GeneratorToolVersions).value_or(std::vector<std::string>{});
    const auto descriptions =
        frame.getParameter<std::vector<std::string>>(GeneratorToolDescriptions).value_or(std::vector<std::string>{});
    for (unsigned int i = 0; i < names.size(); ++i) {
      toolInfos.emplace_back(names[i], versions[i], descriptions[i]);
    }
    return toolInfos;
  };

  /// Put the generator tool meta information into a (metadata) frame
  ///
  /// In order to guarantee consistent storage and retrieval of these metadata
  /// it is necessary to use this utility function.
  ///
  /// @param frame     The (metadata) Frame into which the generator tool info should go
  /// @param toolInfos The generator tool infos that should be stored
  void inline putGenToolInfos(podio::Frame& frame, const std::vector<GeneratorToolInfo>& toolInfos) {
    auto names = std::vector<std::string>();
    auto versions = std::vector<std::string>();
    auto descriptions = std::vector<std::string>();
    for (auto& toolInfo : toolInfos) {
      names.push_back(toolInfo.name);
      versions.push_back(toolInfo.version);
      descriptions.push_back(toolInfo.description);
    }

    using namespace edm4hep::labels;
    frame.putParameter(GeneratorToolNames, std::move(names));
    frame.putParameter(GeneratorToolVersions, std::move(versions));
    frame.putParameter(GeneratorToolDescriptions, std::move(descriptions));
  };
} // namespace utils
} // namespace edm4hep

#endif // EDM4HEP_GENERATORTOOLINFO_H
