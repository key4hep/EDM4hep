# Subtle differences between EDM4hep and LCIO

EDM4hep is largely based on [LCIO](https://github.com/iLCSoft/LCIO), the EDM for
linear collider studies. Given the fundamentally different implementation of
LCIO and EDM4hep there are some general conceptual differences, but by and large
many datatypes provide pretty much exactly the same functionality in both EDMs.
There are, however, a few cases where conceptual differences arise, see for
example [the handling of vertices](@ref md_doc_2_vertex_reco_particle_recos).
Additionally, there are a few cases where some more subtle differences occur.
These are described below.

## Handling of CellIDs

In LCIO the handling of `CellID`s is somewhat inconsistent. There are datatypes
that provide an `int getCellID`, but there are also other datatypes that provide
access to the low and high 32 bits separately via `getCellID0` and `getCellID1`.
EDM4hep consistently uses a `uint64_t` as cell ID, as DD4hep internally also
uses 64 bits.

Where applicable we convert from LCIO to EDM4hep in the following way
```cpp
template<typename LcioType>
uint64_t to64BitCellID(LcioType* obj) {
  const auto cellID0 = obj->getCellID0();
  const auto cellID1 = obj->getCellID1();
  uint64_t cellID = cellID1;
  cellID = (cellID << 32) | cellID0;
  return cellID;
}
```

### CellID in `TPCHit` and `RawTimeSeries`

The `TPCHit` in LCIO corresponds to the `RawTimeSeries` in EDM4hep. The `TPCHit`
only has a 32 bit CellID in LCIO, but uses the same `uint64_t` as all other
times for the `RawTimeSeries` in EDM4hep.

## Floating point differences in `ReconstructedParticle`

LCIO uses `double` precision for the following members, where EDM4hep resorts to
`float` only

- momentum
- mass
- energy

## Floating point storage differences in `MCParticle`

Both in EDM4hep and LCIO the `MCParticle` uses `double` to represent quantities
related to kinematics (e.g. momentum, energy, mass). However, in LCIO this is
only true for the in-memory representation, while on disk these values are
stored as `float`s.

## Signendess of time stamp

`LCEvent::getTimeStamp` returns a `long64` in LCIO, but
`edm4hep::EventHeader::getTimeStamp` returns a `uint64_t`. We keep this
difference deliberately since all timestamps are assumed to be absolute (after
some specified time point) and, consequently, are strictly positive.

## Type difference in number of contributions to `SimCalorimeterHit`

In LCIO `SimCalorimeterHit::getNMCContributions` returns an `int`, but in
EDM4hep contributions are added to the `SimCalorimeterHit` via
`OneToManyRelations`, hence `SimCalorimeterHit::getContributions()::size()`
returns a `size_t`.
