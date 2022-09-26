
[![key4hep](https://github.com/key4hep/EDM4hep/workflows/key4hep_linux/badge.svg)](https://github.com/key4hep/EDM4hep/actions/workflows/key4hep_linux.yml)
[![linux](https://github.com/key4hep/EDM4hep/actions/workflows/lcg_linux_with_podio.yml/badge.svg)](https://github.com/key4hep/EDM4hep/actions/workflows/lcg_linux_with_podio.yml)
# EDM4hep


A generic event data model for future HEP collider experiments.

![](doc/edm4hep_diagram.svg)

**Componets**

| | | |
|-|-|-|
| [Vector3f ](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L9) | [Vector3d ](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L24)   | [Vector2i](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L40)  |
| [Vector2f](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L54) | [TrackState ](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L68) | [ObjectID](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L91)  |


**Datatypes**

| | | |
|-|-|-|
| [EventHeader](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L113)         | [MCParticle](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L123)        | [SimTrackerHit](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L191)         |
| [CaloHitContribution](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L225) | [SimCalorimeterHit](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L237) | [RawCalorimeterHit](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L249)     |
| [CalorimeterHit](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L258)      | [ParticleID](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L270)        | [Cluster](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L283)               |
| [TrackerHit](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L304)          | [TrackerHitPlane](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L321)   | [TPCHit](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L342)                |
| [Track](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L354)               | [Vertex](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L373)            | [ReconstructedParticle](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L390) |

**Associations**

| | | |
|-|-|-|
| [MCRecoParticleAssociation](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L421)        | [MCRecoCaloAssociation](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L430)         | [MCRecoTrackerAssociation](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L439)         |
| [MCRecoTrackerHitPlaneAssociation](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L448) | [MCRecoCaloParticleAssociation](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L457) | [MCRecoClusterParticleAssociation](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L466) |
| [MCRecoTrackParticleAssociation](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L475)   | [RecoParticleVertexAssociation](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L484) |                                                                                                      |

The tests and examples in the `tests` directory show how to read, write, and use these types in your code.


## Status

This project is in a beta stage -- feedback and use of it in production is encouraged, but expect changes until a stable version is released in some weeks.

## Dependencies

Required:

* [PODIO](https://github.com/AIDASoft/podio) >= v00-09-02
* [ROOT](https://github.com/root-project/root) >= v6.18/04

Optional:

* Doxygen (to build code reference)
* HepMC (integration test)
* HepPDT (integration test)

## Build and Install

This project follows the key4hep guidelines and can be build with CMake:

```sh
source /cvmfs/sw-nightlies.hsf.org/key4hep/setup.sh
git clone https://github.com/key4hep/EDM4hep
cd EDM4hep; mkdir build; cd build
cmake ..
make
make test
```

The library files and dictionaries (`libedm4hep.so`, ...) must be put somewhere in `LD_LIBRARY_PATH`.






## Contributing

Contributions and bug reports are welcome!
See our [contributing guidelines](./doc/contributing.md) if you want to contribute code to EDM4hep.
