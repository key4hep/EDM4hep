
[![key4hep](https://github.com/key4hep/EDM4hep/workflows/key4hep_linux/badge.svg)](https://github.com/key4hep/EDM4hep/actions/workflows/key4hep_linux.yml)
[![linux](https://github.com/key4hep/EDM4hep/actions/workflows/lcg_linux_with_podio.yml/badge.svg)](https://github.com/key4hep/EDM4hep/actions/workflows/lcg_linux_with_podio.yml)
# EDM4hep


A generic event data model for future HEP collider experiments.

![](doc/edm4hep_diagram.svg)

**Components**

| | | |
|-|-|-|
| [Vector4f](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L9)      | [Vector3f](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L34)     | [Vector3d](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L56)      |
| [Vector2i](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L84)     | [Vector2f](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L104)    | [TrackState](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L192)   |
| [Quantity](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L221)    | [Hypothesis](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L229)  | [HitLevelData](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L236) |
| [CovMatrix2f](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L123)  |[CovMatrix3f](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L140)   |[CovMatrix4f](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L157)   |
| [CovMatrix6f](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L174) | | |


**Datatypes**

| | | |
|-|-|-|
| [EventHeader](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L246)         | [MCParticle](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L258)        | [SimTrackerHit](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L326)         |
| [CaloHitContribution](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L368) | [SimCalorimeterHit](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L380) | [RawCalorimeterHit](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L392)     |
| [CalorimeterHit](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L401)      | [ParticleID](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L413)        | [Cluster](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L428)               |
| [TrackerHit3D](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L461)          | [TrackerHitPlane](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L487)   | [RawTimeSeries](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L517)                |
| [Track](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L530)               | [Vertex](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L549)            | [ReconstructedParticle](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L577) |
| [SimPrimaryIonizationCluster](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L689) | [TrackerPulse](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L723) | [RecIonizationCluster](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L748) |
| [TimeSeries](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L759) | [RecDqdx](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L771) |                                                                                          |

**Associations**

| | | |
|-|-|-|
| [MCRecoParticleAssociation](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L615)        | [MCRecoCaloAssociation](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L624)         | [MCRecoTrackerAssociation](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L633)         |
| [MCRecoTrackerHitPlaneAssociation](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L642) | [MCRecoCaloParticleAssociation](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L651) | [MCRecoClusterParticleAssociation](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L660) |
| [MCRecoTrackParticleAssociation](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L669)   | [RecoParticleVertexAssociation](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L678) |                                                                                                      |

**Interfaces**

| | | |
|-|-|-|
| [TrackerHit](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L785) | | |


The tests and examples in the `tests` directory show how to read, write, and use these types in your code.


## Status

This project is in a beta stage -- feedback and use of it in production is encouraged, but expect changes until a stable version is released.

## Dependencies

Required:

* [PODIO](https://github.com/AIDASoft/podio) >= v00-15
* [ROOT](https://github.com/root-project/root) >= v6.22/06

Optional:

* Doxygen (to build code reference)
* HepMC (integration test)
* HepPDT (integration test)

## Build and Install

This project follows the key4hep guidelines and can be built with CMake:

```sh
source /cvmfs/sw-nightlies.hsf.org/key4hep/setup.sh
git clone https://github.com/key4hep/EDM4hep
cd EDM4hep; mkdir build; cd build
cmake ..
make
make test
```

The library files and dictionaries (`libedm4hep.so`, ...) must be put somewhere in `LD_LIBRARY_PATH`.

## Python bindings
There are python bindings for all the classes in the datamodel for working with
the classes individually (for working with collections podio has its own
bindings). Make sure that `<CMAKE_INSTALL_PREFIX>/lib` is in `LD_LIBRARY_PATH`,
`<CMAKE_PREFIX_PATH>/python` is in `PYTHONPATH` and `<CMAKE_INSTALL_PREFIX>/include` is in `ROOT_INCLUDE_PATH`:
```python
import edm4hep
particle = edm4hep.MCParticle() # default initialized particle
particle.getCharge() # 0.0

series = edm4hep.TimeSeries(1, 2, 3) # classes can be constructed with non-default parameters
series.getCellID() # 1
series.getTime() # 2.0
series.getInterval() # 3.0

mc = edm4hep.MutableMCParticle() # mutable classes can also be modified
mc.setPDG(2212)
mc.getPDG() # 2212
```

In an interactive session (with `python` or `ipython`, for example) it's
possible to see all the classes by typing `edm4hep.` and then pressing TAB.

## Dumping to JSON
The `edm4hep2json` tool can be used to dump the contents of an EMD4hep file in
JSON format. E.g.

```bash
edm4hep2json some_events.edm4hep.root
```

This will produce an output file `some_events.edm4hep.json`. That has the following structure
```json
{
  "Event <N>": {
     "<CollectionName>": {
       "collID": <ID>,
       "collType": <collectionType>,
       "collection": [
         <elements>
       ]
    },
  }
}
```

As also stated in the [podio
documentation](https://github.com/AIDASoft/podio/blob/master/doc/advanced_topics.md#dumping-json)
there is **no plan to support reading JSON back in once it is dumped, so this
should not be seen as a way to persistify data**, but rather to interface it
with tools that consume JSON.

Use `edm4hep2json --help` to see the available options for selecting which parts
of the input file should be part of the output.

## Contributing

Contributions and bug reports are welcome! See our [contributing guidelines](doc/contributing.md) if you want to contribute code to EDM4hep.
