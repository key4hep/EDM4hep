# v00-99-04

* 2025-11-03 BrieucF ([PR#455](https://github.com/key4hep/EDM4hep/pull/455))
  - Explicitly mention units and intended usage of TrackerHitPlane u, v, du and dv

* 2025-10-30 BrieucF ([PR#385](https://github.com/key4hep/EDM4hep/pull/385))
  - Add a data type for sense wire detectors digitized hits (only the distance to the wire is known)

* 2025-10-27 Mateusz Jakub Fila ([PR#458](https://github.com/key4hep/EDM4hep/pull/458))
  - Fix missing `std::` for math functions, replace more `sqrt` with `hypot`

# v00-99-03

* 2025-10-24 Thomas Madlener ([PR#456](https://github.com/key4hep/EDM4hep/pull/456))
  - Bump the minimum version of podio to v01-06

* 2025-10-24 Juan Miguel Carceller ([PR#453](https://github.com/key4hep/EDM4hep/pull/453))
  - Fix the warnings related to `isValid` being deprecated (https://github.com/AIDASoft/podio/pull/860) after the version of podio is bumped

* 2025-10-23 Juan Miguel Carceller ([PR#457](https://github.com/key4hep/EDM4hep/pull/457))
  - Remove unused variable in tools/CMakeLists.txt

* 2025-10-23 Juan Miguel Carceller ([PR#454](https://github.com/key4hep/EDM4hep/pull/454))
  - Use `std::hypot` when possible, prefer `std::sqrt` or `std::hypot` to sqrt

* 2025-10-21 Thomas Madlener ([PR#446](https://github.com/key4hep/EDM4hep/pull/446))
  - Remove the `GeneratorPdfInfo` datatype again, as the information it contains is only relevant for LO calculations and modern generators already take care of all of this using event weighting.
  - Rework the `GeneratorEventParameters` slightly to contain relevant information only. Information that is not applicable beyond LO is removed as again this is handled by the generator via event weighting.
  - **These are breaking changes!** Currently even files that have been written with an old version of the `GeneratorEventParameters` datatype will not be readable.

* 2025-10-14 Thomas Madlener ([PR#452](https://github.com/key4hep/EDM4hep/pull/452))
  - Do not build a separate `edm4hepOldSchemas` library (and dictionaries), but rather merge everything into the core EDM4hep library (and dictionaries).

* 2025-10-13 Juan Miguel Carceller ([PR#443](https://github.com/key4hep/EDM4hep/pull/443))
  - Use edm4hep.h in edm4hep2json to avoid hardcoding the types
  - Remove the test `test_all_collections` that reads the .cxx file to check if  all the collections are there (but a comment could trigger a false positive, for example). There is still a test that runs `edm4hep2json`.
  - **Bump the minimum required podio version to v01-05.**

* 2025-09-23 Juan Miguel Carceller ([PR#451](https://github.com/key4hep/EDM4hep/pull/451))
  - Set ROOT_LIBRARY_PATH for tests to prevent it from being picked up from the environment

* 2025-09-23 Mateusz Jakub Fila ([PR#450](https://github.com/key4hep/EDM4hep/pull/450))
  - Add missing article data in the citation file so the BibTeX entry generated from it references the article correctly

* 2025-09-23 Mateusz Jakub Fila ([PR#448](https://github.com/key4hep/EDM4hep/pull/448))
  - Update comments referring to removed un-namespaced labels

* 2025-09-23 Thomas Madlener ([PR#447](https://github.com/key4hep/EDM4hep/pull/447))
  - Add some documentation about the subtle differences between LCIO and EDM4hep that are going to stay

* 2025-09-22 Juan Miguel Carceller ([PR#445](https://github.com/key4hep/EDM4hep/pull/445))
  - Remove support for CPack and the unused `EDM4HEP.pc.in`

* 2025-09-16 Mateusz Jakub Fila ([PR#449](https://github.com/key4hep/EDM4hep/pull/449))
  - Fixed another bunch of minor  typos in docstrings and code comments

* 2025-09-02 Thomas Madlener ([PR#404](https://github.com/key4hep/EDM4hep/pull/404))
  - Replace the `spin` by `helicity` in `MCParticle` as the former is not usable without the full spin-density matrix whereas the latter is always well defined (or unset).
    - Defaults to `9` which is used to signify an unset value. Use `hasHelicity` to check whether the `helicity` is set

* 2025-08-26 Mateusz Jakub Fila ([PR#444](https://github.com/key4hep/EDM4hep/pull/444))
  - Include `podio::UserDataCollection` in file created by `creatEDM4hepFile.py`

* 2025-08-22 Mateusz Jakub Fila ([PR#442](https://github.com/key4hep/EDM4hep/pull/442))
  - Fix `edm4hep2json` not recognizing generator metadata types

* 2025-08-19 Juraj Smiesko ([PR#441](https://github.com/key4hep/EDM4hep/pull/441))
  - edm4hep2json: Adding `isSubsetColl` and `collSchemaVersion` to the JSON

* 2025-08-18 Thomas Madlener ([PR#440](https://github.com/key4hep/EDM4hep/pull/440))
  - Remove the deprecated labels that were originally defined outside the `labels` namespace.

* 2025-08-12 jmcarcell ([PR#439](https://github.com/key4hep/EDM4hep/pull/439))
  - Update the schema version after the changes in 32af1cb8 to avoid a warning every time that a file with CaloHitCollections is read

* 2025-08-12 jmcarcell ([PR#438](https://github.com/key4hep/EDM4hep/pull/438))
  - Add timeouts and fixtures to the tests that create and read files to make jobs that fail due to timeouts fail faster

* 2025-08-12 jmcarcell ([PR#437](https://github.com/key4hep/EDM4hep/pull/437))
  - Do not use the namespace HepMC3, removes a warning in unity builds

* 2025-08-12 jmcarcell ([PR#436](https://github.com/key4hep/EDM4hep/pull/436))
  - Remove the deprecated constructor, getters and setters

* 2025-08-12 jmcarcell ([PR#435](https://github.com/key4hep/EDM4hep/pull/435))
  - Add const to the argument of operator= in covariance matrices

* 2025-07-08 Mateusz Jakub Fila ([PR#434](https://github.com/key4hep/EDM4hep/pull/434))
  - Fixed blank documentation page with new doxygen version

* 2025-06-24 Thomas Madlener ([PR#433](https://github.com/key4hep/EDM4hep/pull/433))
  - Add a `stepLength` member to the `CaloHitContribution`
  - Clarify the docstring for the `particle` relation of the `CaloHitContribution`

# v00-99-02

* 2025-05-16 jmcarcell ([PR#430](https://github.com/key4hep/EDM4hep/pull/430))
  - Bump the required version of podio to 1.3. This is needed since ::typeName in link collections are only present in 1.3 and onwards. See https://github.com/key4hep/EDM4hep/issues/415

* 2025-05-14 Mateusz Jakub Fila ([PR#429](https://github.com/key4hep/EDM4hep/pull/429))
  - Update CI build badge in readme

* 2025-05-07 Thomas Madlener ([PR#428](https://github.com/key4hep/EDM4hep/pull/428))
  - Switch to latest actions for CI and enable caching of build artifacts for speeding up workflows

* 2025-05-07 Thomas Madlener ([PR#413](https://github.com/key4hep/EDM4hep/pull/413))
  - Limit the RNTuple backwards compatibility checks to only run with ROOT 6.34.04 and podio 1.2

* 2025-04-28 jmcarcell ([PR#426](https://github.com/key4hep/EDM4hep/pull/426))
  - Remove intermediate variables and make loading libraries and files consistent in `__init__.py`

* 2025-04-24 jmcarcell ([PR#427](https://github.com/key4hep/EDM4hep/pull/427))
  - Fix building with C++23 by not aliasing std::to_underlying to a type.

* 2025-04-01 Thomas Madlener ([PR#425](https://github.com/key4hep/EDM4hep/pull/425))
  - Switch pre-commit workflow to run on Key4hep nightlies to have a recent enough version of `clang-format`.

* 2025-04-01 Mateusz Jakub Fila ([PR#422](https://github.com/key4hep/EDM4hep/pull/422))
  - Add missing units for covariances, particle charge and other members

* 2025-04-01 Mateusz Jakub Fila ([PR#414](https://github.com/key4hep/EDM4hep/pull/414))
  - Replace `podio::detail::linkCollTypeName` with  link collection `typeName` introduced in AIDASoft/podio#748

* 2025-03-21 BrieucF ([PR#420](https://github.com/key4hep/EDM4hep/pull/420))
  - Clarify the `phi` member of the `TrackState`
  - Specify units (radians) for members describing angles

* 2025-03-12 Mateusz Jakub Fila ([PR#417](https://github.com/key4hep/EDM4hep/pull/417))
  - Fix capitalization of `eDep`in yaml comments

* 2025-03-12 Graeme A Stewart ([PR#408](https://github.com/key4hep/EDM4hep/pull/408))
  - Add citation information to README.md and corresponding CITATION.cff file

* 2025-02-20 BrieucF ([PR#406](https://github.com/key4hep/EDM4hep/pull/406))
  - Clarify the comment on the Cluster direction members

* 2025-02-20 Mateusz Jakub Fila ([PR#403](https://github.com/key4hep/EDM4hep/pull/403))
  - Added a tab with standalone documentation pages in doxygen docs

* 2025-02-14 jmcarcell ([PR#405](https://github.com/key4hep/EDM4hep/pull/405))
  - Use `std::swap` (available now that we compile with C++20) and add a default
    case for the switch statement in the covariance matrix utils

* 2025-01-31 Paul Gessinger ([PR#401](https://github.com/key4hep/EDM4hep/pull/401))
  - Refactored YAML to use multiline strings

* 2025-01-31 Thomas Madlener ([PR#398](https://github.com/key4hep/EDM4hep/pull/398))
  - Make the `EventHeader::eventNumber` a 64 bit unsigned integer to avoid issues with the range of 32 bit integers.

* 2025-01-20 Mateusz Jakub Fila ([PR#400](https://github.com/key4hep/EDM4hep/pull/400))
  - Update C++ standard in clang_format to C++20

* 2025-01-16 Thomas Madlener ([PR#395](https://github.com/key4hep/EDM4hep/pull/395))
  - Add proper docstrings to all ParticleID related utilities (`edm4hep::utils::ParticleIDMeta` and `edm4hep::utils::PIDHandler`).
  - Add a `addMetaInfos` convenience overload to the `PIDHandler` to allow passing in several meta objects at once.

* 2025-01-14 Mateusz Jakub Fila ([PR#399](https://github.com/key4hep/EDM4hep/pull/399))
  - Fix component table in readme so it renders correctly

* 2025-01-10 Thomas Madlener ([PR#389](https://github.com/key4hep/EDM4hep/pull/389))
  - Remove the `colorFlow` member variable from the `MCParticle` since it is effectively unused (and should remain that way as there is no really correct way to use it without baking in generator specific assumptions).
    - This is a backwards compatible change w.r.t. readability of existing files (ROOT schema evolution handles this)
    - **This is a breaking change for the API** as `{get,set}ColorFlow` are removed from the `MCParticle`.
  - Remove the now unused `edm4hep::Vector2i` from the components

* 2024-12-20 Thomas Madlener ([PR#394](https://github.com/key4hep/EDM4hep/pull/394))
  - Make sure to also install the `rootmap` and `rdict` files for the old schemas library

* 2024-12-19 Mateusz Jakub Fila ([PR#393](https://github.com/key4hep/EDM4hep/pull/393))
  - Updated doxygen files to the version 1.11 so a dark mode renders properly

* 2024-12-19 Thomas Madlener ([PR#392](https://github.com/key4hep/EDM4hep/pull/392))
  - Bump the minimal podio version to 1.2, since the `links` category in the YAML file was only introduced there.

* 2024-12-19 Mateusz Jakub Fila ([PR#391](https://github.com/key4hep/EDM4hep/pull/391))
  - Install oldSchemas library and dictionary

* 2024-12-19 jmcarcell ([PR#390](https://github.com/key4hep/EDM4hep/pull/390))
  - Make the PIDHandler class non-copyable

* 2024-12-18 tmadlener ([PR#373](https://github.com/key4hep/EDM4hep/pull/373))
  - Switch to templated `podio::Link`s for the `Link` datatypes
  - Bump the schema version to 3
  - **Remove the legacy `Association` headers that were put in place for a smooth transition to `Link`s**

* 2024-11-08 Thomas Madlener ([PR#381](https://github.com/key4hep/EDM4hep/pull/381))
  - Make sure to only run backwards compatibility tests if we have a suitable version of ROOT to still read the RC2 RNTuples stored in the files.

* 2024-10-23 Thomas Madlener ([PR#379](https://github.com/key4hep/EDM4hep/pull/379))
  - Add an `EDM4HEP_WITH_JSON` cmake option (default `ON`) to be explicit about whether we want to build EDM4hep with JSON support or not.
    - Properly propagate this to downstream packages.
    - If set to `OFF` the `edm4hep2json` executable will not be built.

* 2024-10-23 Thomas Madlener ([PR#378](https://github.com/key4hep/EDM4hep/pull/378))
  - Make sure that the bits used for secondary and tertiary vertex bits are actually different

* 2024-10-16 jmcarcell ([PR#377](https://github.com/key4hep/EDM4hep/pull/377))
  - Remove redundant `inline` from templated utility functions.

* 2024-09-30 tmadlener ([PR#376](https://github.com/key4hep/EDM4hep/pull/376))
  - Use `re.search` instead of `re.match` in the version extraction of the backwards compatibility tests to make sure that versions can be extracted even if the file name doesn't start with the expected regex.

* 2024-09-30 Mateusz Jakub Fila ([PR#370](https://github.com/key4hep/EDM4hep/pull/370))
  - Added ruff pre-commit hook for formatting and linting python files
  - Reformatted and fixed warnings in python files
  - Added cmake targets `ruff` and `ruff-format` to lint and format python code

* 2024-09-26 tmadlener ([PR#358](https://github.com/key4hep/EDM4hep/pull/358))
  - Download files written with EDM4hep versions for which we promise backwards compatibility and check whether we can still read them correctly.

# v00-99-01

* 2024-09-20 tmadlener ([PR#374](https://github.com/key4hep/EDM4hep/pull/374))
  - Bump the minimal podio version to v01-01 because we need [AIDASoft/podio#651](https://github.com/AIDASoft/podio/pull/651) and [AIDASoft/podio#678](https://github.com/AIDASoft/podio/pull/678)

* 2024-09-18 jmcarcell ([PR#372](https://github.com/key4hep/EDM4hep/pull/372))
  - Remove unused variable in edm4hep_testhepmc.cc

* 2024-09-17 jmcarcell ([PR#371](https://github.com/key4hep/EDM4hep/pull/371))
  - Relax the requirement for the HepMC3 version

* 2024-09-17 tmadlener ([PR#369](https://github.com/key4hep/EDM4hep/pull/369))
  - Skip the check for the full file in case the RNTuple has not been produce (e.g. if podio hasn't been built with RNTuple support)

* 2024-09-17 tmadlener ([PR#367](https://github.com/key4hep/EDM4hep/pull/367))
  - Fix the cmake warning related to `HepPDT`
  - Make sure that the tests that are enabled by this actually build and run with a suitable HepMC3 version

* 2024-09-16 jmcarcell ([PR#368](https://github.com/key4hep/EDM4hep/pull/368))
  - Use PROJECT_BINARY_DIR instead of CMAKE_BINARY_DIR to always locate the test files

* 2024-09-16 jmcarcell ([PR#366](https://github.com/key4hep/EDM4hep/pull/366))
  - Remove comments in edm4hep.yaml that don't add any information

* 2024-09-16 Mateusz Jakub Fila ([PR#363](https://github.com/key4hep/EDM4hep/pull/363))
  - Added missing attributes for python package

* 2024-09-16 tmadlener ([PR#361](https://github.com/key4hep/EDM4hep/pull/361))
  - Refactor `createEDM4hepFile.py` script to facilitate testing. Apart from the obviously different values in the objects, the overall structure is almost unchanged for the output file.
    - The `MCVertexRecoParticleLinkCollection` has been renamed to the `VertexRecoParticleLinkCollection`
    - The `subdetectorHoleNumbers` are also populated for `Track`s.

* 2024-09-16 Mateusz Jakub Fila ([PR#290](https://github.com/key4hep/EDM4hep/pull/290))
  - Added loading pythonizations from podio

* 2024-09-13 tmadlener ([PR#362](https://github.com/key4hep/EDM4hep/pull/362))
  - Inject the EDM4hep version into the metadata that are stored by podio

* 2024-09-10 jmcarcell ([PR#359](https://github.com/key4hep/EDM4hep/pull/359))
  - Use the Key4hepConfig flag to set the standard, compiler flags and rpath magic.
  - For EDM4hep, the standard keeps being C++20 and the rpath is set as it was before. For the compiler flags `-Wshadow` is added on top of the existing ones.

* 2024-09-09 tmadlener ([PR#360](https://github.com/key4hep/EDM4hep/pull/360))
  - CI: Update versions of github actions where appropriate
  - CI: simplify workflows that build podio and EDM4hep by using cmake commands to invoke the build tool
  - CI: Enable more features from podio for building EDM4hep on top of
    - Also fix a buggy printout that states the commit of podio that was used for building

* 2024-09-04 Mateusz Jakub Fila ([PR#357](https://github.com/key4hep/EDM4hep/pull/357))
  - Fixed typos in member assignment in `createEDM4hep.py`

* 2024-09-02 Mateusz Jakub Fila ([PR#354](https://github.com/key4hep/EDM4hep/pull/354))
  - Removed C++17 compatibility in version header file

* 2024-09-02 jmcarcell ([PR#353](https://github.com/key4hep/EDM4hep/pull/353))
  - Create a RNTuple EDM4hep file (needs podio with RNTuple, if not present the test will be skipped) 
  - Make the createED4MhepFile.py accept an argument for the output file name
  - Add explanations in the README on how to obtain the files.
  - Fix a few issues like https://github.com/key4hep/EDM4hep/issues/355 and https://github.com/key4hep/EDM4hep/issues/356 with the script.

* 2024-08-27 jmcarcell ([PR#343](https://github.com/key4hep/EDM4hep/pull/343))
  - Move to C++20 and remove vector_utils_legacy

* 2024-08-09 jmcarcell ([PR#342](https://github.com/key4hep/EDM4hep/pull/342))
  - Drop support for the ROOTLegacyReader when converting to JSON

* 2024-08-08 jmcarcell ([PR#351](https://github.com/key4hep/EDM4hep/pull/351))
  - Remove a few unnecessary includes

# v00-99

* 2024-07-31 tmadlener ([PR#349](https://github.com/key4hep/EDM4hep/pull/349))
  - Update the `Link` names in the diagram after #341

* 2024-07-30 tmadlener ([PR#350](https://github.com/key4hep/EDM4hep/pull/350))
  - Include the user handle types in the `Collection.h` legacy header files such that they can truly be used transparently.

* 2024-07-30 tmadlener ([PR#348](https://github.com/key4hep/EDM4hep/pull/348))
  - CI: Make sure to use the python bindings of podio that we build in CI

* 2024-07-30 jmcarcell ([PR#347](https://github.com/key4hep/EDM4hep/pull/347))
  - Covariance matrices: Check for the size of the input for one of the constructors
    - `static_cast` the inputs to `float`

* 2024-07-30 tmadlener ([PR#345](https://github.com/key4hep/EDM4hep/pull/345))
  - Bump the schema version of EDM4hep to 2 to have a way to differentiate between *before pre-release* and afterwards

* 2024-07-30 tmadlener ([PR#341](https://github.com/key4hep/EDM4hep/pull/341))
  - Rename the members of the associations to `from` and `to` in preparation for a transparent introduction of templated associations in podio (https://github.com/AIDASoft/podio/pull/257)
    - Add (immediately deprecated) compatibility getters and setters that keep the current behavior on the API side.
  - **Rename `Association` to `Link` and make naming more consistent**
    - Add (immediately deprecated) compatiblity typedefs and headers to keep the current behavior.

* 2024-07-30 Federico Meloni ([PR#337](https://github.com/key4hep/EDM4hep/pull/337))
  - edm4hep::Track: Added Nholes and subdetectorHoleNumbers

* 2024-07-29 jmcarcell ([PR#346](https://github.com/key4hep/EDM4hep/pull/346))
  - Update LCG workflow to use LCG_106 instead of LCG_104

* 2024-07-28 jmcarcell ([PR#272](https://github.com/key4hep/EDM4hep/pull/272))
  - Add a script to create an EDM4hep file. The script is in python and can be called like `python createEDM4hepFile.py`. I have tried to make use of every member and relation in the data model. The purpose is to have something that can create quickly an EDM4hep file and because the script is in python no compilation is needed, hopefully lowering the barrier to get to an EDM4hep file. People can adapt it to their needs or use it as an example for testing, debugging, etc.

* 2024-07-19 tmadlener ([PR#332](https://github.com/key4hep/EDM4hep/pull/332))
  - Make the `Vertex` have relations to the (decay) particles that were used to build it
  - Rename the `ReconstructedParticle` relation to a `Vertex` to `decayVertex` (as dicsussed in [#320](https://github.com/key4hep/EDM4hep/issues/320))

* 2024-07-16 tmadlener ([PR#340](https://github.com/key4hep/EDM4hep/pull/340))
  - Improve the error message of the README links CI check to suggest how to fix it

* 2024-07-16 tmadlener ([PR#311](https://github.com/key4hep/EDM4hep/pull/311))
  - Remove the `dQ/dx` information from the Track
  - Add `TrackPIDHandler` to allow for the reverse lookup

* 2024-07-10 jmcarcell ([PR#339](https://github.com/key4hep/EDM4hep/pull/339))
  - Load libraries without the .so suffix to make it work in other OSes

* 2024-07-08 tmadlener ([PR#336](https://github.com/key4hep/EDM4hep/pull/336))
  - Switch to EL9 as OS for the `dev4` stack based CI workflow

* 2024-07-08 tmadlener ([PR#329](https://github.com/key4hep/EDM4hep/pull/329))
  - Rename the `Vertex` `primary` field to `type` and make it a 32 bit unsigned integer
  - Introduce 3 reserved bits and accompanying utility / ExtraCode functionality for checking primary, secondary or tertiary vertex
  - Introduce `setBit` and `checkBit` utility functionality and also use that for the `MCParticle` extra code

* 2024-07-08 jmcarcell ([PR#326](https://github.com/key4hep/EDM4hep/pull/326))
  - Remove `radiusOfInnermostHit` from tracks

* 2024-07-04 tmadlener ([PR#334](https://github.com/key4hep/EDM4hep/pull/334))
  - Remove compatibility with podio versions before 1.0

* 2024-06-28 tmadlener ([PR#335](https://github.com/key4hep/EDM4hep/pull/335))
  - Remove the association between `TrackerHitPlane` and `SimTrackerHit` from the diagram

* 2024-06-28 tmadlener ([PR#333](https://github.com/key4hep/EDM4hep/pull/333))
  - Remove some of the drift chamber study types that were introduced in [#179](https://github.com/key4hep/EDM4hep/pull/179) again since they need a bit more consideration
    - The removed types and components are: `Hypothesis`, `HitLevelData`, `RecIonizationCluster`, `SimPrimaryIonizationCluster`, `TrackerPulse`
    - Keep the types and components that are already in use, mainly `TimeSeries`
  - Remove the usage of the removed components from `RecDqdx` to make it a purely reconstruction level type
  - Fix script that checks that all types are part of the json dumper

* 2024-06-28 tmadlener ([PR#331](https://github.com/key4hep/EDM4hep/pull/331))
  - Remove the `MCRecoTrackerHitPlaneAssociation` as it is no longer necessary

* 2024-06-28 jmcarcell ([PR#324](https://github.com/key4hep/EDM4hep/pull/324))
  - Change probability to ndf in Vertex

* 2024-06-27 tmadlener ([PR#330](https://github.com/key4hep/EDM4hep/pull/330))
  - Remove docstring of `Track::type` that is no longer relevant for EDM4hep and was a leftover from LCIO.
    - EDM4hep does not (yet) reserve any bits in the Track

* 2024-06-25 Wouter Deconinck ([PR#327](https://github.com/key4hep/EDM4hep/pull/327))
  - Require podio 1.0

* 2024-06-24 Thomas Madlener ([PR#325](https://github.com/key4hep/EDM4hep/pull/325))
  - Introduce a `edm4hep::labels::MCParticles` label to formalize the name of the canonical MCParticle collection

* 2024-06-18 Benedikt Hegner ([PR#310](https://github.com/key4hep/EDM4hep/pull/310))
  - Add `GeneratorEventParameters` and `GeneratorPdfInfo` datatypes to store generator related data in a structured and well defined way.
  - Add a `GeneratorToolInfo` struct and related utility functionality to store some high level metadata on the generator into Frame parameters.

* 2024-06-17 tmadlener ([PR#315](https://github.com/key4hep/EDM4hep/pull/315))
  - Introduce the `edm4hep::labels` namespace to hold the string constants that are used for consistent labeling / naming of collections or parameters.
    - Use this opportunity to homogenize the naming and capitalization of the variables but leave string constants unchanged
  - Deprecate existing string constants

* 2024-06-12 tmadlener ([PR#314](https://github.com/key4hep/EDM4hep/pull/314))
  - Make the python bindings install prefix conform to python conventions by default and allow overriding with the `EDM4HEP_PYTHON_INSTALLDIR` cmake variable
    - Default prefix is now `<prefix>/lib[64]/pythonX.Y/site-packages` (where `lib` or `lib64` is defined from the platform defaults and `X.Y` is the python major and minor version).

* 2024-06-11 tmadlener ([PR#313](https://github.com/key4hep/EDM4hep/pull/313))
  - Switch to `alma9` image to fix documentation deploys

* 2024-06-11 tmadlener ([PR#307](https://github.com/key4hep/EDM4hep/pull/307))
  - To have a more consistent way of filling the `algoType` in the `edm4hep::utils::ParticleIDMeta` use the 32 bit version of MurmurHash3 to hash the `algoName` to get to `algoType`. Fixes #298 
    - Make `algoType` a private member but allow read access to it via the `algoType` member function. `algoType` has to be filled on construction. It is still possible to set it manually, the hashing will only kick in for the constructor taking only a name.

* 2024-06-11 Juraj Smiesko ([PR#283](https://github.com/key4hep/EDM4hep/pull/283))
  - Adding metadata name for event filter statistics.

* 2024-06-04 tmadlener ([PR#303](https://github.com/key4hep/EDM4hep/pull/303))
  - Update the schema diagram to reflect the changes from [#268](https://github.com/key4hep/EDM4hep/pull/268)

* 2024-06-03 Mateusz Jakub Fila ([PR#308](https://github.com/key4hep/EDM4hep/pull/308))
  - Use declarationFile instead of include in CovMatrix to fix generated documentation for covariance matrix components. Fixes [#296](https://github.com/key4hep/EDM4hep/issues/296)

* 2024-05-21 tmadlener ([PR#302](https://github.com/key4hep/EDM4hep/pull/302))
  - Update the schema diagram to reflect the `TrackerHit` as interface
    - Add `TrackerHit3D` as new type
  - Fix all arrow heads to be consistently black

* 2024-05-16 tmadlener ([PR#305](https://github.com/key4hep/EDM4hep/pull/305))
  - Introduce pre-processor checks to transparently switch to the new `std::optional` return values of `podio::Frame::getParameter` (introduced with [AIDASoft/podio#580](https://github.com/AIDASoft/podio/pull/580))

* 2024-05-14 jmcarcell ([PR#304](https://github.com/key4hep/EDM4hep/pull/304))
  - Remove ParticleID that was deleted in https://github.com/key4hep/EDM4hep/pull/268 and reintroduced in https://github.com/key4hep/EDM4hep/pull/287

* 2024-05-07 Andre Sailer ([PR#300](https://github.com/key4hep/EDM4hep/pull/300))
  - DOC: change association descriptions to allow doxygen to link to respective classes

* 2024-05-02 Mateusz Jakub Fila ([PR#297](https://github.com/key4hep/EDM4hep/pull/297))
  - Fix datatypes table formatting. Fix typos

* 2024-05-01 tmadlener ([PR#287](https://github.com/key4hep/EDM4hep/pull/287))
  - Introduce `edm4hep::CovMatrix[2,3,4,6}f` components to represent covariance matrices
    - using `std::array<float, N>` under the hood
    - Access functionality that works on enums for defining dimensions and some utilities to do the indexing into the underlying storage
  - Introduce some `enum class`es for defining dimensions, e.g. `TrackParams` or `Cartesian`
  - Use these components instead of the raw `std::array`s in datatypes and other components and tie the interpretation of the values to the approriate dimension `enum class`
  
  Usage example:
  ```cpp
  #include "edm4hep/TrackerHit3D.h"
  
  void foo(const edm4hep::TrackerHit3D& hit) {
    const auto covXY  = hit.getCovMatrix(edm4hep::Cartesian::x, edm4hep::Cartesian:y);
  }
  ```

* 2024-05-01 tmadlener ([PR#268](https://github.com/key4hep/EDM4hep/pull/268))
  - Remove the relations from `Cluster` to `ParticleID`
  - Remove the relations from `ReconstructedParticle` to `ParticleID`
  - Add a `OneToOneRelation` from `ParticleID` to a `ReconstructedParticle`
  - **This is a breaking change for both existing files and interfaces. There will be no schema evolution for this change**
  - Add a new `edm4hep::utils::PIDHandler` and some related utility functionality to help with handling the necessary meta data.

* 2024-04-19 tmadlener ([PR#295](https://github.com/key4hep/EDM4hep/pull/295))
  - Make sure that the `utils` (currently INTERFACE only) target appears as `edm4hepUtils` "on disk" in order to avoid having on overly generic `libutils.so` once it actually becomes a shared library.

* 2024-04-19 tmadlener ([PR#289](https://github.com/key4hep/EDM4hep/pull/289))
  - Only pass what is strictly necessary as input to doxygen
  - Exclude dependencies to avoid generating empty namespaces in the EDM4hep API reference

* 2024-04-08 jmcarcell ([PR#294](https://github.com/key4hep/EDM4hep/pull/294))
  - Load Constants.h in python, making some names available like `edm4hep::EventHeaderName`

* 2024-04-04 tmadlener ([PR#293](https://github.com/key4hep/EDM4hep/pull/293))
  - Use non deprecated versions of the `SIOWriter` and `SIOReader`

* 2024-04-02 jmcarcell ([PR#292](https://github.com/key4hep/EDM4hep/pull/292))
  - Do not try to set the read-only property ALIAS_GLOBAL

* 2024-03-12 tmadlener ([PR#286](https://github.com/key4hep/EDM4hep/pull/286))
  - Make sure that `operator[]` of the `VectorNx` types return the correct type.

* 2024-03-12 Mateusz Jakub Fila ([PR#285](https://github.com/key4hep/EDM4hep/pull/285))
  - added static asserting vector components' member ordering and padding

* 2024-02-26 tmadlener ([PR#282](https://github.com/key4hep/EDM4hep/pull/282))
  - Bump the patch version to 99 in order to keep preprocessor version checks working for current tagged version.

* 2024-02-26 tmadlener ([PR#281](https://github.com/key4hep/EDM4hep/pull/281))
  - Add `operator!=` to `VectorXY` classes to keep symmetry with updated podio generated code.

* 2024-02-25 Mateusz Jakub Fila ([PR#280](https://github.com/key4hep/EDM4hep/pull/280))
  - Fix readme formatting and links to trackerhit

* 2024-02-23 jmcarcell ([PR#279](https://github.com/key4hep/EDM4hep/pull/279))
  - Fix a few compiler warnings, some of them introduced in 1.0 PRs

* 2024-02-23 tmadlener ([PR#252](https://github.com/key4hep/EDM4hep/pull/252))
  - Make the `edm4hep::TrackerHit` an `interface` type and use that in the `edm4hep::Track`.
  - Rename the current `edm4hep::TrackerHit` to `edm4hep::TrackerHit3D`
  - **This is a breaking change. However, we currently do not plan to implement schema evolution for this**

* 2024-02-22 jmcarcell ([PR#278](https://github.com/key4hep/EDM4hep/pull/278))
  - Remove any path containing /fccanalyses/ for one test to prevent ROOT from loading FCCAnalyses dictionaries

* 2024-02-22 tmadlener ([PR#277](https://github.com/key4hep/EDM4hep/pull/277))
  - Use the units explictly in the yaml file to make them appear nicer in the generated docstrings.

* 2024-02-22 BrieucF ([PR#276](https://github.com/key4hep/EDM4hep/pull/276))
  - "EDM4hep authors" as `Author` field for all data types. Fixes https://github.com/key4hep/EDM4hep/issues/255
  - Remove the confusing reference to collection parameters. Fixes https://github.com/key4hep/EDM4hep/issues/191
  - Homogenize the comments (dot at the end or not, space after // or not).

* 2024-02-22 jmcarcell ([PR#275](https://github.com/key4hep/EDM4hep/pull/275))
  - Remove BITEndpoint from MCParticle

* 2024-02-22 jmcarcell ([PR#274](https://github.com/key4hep/EDM4hep/pull/274))
  - Rename EDep to eDep in SimTrackerHit for consistency with other uses of "eDep"

* 2024-02-22 jmcarcell ([PR#273](https://github.com/key4hep/EDM4hep/pull/273))
  - Rename type to PDG in ReconstructedParticle

* 2024-02-22 Thomas Madlener ([PR#267](https://github.com/key4hep/EDM4hep/pull/267))
  - Bring back MCParticle momenta as doubles. See #237 and #266

* 2024-02-22 tmadlener ([PR#260](https://github.com/key4hep/EDM4hep/pull/260))
  - Remove the `edm4hep::ObjectID` component. Fixes #259 
  - Remove its usage from the `edm4hep::TrackerHit` and `edm4hep::TrackerHitPlane`
  - **This is a breaking change both in code and existing files**. Existing usages might switch to use the `podio::ObjectID` to have effectively the same functionality.

* 2024-02-22 tmadlener ([PR#256](https://github.com/key4hep/EDM4hep/pull/256))
  - Rename `MCParticle` relations to `particle` in `SimTrackerHit` and `SimPrimaryIonizationCluster` to avoid clashes with the `MCParticle` typename.
    - Add `getMCParticle` and `setMCParticle` via `ExtraCode` to keep everything working.
    - **This will break the reading of existing files via podio utilities**

* 2024-02-22 jmcarcell ([PR#254](https://github.com/key4hep/EDM4hep/pull/254))
  - Add a `double weight` `VectorMember` to the `EventHeader` to allow storing multiple weights.
    - **This should not break any existing code as we simply add a new member. However, this will break the possibility of reading existing files through podio**.
  - Introduce the `edm4hep::EventWeights` constant to have a consistent name for storing / accessing them in file level metadata.

* 2024-02-12 tmadlener ([PR#269](https://github.com/key4hep/EDM4hep/pull/269))
  - Put the `schema_version` to the correct level for the downstream usage tests to keep it working after [podio#556)(https://github.com/AIDASoft/podio/pull/556)

# v00-10-05

* 2024-02-07 Thomas Madlener ([PR#266](https://github.com/key4hep/EDM4hep/pull/266))
  - Revert making MCParticle momenta double precision, since this breaks without schema evolution. (Reverts key4hep/EDM4hep#237)
    - All particle momenta are effectively zeroed out at the moment.

* 2024-02-07 jmcarcell ([PR#264](https://github.com/key4hep/EDM4hep/pull/264))
  - Delete build workflow since we have another one for key4hep that covers builds for nightlies, releases and all the operating systems we support

* 2024-02-06 jmcarcell ([PR#263](https://github.com/key4hep/EDM4hep/pull/263))
  - Change ROOTFrame{Writer,Reader} to ROOT{Writer,Reader} following https://github.com/AIDASoft/podio/pull/549

# v00-10-04

* 2024-01-30 tmadlener ([PR#258](https://github.com/key4hep/EDM4hep/pull/258))
  - Bump the version in the schema diagram to v0.10

* 2024-01-23 tmadlener ([PR#237](https://github.com/key4hep/EDM4hep/pull/237))
  - Switch the `momentum` and `momentumAtEndpoint` of the `MCParticle` from `Vector3f` to `Vector3d`. 
  - Add a (deprecated from the beginning) constructor from `Vector3f` to `Vector3d` to ease the transition.

* 2024-01-22 tmadlener ([PR#253](https://github.com/key4hep/EDM4hep/pull/253))
  - Fix she-bang in README links update script.

* 2024-01-12 Mateusz Jakub Fila ([PR#250](https://github.com/key4hep/EDM4hep/pull/250))
  - Fixed doxygen excluded files and path stripping

* 2024-01-08 Mateusz Jakub Fila ([PR#249](https://github.com/key4hep/EDM4hep/pull/249))
  - Fixed formatting of components table in readme

# v00-10-03

* 2024-01-08 Joe Osborn ([PR#248](https://github.com/key4hep/EDM4hep/pull/248))
  - Added a Vector4f object for use as a general 4 dimensional vector with members `x`, `y`, `z` and `t`. Fixes https://github.com/key4hep/EDM4hep/issues/245

* 2023-12-12 Mateusz Jakub Fila ([PR#244](https://github.com/key4hep/EDM4hep/pull/244))
  - Fixed typos and links in documentation and doxygen

* 2023-12-06 tmadlener ([PR#243](https://github.com/key4hep/EDM4hep/pull/243))
  - Make sure that tests also work in Ubuntu 20.04 environments by running catch test discovery in correct environment.

* 2023-12-05 jmcarcell ([PR#235](https://github.com/key4hep/EDM4hep/pull/235))
  - Use `FILE_SET` to install the headers in the top folder together with the library. This also adds them in the `BUILD_INTERFACE`, something that a simple `INSTALL` doesn't do.
  - Bump the CMake version of the LCG stacks
  - Simplify finding ROOT, don't do environment variable manipulation in CMake

# v00-10-02

* 2023-11-14 jmcarcell ([PR#240](https://github.com/key4hep/EDM4hep/pull/240))
  - Change EventHeader to EventHeaderName; we already have an EventHeaderCollection and its elements are called EventHeader (`edm4hep::EventHeader` more precisely)

* 2023-11-14 Leonhard Reichenbach ([PR#239](https://github.com/key4hep/EDM4hep/pull/239))
  - Add a constant for the default expected EventHeader name to be used by the converters and PodioInput

# v00-10-01

* 2023-11-01 jmcarcell ([PR#234](https://github.com/key4hep/EDM4hep/pull/234))
  - Add a constant for CellIDEncoding. Usage:
  
  ``` cpp
  #include "edm4hep/Constants.h"
  
  std::cout << edm4hep::CellIDEncoding << std::endl;
  ```

* 2023-11-01 Juraj Smiesko ([PR#227](https://github.com/key4hep/EDM4hep/pull/227))
  - edm4hep2json now converts all EDM4hep collections, associations and Podio user data
  - added ROOT legacy support for edm4hep2json

* 2023-09-13 jmcarcell ([PR#226](https://github.com/key4hep/EDM4hep/pull/226))
  - Rename the cmake executable or target `unittests` to `unittests_edm4hep`, to avoid possible collisions since the `unittests` name is relatively common

* 2023-09-11 Wouter Deconinck ([PR#225](https://github.com/key4hep/EDM4hep/pull/225))
  - Define schema_version at top level in yaml file

* 2023-09-11 tmadlener ([PR#224](https://github.com/key4hep/EDM4hep/pull/224))
  - Add `SKIP_CATCH_DISCOVERY` option to turn of `catch_discover_tests` which may not run in the right environment in older cmake versions.

* 2023-09-05 Andre Sailer ([PR#223](https://github.com/key4hep/EDM4hep/pull/223))
  - CI: use same lcg stacks as podio
  - Test: update to Catch2 3.4.0, same as in podio, and c++20 compatible

* 2023-09-05 jmcarcell ([PR#222](https://github.com/key4hep/EDM4hep/pull/222))
  - Remove init.sh

# v00-10

* 2023-07-07 BrieucF ([PR#212](https://github.com/key4hep/EDM4hep/pull/212))
  - Add a script to automatically update the README.md links

* 2023-07-04 tmadlener ([PR#209](https://github.com/key4hep/EDM4hep/pull/209))
  - Add brief documentation of the `edm4hep2json` tool with minimal documentation of the output JSON schema.

* 2023-06-30 jmcarcell ([PR#207](https://github.com/key4hep/EDM4hep/pull/207))
  - Improve python API, use `import edm4hep` instead of `from edm4hep import edm4hep`

* 2023-06-29 tmadlener ([PR#206](https://github.com/key4hep/EDM4hep/pull/206))
  - Replace TPCHit with RawTimeSeries
  - Update version to 0.9
  - Add TrackerPulse

* 2023-06-14 jmcarcell ([PR#204](https://github.com/key4hep/EDM4hep/pull/204))
  - Add python bindings for the datamodel classes and some documentation on how to use the bindings

* 2023-06-07 FinnJohannsen ([PR#188](https://github.com/key4hep/EDM4hep/pull/188))
  - Changed the name of one VectorMember of edm4hep::track from subDetectorHitNumbers to subdetectorHitNumbers to be consistent with other spellings of the word subdetector in the yaml file and in LCIO.

# v00-09

* 2023-05-03 Thomas Madlener ([PR#152](https://github.com/key4hep/EDM4hep/pull/152))
  - Add a `EDM4hepVersion.h` file that has the same basic structure and functionality as other Key4hep packages.

* 2023-05-02 jmcarcell ([PR#193](https://github.com/key4hep/EDM4hep/pull/193))
  - Add a configuration file for the new podio visualization tool

* 2023-04-28 jmcarcell ([PR#203](https://github.com/key4hep/EDM4hep/pull/203))
  - Remove root version check inside CMakeLists.txt

* 2023-04-26 jmcarcell ([PR#205](https://github.com/key4hep/EDM4hep/pull/205))
  - Add missing units to the comments

* 2023-04-23 Thomas Madlener ([PR#200](https://github.com/key4hep/EDM4hep/pull/200))
  - Add `schema_version` to YAML definition now that podio has limited support (see AIDASoft/podio#341)

# v00-08

* 2023-04-03 Paul Gessinger ([PR#201](https://github.com/key4hep/EDM4hep/pull/201))
  - Added string stream include in `edm4hep2json.hxx`

* 2023-03-02 wenxingfang ([PR#179](https://github.com/key4hep/EDM4hep/pull/179))
  - Extend for drift chamber and TPC study
  - The extended EDMs are tested within CEPCSW, and it works well
  - The extended edm4hep::TrackerPulse and edm4hep::TrackerData are similar to what they are in ILC TPC

* 2023-02-27 Juraj Smiesko ([PR#182](https://github.com/key4hep/EDM4hep/pull/182))
  - JSON Exporter: Adding the possibility to provide list of events
  - Update JSON Exporter to use Frame based I/O

* 2023-02-22 Thomas Madlener ([PR#194](https://github.com/key4hep/EDM4hep/pull/194))
  - Make the HepMC example use Frame based I/O

* 2023-02-21 Thomas Madlener ([PR#184](https://github.com/key4hep/EDM4hep/pull/184))
  - Make the I/O tests use `podio::Frame` functionality

* 2023-02-01 Dmitry Kalinkin ([PR#190](https://github.com/key4hep/EDM4hep/pull/190))
  - Document some units for edm4hep::Vector3f fields

* 2023-01-23 Thomas Madlener ([PR#189](https://github.com/key4hep/EDM4hep/pull/189))
  - Make the minimum nlohmann json version 3.10.5, as other patch releases of the 3.10 series, seem to not compile. Fixes #181

* 2023-01-06 lintao ([PR#187](https://github.com/key4hep/EDM4hep/pull/187))
  - Fix a typo in setDecayedInCalorimeter.

# v00-07-02

* 2022-11-29 Thomas Madlener ([PR#186](https://github.com/key4hep/EDM4hep/pull/186))
  - Use agreed upon spelling of lower case "hep" on doxygen ref page header.

* 2022-11-15 Thomas Madlener ([PR#185](https://github.com/key4hep/EDM4hep/pull/185))
  - Only build JSON support if a suitable version of nlohmann/json is found. This should fix a few issues in CI.
  - Fix formatting of `edm4hep2json`.
  - Update versions of github actions used in CI to latest available.

# v00-07-01

* 2022-10-18 Juraj Smiesko ([PR#163](https://github.com/key4hep/edm4hep/pull/163))
  - Adding  EDM4hep -> Phoenix json conversion executable

# v00-07-01

* 2022-10-18 Juraj Smiesko ([PR#163](https://github.com/key4hep/edm4hep/pull/163))
  - Adding  EDM4hep -> Phoenix json conversion executable

# v00-07

* 2022-09-27 Juraj Smiesko ([PR#178](https://github.com/key4hep/EDM4hep/pull/178))
  - Updated line links to datatypes/components/associations in the README

* 2022-09-27 Thomas Madlener ([PR#176](https://github.com/key4hep/EDM4hep/pull/176))
  - Bump the minimum cmake version to 3.16. Fixes #166 
  - Update minimally required versions of podio and ROOT in README

* 2022-08-25 Thomas Madlener ([PR#175](https://github.com/key4hep/EDM4hep/pull/175))
  - Move the dataframe utilities to the utils folder
  - Make the existing functionality templated and explicitly instantiate all possible datatypes
  - Add a python RDataFrame test

* 2022-08-24 Wouter Deconinck ([PR#149](https://github.com/key4hep/EDM4hep/pull/149))
  - Add utilities to work with `Vector` component classes including
    - (`constexpr`) `operator` overloads for basic arithmetic with vectors
    - utility functionality that is commonly used with vectors
    - concepts to work with in generic vector related code.
  - Include `edm4hep/utils/vector_utils.h` and work with functionality in `edm4hep::utils` namespace
    - This include will automatically use the concept powered version if supported by the compiler and fall back to a legacy implementation if not.

* 2022-08-23 Thomas Madlener ([PR#174](https://github.com/key4hep/EDM4hep/pull/174))
  - Remove no longer necessary tests covering subset collection functionality

* 2022-08-23 Thomas Madlener ([PR#173](https://github.com/key4hep/EDM4hep/pull/173))
  - Add a `pre-commit` and a `clang-format` config file and run them in CI.

* 2022-08-18 Thomas Madlener ([PR#172](https://github.com/key4hep/EDM4hep/pull/172))
  - Fix the `kinematics.h` header only utility file such that it also works when used via PyROOT.

* 2022-08-18 stoimenova ([PR#158](https://github.com/key4hep/EDM4hep/pull/158))
  - Expanded write_events and read_events test with two new classes

# v00-06

* 2022-08-11 Thomas Madlener ([PR#171](https://github.com/key4hep/EDM4hep/pull/171))
  - Update the version string in the diagram

* 2022-08-10 Andre Sailer ([PR#169](https://github.com/key4hep/EDM4hep/pull/169))
  - RDF Test: add libedm4hepRDF.so directory to LD_LIBRARY_PATH, fixes #167

* 2022-08-10 Wouter Deconinck ([PR#165](https://github.com/key4hep/EDM4hep/pull/165))
  - Add a datamodel derived from EDM4hep to the downstream package tests

* 2022-08-03 Wouter Deconinck ([PR#164](https://github.com/key4hep/EDM4hep/pull/164))
  - Install edm4hep.yaml to CMAKE_INSTALL_DATADIR/edm4hep. The `EDM4HEP_DATA_DIR` cmake variable points to this directory when using edm4hep via `find_package(EDM4HEP)`.

* 2022-07-18 Valentin Volkl ([PR#162](https://github.com/key4hep/EDM4hep/pull/162))
  - [cmake] set EDM4HEP_SET_RPATH ON by default
  - [cmake] deduplicate RPATH settings

* 2022-06-17 Valentin Volkl ([PR#159](https://github.com/key4hep/EDM4hep/pull/159))
  - Fix doxygen generation - update setup on gitlab
  - Update version string in diagram

# v00-05

* 2022-06-14 clementhelsens ([PR#138](https://github.com/key4hep/EDM4hep/pull/138))
  - TrackState: add time field (and adapt covariance matrix accordingly)

* 2022-06-10 Thomas Madlener ([PR#156](https://github.com/key4hep/EDM4hep/pull/156))
  - Make EDM4hep export its current version properly for usage in downstream CMake packages.
    - Make sure downstream package test covers this.

* 2022-05-26 clementhelsens ([PR#137](https://github.com/key4hep/EDM4hep/pull/137))
  - Adding dNdX to edm4hep::Track as discussed https://indico.cern.ch/event/1126908/

* 2022-05-24 Thomas Madlener ([PR#140](https://github.com/key4hep/EDM4hep/pull/140))
  - Remove the `hitContributions` vector member from the `edm4hep::Cluster` as it was discovered to be unused in LCIO.

* 2022-05-24 Thomas Madlener ([PR#139](https://github.com/key4hep/EDM4hep/pull/139))
  - Remove the 'edx` member from the `edm4hep::TrackeHit` as it is there by accident and a hit has no dx.

# v00-04-02

* 2022-05-24 Laurent Forthomme ([PR#151](https://github.com/key4hep/EDM4hep/pull/151))
  - const-qualified ReconstructedParticle::isCompound method

* 2022-04-12 Valentin Volkl ([PR#153](https://github.com/key4hep/EDM4hep/pull/153))
  - remove tricktrack dependency and tests ( to be moved downstream)

* 2022-03-18 Thomas Madlener ([PR#148](https://github.com/key4hep/EDM4hep/pull/148))
  - Update the compilers that are used for the `dev` based CI workflows

# v00-04-01

* 2022-03-11 Thomas Madlener ([PR#143](https://github.com/key4hep/edm4hep/pull/143))
  - Move the install destination of the `kinematics.h` utility header to be in `<prefix>/include/edm4hep/utils` (previously was in `<prefix>/include/utils/edm4hep`. This makes it better aligned with how namespacing is done, which is `edm4hep::utils`.
  - Add minimal documentation for the `utils` folder.

* 2022-02-28 Thomas Madlener ([PR#142](https://github.com/key4hep/edm4hep/pull/142))
  - Remove the LCG release workflows that do not build podio@master on the fly because
  - Update status badges

* 2022-02-28 Valentin Volkl ([PR#141](https://github.com/key4hep/edm4hep/pull/141))
  - doc: add setup script to readme

* 2022-01-26 Thomas Madlener ([PR#136](https://github.com/key4hep/edm4hep/pull/136))
  - Update the supported c++ standards in the CMake configuration (17 and 20 at the moment).

* 2022-01-17 Thomas Madlener ([PR#132](https://github.com/key4hep/edm4hep/pull/132))
  - With AIDASoft/podio#205 the naming of the podio generated classes has changed and the immutable classes are now default, while mutable classes now have a `Mutable` prefix.

* 2021-12-14 Placido Fernandez Declara ([PR#133](https://github.com/key4hep/edm4hep/pull/133))
  - Add association between ReconstructedParticle and Vertex

* 2021-12-14 Thomas Madlener ([PR#125](https://github.com/key4hep/edm4hep/pull/125))
  - Use fixed width integer types throughout the whole datamodel.

* 2021-11-26 Valentin Volkl ([PR#131](https://github.com/key4hep/edm4hep/pull/131))
  - switch to hepmc3 in tests

* 2021-11-23 Valentin Volkl ([PR#130](https://github.com/key4hep/edm4hep/pull/130))
  - [doc] update diagram for v0.4

* 2021-10-22 Thomas Madlener ([PR#126](https://github.com/key4hep/edm4hep/pull/126))
  - Fix test dependencies to make it possible to run them in parallel via the `-j` flag.
  - Run the tests requiring subset collection functionality only if podio already supports it.

# v00-04

* 2021-09-06 Placido Fernandez Declara ([PR#122](https://github.com/key4hep/edm4hep/pull/122))
  - Adds missing types to run clicReconstruction with EDM4hep input/output
  - Adds Vector2f, TrackerHitPlane, MCRecoTrackerHitPlaneAssociation, MCRecoCaloParticleAssociation, MCRecoClusterParticleAssociation, MCRecoTrackParticleAssociation

* 2021-09-03 Thomas Madlener ([PR#116](https://github.com/key4hep/edm4hep/pull/116))
  - Remove the `RecoParticleRef` datatype again because podio supports subset collections natively now (see AIDASoft/podio#197)

* 2021-07-28 Thomas Madlener ([PR#120](https://github.com/key4hep/edm4hep/pull/120))
  - Add first utility functionality for kinematics living in namespace `edm4hep::utils`:
    - `edm4hep::utils::p` returns the total momentum for the passed particle
    - `edm4hep::utils::pt` and `edm4hep::utils::pT` returns the transverse momentum for the passed particle
    - `edm4hep::utils::p4` returns the four momentum of the passed particle, depending on the (optional) second argument it uses the momentum of the passed particle and either
      - the mass of the passed particle if the second argument is `edm4hep::utils::UseMass` (**default**)
      - the energy of the passed particle if the second argument is `edm4hep::utils::UseEnergy`
      - an arbitrary value for either the mass or the energy if the second argument is `edm4hep::utils::SetMass{value}` or `edm4hep::utils::SetEnergy{value}` respectively.

* 2021-07-22 Valentin Volkl ([PR#119](https://github.com/key4hep/edm4hep/pull/119))
  -  [ci] remove travis config (superseded by github actions)
  -  [ci] rename workflows (`s/linux/lcg_linux/` as linux on its own is not very informative ) and fix badges.

* 2021-07-22 Thomas Madlener ([PR#118](https://github.com/key4hep/edm4hep/pull/118))
  - Add github actions CI workflows
  - Fix issue with CI builds that build podio on the fly

# v00-03-01

* 2021-02-23 Valentin Volkl ([PR#110](https://github.com/key4hep/edm4hep/pull/110))
  - Update relation diagram for docs

* 2021-02-11 Andre Sailer ([PR#108](https://github.com/key4hep/edm4hep/pull/108))
  - TrackState Covariance: should be the lower triangle, like other covariance matrices, fixes #107

* 2021-02-02 Valentin Volkl ([PR#105](https://github.com/key4hep/edm4hep/pull/105))
  - Use gitlab CI to deploy doxygen site to eos

# v00-03

* 2020-12-03 Valentin Volkl ([PR#101](https://github.com/key4hep/edm4hep/pull/101))
  - Remove DDG4 plugin (moves to upstream dd4hep, https://github.com/AIDASoft/DD4hep/pull/740)

* 2020-12-03 vvolkl ([PR#98](https://github.com/key4hep/edm4hep/pull/98))
  - Add rpath setup and update compiler flags and linker setup

* 2020-11-24 Valentin Volkl ([PR#97](https://github.com/key4hep/edm4hep/pull/97))
  - [cmake] add call to find_package_handle_standard_args to get a Found: msg

* 2020-11-11 tmadlener ([PR#84](https://github.com/key4hep/edm4hep/pull/84))
  - Use new cmake macros from podio to build the datamodel libraries and dictionary, building everything that is supported by podio.
  - Add tests for reading/writing using the SIO backend.

* 2020-11-10 Valentin Volkl ([PR#94](https://github.com/key4hep/edm4hep/pull/94))
  - Remove Delphes plugin (which is migrated to its own repository key4hep/k4simdelphes)

* 2020-11-08 clementhelsens ([PR#93](https://github.com/key4hep/edm4hep/pull/93))
  This patch is to avoid a segmentation fault when calling the Delphes treeWriter only occurring when reading a LHE file from Pythia8

* 2020-11-08 clementhelsens ([PR#92](https://github.com/key4hep/edm4hep/pull/92))
  - This add a new executable to run EvtGen with the Pythia8 plugin

* 2020-11-08 vvolkl ([PR#88](https://github.com/key4hep/edm4hep/pull/88))
  - update lcg release used in init script to 97a

* 2020-11-05 tmadlener ([PR#90](https://github.com/key4hep/edm4hep/pull/90))
  - Fix double free in Delphes EDM4Hep converter that lead to spurious segmentation violations (#89)

* 2020-10-14 tmadlener ([PR#83](https://github.com/key4hep/edm4hep/pull/83))
  - Make delphes output configuration more consistent, by using branch names also for the generated particle collections.

* 2020-10-14 Valentin Volkl ([PR#82](https://github.com/key4hep/edm4hep/pull/82))
  - save the delphes particle status as edm4hep::MCparticle.GeneratorStatus

* 2020-10-14 clementhelsens ([PR#81](https://github.com/key4hep/edm4hep/pull/81))
  - Add a python script to convert EDM4Hep to LCIO
  - Currently just converting ReconstructedParticles and Tracks

* 2020-10-13 Valentin Volkl ([PR#85](https://github.com/key4hep/edm4hep/pull/85))
  - [ci] fix the library path for builds with a local podio

* 2020-09-16 Frank Gaede ([PR#79](https://github.com/key4hep/edm4hep/pull/79))
  - fix delphes EDM4Hep plugin
       - set energy for clusters and reco particles in DelphesEDM4HepConverter
       - add example higgs_recoil_plots.C  (same as in delphes2lcio)
       - minor fix for building DelphesPythia8 on MacOS
       - make plugin compatible w/ Pythia8.3

* 2020-09-15 vvolkl ([PR#80](https://github.com/key4hep/edm4hep/pull/80))
  - Fix CI to pick up the local podio at runtime

# v00-02

* 2020-08-20 Valentin Volkl ([PR#78](https://github.com/key4hep/edm4hep/pull/78))
  - Add a cmake option for cpack

* 2020-08-19 Valentin Volkl ([PR#77](https://github.com/key4hep/edm4hep/pull/77))
  - CI Update: Add option to build PODIO on the fly, dev3

* 2020-08-19 Valentin Volkl ([PR#76](https://github.com/key4hep/edm4hep/pull/76))
  - Fix capitalization in edm4hep.yaml: pDG -> PDG

* 2020-08-19 tmadlener ([PR#69](https://github.com/key4hep/edm4hep/pull/69))
  - Add additional comment to `mass`, `energy` and `momentum` members of `ReconstructedParticle` description to clearly state that the 4-momentum state is not automatically kept consistent internally. Since there are too many different use-cases, a general solution that always works was deemed hard to achieve and it is, thus, the users responsibility to keep this state consistent if it is necessary.

* 2020-08-11 tmadlener ([PR#75](https://github.com/key4hep/edm4hep/pull/75))
  - Updated datamodel definition yaml file to work with new podio version

* 2020-08-11 tmadlener ([PR#56](https://github.com/key4hep/edm4hep/pull/56))
  - Add a converter module for delphes that can output edm4hep files. The module is used in several standalone delphes executables that take different input formats (e.g. stdhep, pythia8, etc.) and **standard delphes cards** and produce edm4hep output root files. It can also be used in conjunction with gaudi. **This is an early release to gather feedback and to iterate on that feedback. Currently not all of the input readers that come with standard delphes are implemented and also the information content of the edm4hep files is restricted to the most important information. More information can be added on demand.** A brief introduction and some examples are included.

* 2020-08-04 vvolkl ([PR#73](https://github.com/key4hep/edm4hep/pull/73))
  - Change DD4hep environment variable in tests

* 2020-06-23 Andre Sailer ([PR#68](https://github.com/key4hep/edm4hep/pull/68))
  - The rootmap file should point to the dictionary library, and not the base library
  
  - Tests: Fill the LD_LIBRARY_PATH environment variable with the location of the rootmap file

* 2020-06-23 Frank Gaede ([PR#65](https://github.com/key4hep/edm4hep/pull/65))
  - finalize the EDM4hep output module for DDG4/ddsim
        - add treatment of event parameter meta data

* 2020-06-18 tmadlener ([PR#67](https://github.com/key4hep/edm4hep/pull/67))
  - mark MCParticle::getEnergy const to make it accessible in const context

* 2020-05-26 Frank Gaede ([PR#63](https://github.com/key4hep/edm4hep/pull/63))
  - fix linking on Ubuntu
       - use -Wl,--no-as-needed for linking edm4hepDict (if available)
       - follows https://github.com/AIDASoft/podio/pull/91

* 2020-05-26 Thomas Madlener ([PR#62](https://github.com/key4hep/edm4hep/pull/62))
  - Adapt to new naming of functions in podio

* 2020-05-26 Valentin Volkl ([PR#58](https://github.com/key4hep/edm4hep/pull/58))
  - Rename cluster vectormembers with a plural form

* 2020-05-25 Frank Gaede ([PR#61](https://github.com/key4hep/edm4hep/pull/61))
  - adapt for Macos:
       - add missing libs and includes for dictionary building
  - adapt to method name changes in https://github.com/AIDASoft/podio/pull/88

* 2020-05-19 Valentin Volkl ([PR#57](https://github.com/key4hep/edm4hep/pull/57))
  - add weights to associations

# v00-01

* 2020-04-14 Frank Gaede ([PR#53](https://github.com/key4hep/edm4hep/pull/53))
  - allow automatic change of version in CMakeLists.txt
         - introduced version variables

* 2020-04-07 Andre Sailer ([PR#51](https://github.com/key4hep/edm4hep/pull/51))
  - Use PODIO_GENERATE_DATAMODEL, needs #aidasoft/podio#73, fixes #49

* 2020-03-20 Valentin Volkl ([PR#44](https://github.com/key4hep/edm4hep/pull/44))
  - Add EventHeader data type

* 2020-03-18 Valentin Volkl ([PR#43](https://github.com/key4hep/edm4hep/pull/43))
  - add DDG4EDM4hep plugin for use with ddsim

* 2020-03-16 vvolkl ([PR#48](https://github.com/key4hep/edm4hep/pull/48))
  - Adapt to the changes in AIDASoft/podio#70 , add "includeSubfolder" option
  - Adapt to the changes in AIDASoft/podio#69 , splitting of podio libraries
  - CMake: Add logic to re-run class generator if edm4hep.yaml changes

* 2020-03-13 Valentin Volkl ([PR#40](https://github.com/key4hep/edm4hep/pull/40))
  -  Improvements  to doxygen configuration

* 2020-03-04 Valentin Volkl ([PR#46](https://github.com/key4hep/edm4hep/pull/46))
  - fix cmake target include and add test of edm4hep cmake targets

* 2020-02-19 Andre Sailer ([PR#39](https://github.com/key4hep/edm4hep/pull/39))
  - Fix installation location of EDM4hepVersion.h (EDM4HEP -> edm4hep)
  - Change EDM4hep*.cmake install location to `${CMAKE_INSTALL_LIBDIR}/cmake/EDM4HEP`
  - Merge edm4hep dictionary library into main edm4hep library
  - Merge edm4hepRDF dictionary library into main edm4hepRDF library
  - Use CMAKE_INSTALL_*DIR variables defined by GNUInstallDirs

* 2020-02-19 Valentin Volkl ([PR#32](https://github.com/key4hep/edm4hep/pull/32))
  - add library + dict with lambdas for use with rdataframe

* 2020-02-17 Andre Sailer ([PR#36](https://github.com/key4hep/edm4hep/pull/36))
  - Cmake: if BUILD_TESTING=OFF no tests are compiled

* 2020-02-17 Valentin Volkl ([PR#35](https://github.com/key4hep/edm4hep/pull/35))
  - build system bugfix: Ensure that the CI picks up local version of packages also present in LCG

* 2020-02-17 Andre Sailer ([PR#34](https://github.com/key4hep/edm4hep/pull/34))
  - Cmake: remove directory properties, needs AIDASoft/podio#68

* 2020-02-17 Valentin Volkl ([PR#24](https://github.com/key4hep/edm4hep/pull/24))
  - Add an integration test using the TrickTrack seeding library

* 2020-02-14 vvolkl ([PR#26](https://github.com/key4hep/edm4hep/pull/26))
  - add  diagram showing association between types
  - improve whitespace in edm4hep.yaml
  - update Readme

* 2020-02-05 vvolkl ([PR#23](https://github.com/key4hep/edm4hep/pull/23))
  - Add plcio-like types and some associations needed for a first working version: TrackState, ObjectID, TrackerHit, TPCHit, Track, Vertex, ReconstructedParticle, MCRecoParticleAssociation, MCRecoTrackerAssociation, MCRecoCaloAssociation

* 2020-01-21 Valentin Volkl ([PR#22](https://github.com/key4hep/edm4hep/pull/22))
  - Add adapted plcio Calorimeter types  needed for a full working version

* 2020-01-21 Valentin Volkl ([PR#20](https://github.com/key4hep/edm4hep/pull/20))
  - Skip building podio tests in CI

* 2020-01-08 Valentin Volkl ([PR#21](https://github.com/key4hep/edm4hep/pull/21))
  - Replace tabs in test directory by spaces

* 2019-10-25 Valentin Volkl ([PR#17](https://github.com/key4hep/edm4hep/pull/17))
  - Fix edm4hep spelling convention:
         - EDM4hep in mixed case, EDM4HEP in the context of all caps and edm4hep in code

* 2019-10-15 Valentin Volkl ([PR#18](https://github.com/key4hep/edm4hep/pull/18))
  * Use an extended script for CI builds that compiles and install the HEAD of podio on the fly.

* 2019-10-02 Frank Gaede ([PR#16](https://github.com/key4hep/edm4hep/pull/16))
  - add some templates for pull requests and issues
  - add contributing guidelines

* 2019-10-02 Frank Gaede ([PR#14](https://github.com/key4hep/edm4hep/pull/14))
  - add tests write_events and read_events
        - implement code for MCParticle, SimTrackerHit and SimCalorimeterHit
  - add file for creating doxygen documentation
  - add missing c'tors and operators to Vector components
  - make use of HepMC and PDT optional

* 2019-09-27 Frank Gaede ([PR#7](https://github.com/key4hep/edm4hep/pull/7))
  - fix component names according discussion at CERN: [https://codimd.web.cern.ch/NtM0GlENRFG5Wc_bAn_sxA?both#](https://codimd.web.cern.ch/NtM0GlENRFG5Wc_bAn_sxA?both#)
  - add some extraCode for convenient methods to MCParticle 
        - to be discussed if we want these in this way 
  - fix the install directories to observe `CMAKE_INSTALL_PREFIX`
  - follow cmake updates in AIDASoft/podio#54


Frank Gaede 2019-10-02 
  - use same HepMC event in write/read_events
  - add missing c'tors for components

Frank Gaede 2019-10-01 
  -  add simple read and write example
  - add file for creating doxygen documentation
  - fix podio_INCLUDE_DIR in cmake
  - make use of HepMC and PDT optional

Frank Gaede 2019-09-27 
  - add SimTrackerHit and CalorimeterHit proposals

Valentin Volkl 2019-09-27 
  - Add particle relations to hepmc conversion example.

Valentin Volkl 2019-09-25 
  - Add cmake find scripts from Gaudi
  - Add first test converting HepMC to EDM4HEP

Valentin Volkl 2019-09-20 
  - [build infrastructure] pull initial setup from fcc-edm
  - [build infrastructure] Fix cmake variable name. (Temporary, as this has to be changed with the next podio version)
  - Add travis build status
  - follow recent podio cmake updates (#54)

Frank Gaede 2019-09-25 
  - fix install dirs (using CMAKE_INSTALL_PREFIX)
  - rename components;add some MCParticle methods

vvolkl 2019-09-23 
  - Fix CI and cmake (#2)

Graeme A Stewart 2019-09-20 
  - Trivial update
  - Merge pull request #1 from vvolkl/build_infrastructure
  - test
  - [build infrastructure] pull initial setup from fcc-edm

Graeme A Stewart 2019-09-19 
  - Initial commit
