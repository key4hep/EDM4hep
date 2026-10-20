# SimCalorimeterHit and CaloHitContribution

The `edm4hep::SimCalorimeterHit` is the container for `edm4hep::CaloHitContribution`s
with some summary information computed from these contributions. The content of
the hit level information depends on how the hit and its contributions were
populated. Consult the corresponding documentation of the algorithm or program
that produced them.

Since there is no generally applicable way of computing hit level quantities
from the contained contributions this is not done by EDM4hep. Typically one uses
the information from the contributions for applications where detailed
simulation information is necessary, e.g. for digitization. In particular, no
time selection is applied to the contributions, and hit time-related effects
have to be handled by the digitization.

## Adding contributions to a hit

`MutableSimCalorimeterHit::addToContributions` only attaches a contribution to a
hit, **it leaves all hit level quantities untouched**. If the energy of the hit
should be kept consistent with the energy of its contributions, use
`MutableSimCalorimeterHit::addMCParticleContribution` instead, which attaches
the contribution and adds its energy to the energy of the hit

```cpp
#include "edm4hep/CaloHitContributionCollection.h"
#include "edm4hep/SimCalorimeterHitCollection.h"

auto hits = edm4hep::SimCalorimeterHitCollection();
auto contribs = edm4hep::CaloHitContributionCollection();

auto hit = hits.create();
auto contrib = contribs.create();
contrib.setEnergy(1.5f);

hit.addMCParticleContribution(contrib); // hit.getEnergy() is now 1.5
```
