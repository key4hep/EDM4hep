
[![key4hep](https://github.com/key4hep/EDM4hep/workflows/key4hep_linux/badge.svg)](https://github.com/key4hep/EDM4hep/actions/workflows/key4hep_linux.yml)
[![linux](https://github.com/key4hep/EDM4hep/actions/workflows/lcg_linux_with_podio.yml/badge.svg)](https://github.com/key4hep/EDM4hep/actions/workflows/lcg_linux_with_podio.yml)
# EDM4hep


A generic event data model for future HEP collider experiments.

![](doc/edm4hep_diagram.svg)



|                                                                                           |                                                                                       |                                                                                          |                                                                                       |
| -------------                                                                             | -------------                                                                         | -----                                                                                    | ---                                                                                   |
| [Vector3f ](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L9)                   | [Vector3d ](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L23)              | [Vector2i ](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L38)                 | [TrackState ](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L50)            |
| [SimCalorimeterHit](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L201)         | [RawCalorimeterHit](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L212)     | [ CalorimeterHit ](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L222)         | [Cluster](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L247)               |
| [MCParticle](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L84)                 | [Vertex](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L318)                | [ParticleID ](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L234)              | [ReconstructedParticle](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L335) |
| [SimTrackerHit](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L156)             | [TPCHit](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L288)                | [TrackerHit](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L270)               | [Track](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L300)                 |
| [MCRecoParticleAssociation](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L362) | [MCRecoCaloAssociation](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L370) | [MCRecoTrackerAssociation](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L370) | [ObjectID](https://github.com/key4hep/EDM4hep/blob/master/edm4hep.yaml#L71)               |


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
* TrickTrack (integration test)

## Build and Install

This project follows the key4hep guidelines and can be build with CMake:

```
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
