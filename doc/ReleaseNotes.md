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
