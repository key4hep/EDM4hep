# EDM4hep utilities

This folder contains utility functionality for EDM4hep.

## `Helix.h`

`edm4hep::utils::Helix` describes a trajectory in a constant magnetic field along
z. Link against `EDM4HEP::utils`. It uses `Vector3d` and double precision, with mm,
GeV, tesla, and charge in units of the positron charge.

```cpp
#include <edm4hep/utils/Helix.h>

const edm4hep::utils::Helix helix({0., 0., 0.}, {1., 0., 0.5}, -1., 2.);
const auto position = helix.positionAt(100.); // signed transverse path length, mm
const auto momentum = helix.getExtrapolatedMomentum(position);
const auto residual = helix.getDistanceToPoint({10., 20., 30.});
if (const auto path = helix.pathToRadius(2150.)) { // first crossing of a barrel at r = 2150 mm
  const auto atBarrel = helix.positionAt(*path);
}
```

`pathToZ(z)`, `pathToRadius(r)` and `pathToPlane(point, normal)` return the
smallest non-negative transverse path length at which the trajectory reaches the
plane at `z`, the cylinder of radius `r` around the z axis, or a plane parallel
to z (e.g. a face of a polygonal barrel), or an empty `std::optional` if it never
does. Together with `positionAt` they replace pandora's `Helix::GetPointInZ`,
`Helix::GetPointOnCircle` and `Helix::GetPointInXY` for projecting tracks to the
calorimeter faces. Path
lengths of different crossings can be compared directly to find the first one.

The Cartesian constructor replaces MarlinUtil's `Initialize_VP`.
`Helix::fromCanonical(phi0, d0, z0, omega, tanLambda, Bz)` replaces
`Initialize_Canonical` for unit absolute charge. Both accept an optional reference
point for the impact parameters, defaulting to the origin. `getD0`, `getZ0`,
`getOmega`, `getTanLambda`, and `getCharge` provide the corresponding parameters.
Canonical `phi0` is the momentum azimuth at the transverse closest approach to
the reference point, and `fromCanonical` requires `omega * d0 <= 1` so that the
parameters describe that closest approach. An excess within float rounding, as
can come from a `TrackState`, is moved onto `omega * d0 = 1`.

`Helix::fromTrackState(trackState, Bz)` reads `phi` as the direction at the
transverse closest approach to `referencePoint`, with `D0`/`Z0` relative to that
point. This is the direction at the reference point when it lies on the track.
`toTrackState(location, referencePoint)` writes a state in the same way, e.g.
with a hit position as reference (giving `D0 = Z0 = 0` for a hit on the track) or
the origin for `AtIP`.

The class supports negative fields, non-unit charges, and straight tracks
(zero charge or field). Zero transverse momentum and non-finite inputs throw
`std::invalid_argument`. A canonical straight track cannot determine its momentum;
use the Cartesian constructor instead.

For curling tracks, the impact parameters use the transverse closest approach
within half a turn of the starting position. In particular, Z0 is local to that
turn, whereas MarlinUtil chooses the PCA turn closest to z = 0. Point residuals
use the transverse projection and then choose the turn closest in z to the
query point. Their `threeDimensional` value is the quadrature sum of transverse
and longitudinal residuals, as in MarlinUtil; it is not the global minimum 3D
distance to the helix. Their `path` is the signed transverse path length from
`getPosition()` to that projection (negative behind the start); multiply by
`sqrt(1 + tanLambda^2)` for the 3D arc length. `getExtrapolatedMomentum`
returns the tangent at the transverse projection, independent of the query
point's z coordinate.

## `kinematics.h`

The `edm4hep/utils/kinematics.h` header contains some functionality for getting common kinematics information from EDM4hep data types. All of the functions live in the `edm4hep::utils` namespace.

### `edm4hep::utils::p4`
The `p4` function can be used to get the 4 momentum vector from a given particle (or datatype that offers at least a `getMomentum` function).
Since EDM4hep does not guarantee any internal consistency of the 4 momentum state it is possible to use either the mass or the energy of a particle to get the 4 momentum. Additionally it is also possible pass in user defined values for the mass or the energy.

For some example usages see the [unit tests](/test/utils/test_kinematics.cpp)

## `graph-conf.yaml`

This is a configuration file to be used with the podio visualization tool for
clustering data types into categories and a better looking graph.
