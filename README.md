
[![Key4hep build](https://github.com/key4hep/EDM4hep/actions/workflows/key4hep-build.yaml/badge.svg)](https://github.com/key4hep/EDM4hep/actions/workflows/key4hep-build.yaml)
[![linux](https://github.com/key4hep/EDM4hep/actions/workflows/lcg_linux_with_podio.yml/badge.svg)](https://github.com/key4hep/EDM4hep/actions/workflows/lcg_linux_with_podio.yml)
[![DOI](https://zenodo.org/badge/209480664.svg)](https://zenodo.org/doi/10.5281/zenodo.4785062)

# EDM4hep

A generic event data model for future HEP collider experiments.

![](doc/edm4hep_diagram.svg)

**Components**

| | | |
|-|-|-|
| [Vector4f](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L9)      | [Vector3f](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L34)      | [Vector3d](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L57)     |
| [Vector2f](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L82)     | [TrackState](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L180)   | [Quantity](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L209)    |
| [CovMatrix2f](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L101) | [CovMatrix3f](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L121)  | [CovMatrix4f](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L140) |
| [CovMatrix6f](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L160) |   |   |


**Datatypes**

| | | |
|-|-|-|
| [EventHeader](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L218)         | [MCParticle](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L230)        | [SimTrackerHit](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L296)         |
| [CaloHitContribution](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L336) | [SimCalorimeterHit](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L348) | [RawCalorimeterHit](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L359)     |
| [CalorimeterHit](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L368)      | [ParticleID](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L379)        | [Cluster](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L393)               |
| [TrackerHit3D](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L425)          | [TrackerHitPlane](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L450)   | [RawTimeSeries](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L478)                |
| [Track](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L492)               | [Vertex](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L509)            | [ReconstructedParticle](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L555) |
| [TimeSeries](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L593) | [RecDqdx](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L605) |                                                                                          |

**Links**

| | | |
|-|-|-|
| [RecoMCParticleLink](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L660)        | [CaloHitSimCaloHitLink](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L684)         | [TrackerHitSimTrackerHitLink](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L690)         |
| [CaloHitMCParticleLink](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L666) | [ClusterMCParticleLink](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L672) | [TrackMCParticleLink](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L678)   |
| [VertexRecoParticleLink](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L696) | | |

**Generator related (meta-)data**

| | | |
|-|-|-|
| [GeneratorEventParameters](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L617) | | |
| [GeneratorPdfInfo](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L633) | | |

**Interfaces**

| | | |
|-|-|-|
| [TrackerHit](https://github.com/key4hep/EDM4hep/blob/main/edm4hep.yaml#L644) | | |

The tests and examples in the `tests` directory show how to read, write, and use these types in your code.


## Status

This project is in a beta stage -- feedback and use of it in production is encouraged, but expect changes until a stable version is released.

## Dependencies

Required:

* [PODIO](https://github.com/AIDASoft/podio) >= v01-00
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

## Example files

Example EDM4hep files can be obtained from the Continuous Integration (CI)
workflows. From the EDM4hep github page, go to Actions -> Key4hep build, click
one of the runs (the latest scheduled is preferred) and they will appear at the
bottom, under Artifacts.

## Contributing

Contributions and bug reports are welcome! See our [contributing guidelines](doc/contributing.md) if you want to contribute code to EDM4hep.

## Citing EDM4hep

If you wish to cite EDM4hep please reference this paper:

```bibtex
@article{Gaede:2022leb,
    author = "Gaede, Frank and Madlener, Thomas and Declara Fernandez, Placido and Ganis, Gerardo and Hegner, Benedikt and Helsens, Clement and Sailer, Andre and A. Stewart, Graeme and Völkl, Valentin",
    title = "{EDM4hep - a common event data model for HEP experiments}",
    doi = "10.22323/1.414.1237",
    journal = "PoS",
    volume = "ICHEP2022",
    pages = "1237",
    month = "11",
    year = "2022"
}
```
