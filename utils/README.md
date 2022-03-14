# EDM4hep utilities

This folder contains utility functionality for EDM4hep.

## `kinematics.h`

The `edm4hep/utils/kinematics.h` header contains some functionality for getting common kinematics information from EDM4hep data types. All of the functions live in the `edm4hep::utils` namespace.

### `edm4hep::utils::p4`
The `p4` function can be used to get the 4 momentum vector from a given particle (or datatype that offers at least a `getMomentum` function).
Since EDM4hep does not guarantee any internal consistency of the 4 momentum state it is possible to use either the mass or the energy of a particle to get the 4 momentum. Additionally it is also possible pass in user defined values for the mass or the energy.

For some example usages see the [unit tests](/test/utils/test_kinematics.cpp)
