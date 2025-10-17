#ifndef EDM4HEP_UTILS_PARTICLEIDUTILS_H
#define EDM4HEP_UTILS_PARTICLEIDUTILS_H

#include <edm4hep/ParticleIDCollection.h>
#include <edm4hep/ReconstructedParticle.h>

#include <podio/Frame.h>

#include <map>
#include <optional>
#include <string>
#include <vector>

namespace edm4hep::utils {

/// A simple struct bundling relevant metadata for a ParticleID collection
///
/// This is the recommended class to use for storing ParticleID related
/// metadata. Most importantly it contains the name of an algorithm as well as
/// the names of the parameters (if any). Additionally, it can also
/// automatically determine an algorithm type (to be used in
/// edm4hep::PartilceID::setAlgorithmType) given a name. This is a 32 bit hash
/// of the algorithm name.
///
/// @note Given that this structure and the actual contents of the corresponding
/// ParticleIDCollection are disjoint it is your responsibility to set the
/// information accordingly. We offer some utility functionality to ensure
/// consistent setting, but the tools can in principle also be used without
/// that.
struct ParticleIDMeta {
  /// Constructor for I/O purposes or with a pre-defined algorithm type
  ///
  /// @note We generally recommend not using pre-defined algorithm types, but
  /// rather to use the automatically determined ones from the
  ///
  /// @param algName  The name of the PID algorithm
  /// @param algType  The (encoded) algorithm type (stored in the ParticleID
  ///                 algorithmType) field
  /// @param parNames The (optional) parameter names for this PID algorithms.
  ///                 There should be a parameter name for each parameter that
  ///                 is stored in the ParticleID parameters field
  ParticleIDMeta(const std::string& algName, int32_t algType, const std::vector<std::string>& parNames = {});

  /// Main constructor for creating a ParticleIDMeta object without parameters
  ///
  /// @param algName  The name of the PID algorithm
  ParticleIDMeta(const std::string& algName);

  /// Main constructor for creating a ParticleIDMeta object with parameters
  ///
  /// @param algName  The name of the PID algorithm
  /// @param parNames The (optional) parameter names for this PID algorithms.
  ///                 There should be a parameter name for each parameter that
  ///                 is stored in the ParticleID parameters field
  ParticleIDMeta(const std::string& algName, const std::vector<std::string>& parNames);

  ~ParticleIDMeta() = default;
  /// Default constructor
  ///
  /// @note It is not possible to change the algorithm type when this
  /// constructor is used
  ParticleIDMeta() = default;
  ParticleIDMeta(const ParticleIDMeta&) = default;
  ParticleIDMeta& operator=(const ParticleIDMeta&) = default;
  ParticleIDMeta(ParticleIDMeta&&) = default;
  ParticleIDMeta& operator=(ParticleIDMeta&&) = default;

  std::string algoName{};                ///< The name of the algorithm
  std::vector<std::string> paramNames{}; ///< The names of the parameters

  /// Get the encoded algorithm type for the PID algorithm that is described by
  /// this meta object
  ///
  /// @returns the algorithm type (usually a 32 bit hash of the algorithm name)
  int32_t algoType() const { return m_algoType; }

private:
  int32_t m_algoType{0}; ///< The (potentially user defined) algorithm type
};

/// Get the index of a parameter for a given PID algorithm
///
/// @param pidMetaInfo A metadata object describig a ParticleID algorithm
/// @param param       The name of the parameter
///
/// @returns The index of the parameter which can be used to index into the
///          ParticleID::getParameters() values or an empty optional in case the
///          parameter name could not be found in the metadata that were passed.
std::optional<int> getParamIndex(const ParticleIDMeta& pidMetaInfo, const std::string& param);

/// Utility class to invert the ParticleID to ReconstructedParticle relation
///
/// This is the main utility class to use when trying to obtain ParticleID
/// objects related to ReconstructedParticles. Basic functionality (i.e.
/// retrieving the related ParticleIDs) can be done without any additional
/// metadata. The full functionality is enabled by also passing in metadata (in
/// the form of ParticleIDMeta) or alternatively adding it after the initial
/// construction.
///
/// This uses an internal map that is updated whenever a new
/// ParticleIDCollection is added. Hence, lookup of related ParticleID objects
/// for a given ReconstructedParticle avoids looping over all collections.
///
/// @note there are no checks in place that ensure that the metadata that has
/// been added to a given PIDHandler actually correspond to any of the stored
/// ParticleIDs. It is in fact even possible to use the PIDHandler without any
/// PartileID objects to simply query meta information about ParticleID
/// algorithms.
///
/// See [this page](@ref md_doc_2_p_i_d_handler) for example usage and more information.
class PIDHandler {

  using RecoPidMapT = std::multimap<edm4hep::ReconstructedParticle, edm4hep::ParticleID>;

  RecoPidMapT m_recoPidMap{}; ///< The internal map from recos to pids

  std::map<std::string, int> m_algoTypes{}; ///< Maps algo names to algo types

  /// Maps algo types to the full particle id meta information
  std::map<int, edm4hep::utils::ParticleIDMeta> m_algoPidMeta{};

public:
  PIDHandler() = default;
  ~PIDHandler() = default;
  // Copies are not allowed to avoid copying the internal maps
  PIDHandler(const PIDHandler&) = delete;
  PIDHandler& operator=(const PIDHandler&) = delete;
  PIDHandler(PIDHandler&&) = default;
  PIDHandler& operator=(PIDHandler&&) = default;

  /// Construct a PIDHandler from an arbitrary number of ParticleIDCollections
  ///
  /// This constructor does not retrieve any metadata and on its own will only
  /// enable the basic functionality. For the full functionality add the
  /// necessary metadata in a follow up call to @ref addMetaInfos or @ref
  /// addMetaInfo
  ///
  /// @tparam PIDColls A variadic template that we use to enable passing
  ///                  arbitrary numbers of collections
  ///
  /// @param coll A ParticleIDCollection for constructing a PIDHandler
  /// @param pidColls An arbitrary number of additional ParticleIDCollections
  ///                 that will also be added to the internal map
  ///
  /// @returns A PIDHandler that is able to handle relations between
  ///          ReconstructedParticles and all ParticleID objects that are part
  ///          of the passed collection
  template <typename... PIDColls>
  static PIDHandler from(const ParticleIDCollection& coll, const PIDColls&... pidColls) {
    static_assert((std::is_same_v<PIDColls, edm4hep::ParticleIDCollection> && ...),
                  "PIDHandler can only be constructed from ParticleIDCollections");
    PIDHandler handler{};
    handler.addColl(coll);
    (handler.addColl(pidColls), ...);
    return handler;
  }

  /// Create a PIDHandler from a Frame potentially also populating some metadata information.
  ///
  /// Create a PIDHandler using all ParticleIDCollections that can be found. If
  /// metadata is passed automatically also load all related metadata (if
  /// available).
  ///
  /// @note This constructor does not guarantee that **all**
  /// ParticleIDCollections will have valid metadata loaded. Only if such
  /// metadata is actually present and found will it be ingested, otherwise only
  /// the ParticleID objects will be added to the internal map.
  ///
  /// @param event    The event Frame from which all ParticleIDCollections will be
  ///                 retrieved
  /// @param metadata An (optional) metadata Frame from which ParticleIDMeta will
  ///                 be automatically retrieved for all ParticleIDCollections
  ///                 for which it is actually available.
  ///
  /// @returns A PIDHandler that is able to handle relations between
  ///          ReconstructedParticles and all ParticleID objects that were found
  ///          in the event Frame. If metadata is found on top of that, also
  ///          that has been ingested and can now be queried through this handler.
  static PIDHandler from(const podio::Frame& event, const podio::Frame& metadata = {});

  /// Add the information from one ParticleIDCollection to the handler
  ///
  /// @note This will only add the collection to enable basic functionality. In
  /// order to enable the full functionality it is also necessary to add the
  /// necessary ParticleIDMeta information that corresponds to the added
  /// collection.
  ///
  /// This is the main function for adding new collections. All other methods
  /// that add ParticleID collections will eventually call this in one way or
  /// another.
  ///
  /// @param coll A ParticleIDCollection that should be added to the internal map
  void addColl(const edm4hep::ParticleIDCollection& coll);

  /// Add the information from one ParticleIDCollection to the handler together
  /// with its meta data
  ///
  /// @note This method does no check in any form whether the passed collection
  /// and metadata are actually related in any form.
  ///
  /// @param coll    A ParticleIDCollection that should be added to the internal
  ///                map
  /// @param pidInfo The metadata describing the algorithm that has been used to
  ///                determine the ParticleIDs of the coll
  void addColl(const edm4hep::ParticleIDCollection& coll, const edm4hep::utils::ParticleIDMeta& pidInfo);

  /// Add (arbitrary) metadata describing a PID algorithm.
  ///
  /// This is the main function for ingesting metadata. All other methods that
  /// add ParticleID algorithm metadata will eventually call this in one way or
  /// another.
  ///
  /// @param pidInfo The metadata describing an algorithm that has been used to
  ///                determine ParticleID objects
  void addMetaInfo(const edm4hep::utils::ParticleIDMeta& pidInfo);

  /// Add several (arbitrary) meta informations simultaneously
  ///
  /// @tparam PIDMetas A variadic template that we use to enable calling this
  ///                  with an arbitrary number of ParticleIDMeta objects
  ///
  /// @param pidMetas An arbitrary number of metadata objects describing
  ///                 algorithms that have been used to determine ParticleID
  ///                 objects
  template <typename... PIDMetas>
  void addMetaInfos(const PIDMetas&... pidMetas) {
    static_assert((std::is_same_v<PIDMetas, edm4hep::utils::ParticleIDMeta> && ...),
                  "Only ParticleIDMeta can be used to add metadata to a PIDHandler");
    (addMetaInfo(pidMetas), ...);
  }

  /// Retrieve all ParticleIDs that are related to the passed
  /// ReconstructedParticle
  ///
  /// @param reco The ReconstructedParticle for which ParticleIDs should be
  ///             looked up
  ///
  /// @returns All ParticleID objects (that this PIDHandler knows about) that
  ///          point to the passed ReconstructedParticle
  std::vector<edm4hep::ParticleID> getPIDs(const edm4hep::ReconstructedParticle& reco) const;

  /// Retrieve the ParticleID for a given PID algorithm
  ///
  /// @param reco The ReconstructedParticle for which a ParticleID should be
  ///                 looked up
  /// @param algoType The (encoded) algorithm type that corresponds to the
  ///                 desired algorithm. See also @ref getAlgoType
  ///
  /// @returns The ParticleID object for this PID algorithm that points to the
  ///          passed ReconstrucedParticle if found or an empty optional
  ///          otherwise
  std::optional<edm4hep::ParticleID> getPID(const edm4hep::ReconstructedParticle& reco, int algoType) const;

  /// Retrieve the index in the parameters for a given parameter name and
  /// algoType
  ///
  /// @param algoType  The (encoded) algorithm type that corresponds to the
  ///                  desired algorithm. See also @ref getAlgoType
  /// @param paramName The name of the parameter
  ///
  /// @returns The index of the parameter which can be used to index into the
  ///          ParticleID::getParameters() values or an empty optional in case the
  ///          parameter name could not be found for the passed algorithm type
  ///
  /// See also @ref edm4hep::utils::getParamIndex
  std::optional<int> getParamIndex(int32_t algoType, const std::string& paramName) const;

  /// Retrieve the algoType for a given algorithm name
  ///
  /// @param algoName
  ///
  /// @returns The (encoded) algorithm type for the desired algorithm if known
  ///          to the PIDHandler otherwise an empty optional.
  std::optional<int32_t> getAlgoType(const std::string& algoName) const;

  /// Set the metadata information for the passed collection in the metadata Frame.
  ///
  /// This also sets the algorithmType of all elements in the collection to the
  /// one that is found in the meta information.
  ///
  /// @param metadata The metadata Frame into which the information should be
  ///                 stored
  /// @param pidcoll  A ParticleIDCollection for which the corresponding
  ///                 algorithm type will be set (consistent with what is found
  ///                 in the @p pidMetaInfo)
  /// @param pidMetaInfo The metadata object that corresponds to the ParticleID
  ///                    algorithm that has been used for creating the @p
  ///                    pidcoll
  static void setAlgoInfo(podio::Frame& metadata, edm4hep::ParticleIDCollection& pidcoll, const std::string& collname,
                          const edm4hep::utils::ParticleIDMeta& pidMetaInfo);

  /// Set the metadata information for a given collection name in the metadata Frame.
  ///
  /// @note It is user responsibility to ensure that the meta information that
  /// is passed here and the one that is present in the collection with the
  /// given name is consistent
  ///
  /// @param metadata The metadata Frame into which the information should be
  ///                 stored
  /// @param collname The name of the (ParticleID) collection for which this
  ///                 meta information should be stored
  /// @param pidMetaInfo The metadata object that corresponds to the ParticleID
  ///                    algorithm that has been used for creating the
  ///                    ParticleID objects stored in the collection with the @p
  ///                    collname
  static void setAlgoInfo(podio::Frame& metadata, const std::string& collname,
                          const edm4hep::utils::ParticleIDMeta& pidMetaInfo);

  /// Get the ParticleID meta information for a given collection name from the metadata Frame.
  ///
  /// @param metadata The metadata frame in which to search for ParticleID
  ///                 related metadata
  /// @param collName The (ParticleID) collection name for which to obtain metadata
  ///
  /// @returns The metadata related to the PID algorithm that has been used to
  ///          create the @p collName collection if available, otherwise an
  ///          empty optional.
  static std::optional<edm4hep::utils::ParticleIDMeta> getAlgoInfo(const podio::Frame& metadata,
                                                                   const std::string& collName);
};
} // namespace edm4hep::utils

#endif // EDM4HEP_UTILS_PARTICLEIDUTILS_H
