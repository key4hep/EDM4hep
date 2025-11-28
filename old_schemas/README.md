# Old EDM4hep schemas

This folder contains old EDM4hep schemas for which backwards compatibility is
guaranteed and tested. **Note that these schemas are not carbon copies of the
ones that were released with tagged EDM4hep releases.** Rather, we have removed
a few things which we either handle manually or for which we have decided to
only have backwards compatibility guarantees at a later point. (The removals are
mostly necessary to get the schemas past the podio code generation and it's
internal validation that otherwise prevents us from introducing breaking changes).

The things that have been removed are:
- The old `Link` datatypes that were present before `podio::LinkCollection` was
  introduced (see [#373](https://github.com/key4hep/EDM4hep/pull/373) and the
  other PRs related in there). The necessary schema evolution is done manually
  (the functionality is introduced in the same PR).
- The `GeneratorEventParameters` as well as the `GeneratorPdfInfo` is removed
  from `v2` and `v3` since these were overhauled considerably in
  [#446](https://github.com/key4hep/EDM4hep/pull/446). The decision at the time
  was that since these have been virtually unused, to go with a breaking change,
  given the overall amount of changes that happened.
