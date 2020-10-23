# Delphes to EDM4hep output converter

The `DelphesEDM4HepConverter` creates an `edm4hep` output file from a `delphes`
process.

## Configuration

The `DelphesEDM4HepConverter` takes an
[`OutputSetting`](DelphesEDM4HepConverter.h#L25-L98) parameter to configure which
`delphes` branches go into which `edm4hep` output collections. In the current
implementation of the conversions this is filled from an additional configuration file
that defines the necessary parameters:

[edm4hep_output_config.tcl](edm4hep_output_config.tcl). This `tcl` file has to
be passed to the conversion executable as parameter from the command line. **No
changes to the delphes card that is used are necessary.** An exemplary call
could look something like this (replacing the `delphes_card.tcl` and the
`input_file.stdhep` with actual input files):

``` bash
DelphesSTDHEP_EDM4HEP delphes_card.tcl \
                      edm4hep_output_config.tcl \
                      edm4hep_output.root \
                      input_file.stdhep
```

A module `EDM4HepOutput` is introduced solely for the purpose of being able to
easily reuse the `tcl` parser from delphes and to introduce a logical
containment for the parameters. The names of the delphes branches are taken from the `TreeWriter`
module defintion and used in the `EDM4HepOutput`.

### Collection conversions

The configuration mainly steers which Delphes collections are still available in
the `edm4hep` output. A description of which Delphes output classes correspond
to which `edm4hep` classes can be found [below](#class-conversions). The
collections specified in the `EDM4HepOutput` module are the `BranchName`s taken
from the `TreeWriter` definition. Each of the following configuration parameters
takes a list of collection names that will then also be available in the output.

#### `ReconstructedParticleCollections`
All `Track` and `Tower` input collections that will be stored in one global
`edm4hep::ReconstructedParticleCollection`. These will be associated to the
generated particles stored as `edm4hep::MCParticle`s. The converter works under
the assumption that this is a non-overlapping list of particles. It is the users
responsibility to make sure that this is the case. (See [known
issues](#known-issues)).
  
#### `GenParticleCollection`
All Delphes `GenParticle` collections that will be considered and stored as
`edm4hep::MCParticle`. Each Delphes collection will be put into its own
`edm4hep` output collection under the same name, but all generated particles
will be considered for the associations to the `ReconstructedParticle`s in the
global collection (See [above](#reconstructedparticlecollections)). Usually it
is enough to use the `Particle` branch here since that contains all generated
particles from Delphes.

#### `JetCollections`
All Delphes `Jet` collections that will be converted and stored. Each delphes
collection gets its own `edm4hep` output collection under the same name. The jet
constituents are taken from the global `ReconstructedParticle` collection (See [above](#reconstructedparticlecollections)).

#### `MuonCollections`
All Delphes `Muon` collections that will be converted and stored. Each delphes
collection gets its own `edm4hep` output collection under the same name. Only
particles from the global `ReconstructedParticle` collection are considered.

#### `ElectronCollections`
All Delphes `Electron` collections that will be converted and stored. Each
delphes collection gets its own `edm4hep` output collection under the same name.
Only particles from the global `ReconstructedParticle` collection are
considered.

#### `PhotonCollections`
All Delphes `Photon` collections that will be converted and stored. Each delphes
collection gets its own `edm4hep` output collection under the same name. Only
particles from the global `ReconstructedParticle` collection are considered.

#### `MissingETCollections`
All Delphes `MissingET` collections that will be converted and stored. Each
delphes collection gets its own `edm4hep` output collection under the same name.
The filled collection contains only one element per event.

#### `ScalarHTCollections`
All Delphes `ScalarHT` collections that will be converted and stored. Each
delphes collection gets its own `edm4hep` output collection under the same name.
The filled collection contains only one element per event.


The parameters **`RecoParticleCollectionName`** and
**`MCRecoAssociationCollectionName`** control the names of the [global
reconstructed particle colleciton](#reconstructedparticlecollections) and the
collection with the `MCRecoParticleAssociation`s that can be used to find the
`MCParticle`s associated to `ReconstructedParticle`s (and vice versa).

### Class conversions

The following table lists which Delphes classes correspond to which `edm4hep`
classes. For the conversion the Delphes classes are taken from the `TreeWriter`
(the `BranchClass` for each `Branch` defined in there)

| Delphes       | `edm4hep`                                         |
|---------------|---------------------------------------------------|
| `GenParticle` | `MCParticle`                                      |
| `Track`       | `ReconstructedParticle` with associated `Track`   |
| `Tower`       | `ReconstructedParticle` with associated `Cluster` |
| `Jet`         | `ReconstructedParticle`                           |
| `Muon`        | `RecoParticleRef`                                 |
| `Electron`    | `RecoParticleRef`                                 |
| `Photon`      | `RecoParticleRef`                                 |
| `MissingET`   | `ReconstructedParticle`                           |
| `ScalarHT`    | `ParticleID`                                      |
|  n/a          | `MCRecoParticleAssociation`                       |

All Delphes classes that are not listed here are currently not converted.


### Known issues

- [ ] Double counting of Tracks and Clusters. In Delphes it is possible that a
      `Tower` and a `Track` point back to the same generated particle. This is
      currently not checked and each `Track` and `Tower` will be converted into
      an `edm4hep::ReconstructedParticle`. Hence, it is possible to get more
      than one per generated particle. This means that the number of jet
      constituents will be differend between Jets in the Delphes output and in
      the `edm4hep` output.

- [ ] Not all available information is used in the conversion. An incomplet list
      of things that are currently not available in `edm4hep`:
  - [ ] Jet substructure variables (including subjets)
  - [ ] Isolation variables
  - [ ] Flavor tag information
  - [ ] Tau tag information
