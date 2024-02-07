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
