# PIDHandler introduction and usage

This page contains some bigger picture introduction for the utilities that are
available to work with `ParticleID`s and its related metadata. It also contains
examples for the most common usage patterns.

## PIDHandler basics
The `PIDHandler` can be use to work with `ParticleIDCollection`s. The main
features are
- the retrieval meta information for a `ParticleIDCollection`, making it
  possible to e.g. retrieve the indices of parameters from parameter names
- the possibility to invert the relation to `ReconstructedParticle`s, which
  allows one to, e.g. get all `ParticleID`s that point to a specific
  `ReconstructedParticle`.

### When (not) to use the PIDHandler
NOTE: Depending on how you use the `PIDHandler` it might incur an unnecessary
performance overhead. The main purpose is to use it in cases, where the relation
between `ParticleID`s and `ReconstructedParticle`s is not trivial, e.g.
- When the two collections do **not** run in parallel
- When not all `ReconstructedParticle`s have a `ParticleID` attached
- When you want to have access to several `ParticleID`s for a given
  `ReonstructedParticle` and there is no trivial relation between them.

In case your collections run in parallel it will be much quicker to just loop
over them in parallel.

In case you just want look at one `ParticleID` collection, but need to look at
some properties of the `ReconstructedParticle`, simply use the existing relation
to do that.


## ParticleIDMeta basics
`ParticleIDMeta` is a simple struct that bundles all ParticleID meta information
for one collection together. Whenever metadata is involved for ParticleIDs this
will be the thing to use.

## General considerations

There is no strictly enforced coupling between the meta information that is set
for a collection and the contents of that collection. In order for everything to
work as expected the following assumptions have to be met
- The collection name that is passed to the `setAlgoInfo` methods to set meta
  information has to match the collection name used for putting collections into
  events.
- All elements of a `ParticleIDCollection` have the same value set for
  `algorithmType`. Additionally, it is usually assumed that this is a unique
  value.
- The `ParticleIDMeta::paramNames` is assumed to match the parameters as they
  are set in the `ParticleID` elements.

Additionally there are a few usage considerations to be kept in mind
- The `PIDHandler` can be used without any meta data about the contained
  `ParticleID`s. However, in this case it will only be useful for inverting
  relations and for getting `ParticleID`s that point to a given
  `ReconstructedParticle`.
- There is no guarantee that the meta information that is set in a `PIDHandler`
  is consistent with the elements it knows about. Some constructors attempt to
  construct a consistent handler, but it is always possible to set meta
  information via `setMetaInfo`, which might be completely unrelated to the
  actual contents of a handler.

Given that there is no enforced consistency the interface and the utility
functionality makes heavy use of
[`std::optional`](https://en.cppreference.com/w/cpp/utility/optional) in its
return values / types. This makes it possible to check whether the desired
information was actually present, without having to reserve some special values
to indicate that fact.

## Usage examples

**The following examples assume the following includes** (which will not be visible in the examples)

```cpp
#include <edm4hep/utils/ParticleIDUtils.h>
#include <edm4hep/ParticleIDCollection.h>

#include <podio/Frame.h>
```

**The following examples additionally assume, that there is a `metadata` and an
`event` Frame present.** The `metadata` Frame can be obtained like this from
standard EDM4hep files.

```cpp
#include <podio/ROOTReader.h>
#include <podio/Frame.h>

podio::Frame getMetadataFrame(const std::string& filename) {
  podio::ROOTReader reader{};
  reader.openFile(filename);
  return podio::Frame(reader.readNextEntry(podio::Category::Metadata));
}
```

Alternatively it can be created as an empty Frame and then just written using
the `podio::Category::Metadata` category name.

**Finally most of the examples assume that the desired values were found and
simply get the `value` from the returned `std::optional` directly.** This means
that most of the times you will see lines like this

```cpp
const auto value = pidHandler.getAlgoType("someAlgo").value();
```

This will throw an exception if some information is not available. Check if the
optional has a value when actually using these utilities.

### Creating a PIDHandler for a complete event

If you want to work with a `PIDHandler` that has a somewhat global view of all
`ParticleID`s that are present you can simply construct one from an event Frame

```cpp
const auto pidHandler = edm4hep::utils::PIDHandler::from(event);
```

You can also construct a `pidHandler` that populates some meta information
internally by also passing in the `metadata` Frame to this constructor
```cpp
const auto pidHandler = edm4hep::utils::PIDHandler::from(event, metadata);
```

### Creating a PIDHandler from an arbitrary number of collections

If you simply want to use a `PIDHandler` to invert the relations for a given set
of `ParticleIDCollection`s you can do that via

```cpp
const auto& tofPID = event.get<edm4hep::ParticleIDCollection>("ToFPID");
const auto& dNdXPID = event.get<edm4hep::ParticleIDCollection>("dNdXPID");
const auto& mvaPID = event.get<edm4hep::ParticleIDCollection>("mvaPID");

const auto pidHandler = edm4hep::utils::PIDHandler::from(tofPID, dNdXPID, mvaPID);
```

**This handler will not have any access to metadata!** If you need metadata as
well, you can add that after the fact using the `addMetaInfo` method.

### Using a PIDHandler to get the ParticleIDs for a reco particle

If you have constructed a `PIDHandler` you can use it to get all or specific
related `ParticleID` objects of a `ReconstructedParticle`

```cpp
const auto pidHandler = /* constructed as above somehow */;

const auto recos = event.get<edm4hep::ReconstructedParticle>("recos");
const auto reco = recos[0];

// Get all related ParticleIDs
const auto pids = pidHandler.getPIDs(reco);

// Only get a specific ParticleID
const auto tofAlgoType = pidHandler.getAlgoType("ToFPID").value();
const auto tofPID = pidHandler.getPID(reco, tofAlgoType).value();
```

### Retrieving meta information for a collection

If you simply want to get the meta information for a collection (called "ToFPID"
in this example) do

```cpp
const auto algoInfo = edm4hep::utils::PIDHandler::getAlgoInfo(metadata, "ToFPID").value();
```


### Using meta information of a collection

If you have retrieved the meta information you can directly use that to learn
something about the `ParticleID`s it describes

```cpp
const auto& tofPIDs = event.get<edm4hep::ParticleIDCollection>("ToFPID");
const auto algoInfo = edm4hep::utils::PIDHandler::getAlgoInfo(metadata, "ToFPID").value();
const auto timeIndex = edm4hep::utils::getParamIndex(algoInfo, "time").value();

for (const auto tof : tofPIDs) {
  const auto time = tof.getParameters(timeIndex);
  // ...
}
```

If you have a `PIDHandler` with enough meta information set it is also possible
to retrieve the `timeIndex` from that

```cpp
const auto pidHandler = /* construct somehow see above */
const auto tofAlgoType = pidHandler.getAlgoType("ToFPID").value();
const auto timeIndex = pidHandler.getParamIndex("time").value();

// ... as above
```


### Setting meta information for a collection

If you have a `ParticleIDCollection` and want to persist some meta information
for it the following snippet shows you how to do that

```cpp
// Create ParticleID meta information for an algorithm
const auto tofMeta = edm4hep::utils::ParticleIDMeta{"TimeOfFlight", 42, {"time", "time_error"}};

auto tofPIDs = edm4hep::ParticleIDCollection{};
// ... fill collection

edm4hep::utils::PIDHandler::setAlgoInfo(metadata, tofPIDs, "ToFPID", tofMeta);
event.put(std::move(tofPIDs), "ToFPID");
```

This will put the necessary metadata into the `metadata` Frame and also take
care of setting the `algorithmType` field of all elements of the `tofPIDs`
collection to `42` (the value it is set to in the `tofMeta` object).

**Things to note:**
- It is important to use the same names for putting the collection into the
  `event` Frame and the one that is passed to `PIDHandler::setAlgoInfo`!

### Setting meta information using a collection name

If you want to set the meta information for a `ParticleIDCollection` that you no
longer have mutable access to, this can be done via

```cpp
// Create ParticleID meta information for an algorithm
const auto tofMeta = edm4hep::utils::ParticleIDMeta{"TimeOfFlight", 42, {"time", "time_error"}};

edm4hep::utils::PIDHandler::setAlgoInfo(metadata, "ToFPID", tofMeta);
```

**Things to note:**
- In this case it is user responsibility to set the `algoType` of the
  `ParticleIDMeta` to the same value as the one that is used for the elements of
  the collection.
- It is important to use the same names for putting the collection into the
  `event` Frame and the one that is passed to `PIDHandler::setAlgoInfo`!
