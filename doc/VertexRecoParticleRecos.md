# The relations and associations between `Vertex` and `ReconstructedParticle`

The `Vertex` and the `ReconstructedParticle` have relations that can in
principle form a loop:
- The `Vertex` has a `OneToManyRelation` to `particles`
- The `ReconstructedParticle` has a `OneToOneRelation` to a `startVertex`
- The `RecoParticleVertexAssociation` is an association that links both

Since this has the potential for some confusion this document gives a brief
overview of the main design principles for EDM4hep and also shows the intended
way of using all of these. Additionally, this is one of the places where EDM4hep
is conceptually different than LCIO. Hence, we will also show how the two
concepts map to each other.

## Design principles

The major design principle was *make it possible to create each collection in
separate steps **without having to mutate collections once they have been
created***. The latter part is the most important one, as it is imposed by
EDM4hep's mutability concept. As a consequence, filling the relation information
should follow these main guidelines
- the decay particles that form a `Vertex` should be added to the `particles`.
  Hence, a `Vertex` can always be queried for those
- a `ReconstructedParticle` that decayed at a given `Vertex` should point to
  this `Vertex` via the `decayVertex`. It is then always possible to get the
  decay produces via `getDecayVertex().getParticles()`.
- in order to allow navigation from a decay particle to the vertex it originated
  from a `RecoParticleVertexAssociation` should be created. If no such
  navigation is necessary, creating these associations can also be omitted.
- in order to allow for an easy navigation from the `Vertex` to the high level
  `ReconstructedParticle` a `RecoParticleVertexAssociation` should be created if
  necessary.

## Using `Vertex` and (high level) `ReconstructeParticle` objects

The following examples show a few use cases and how to achieve them. We assume
that the following variables are defined externally somehow
- `particle` is a `edm4hep::ReconstructeParticle`. This denotes a decaying particle
- `startVtxAssocs` is a `edm4hep::RecoParticleVertexAssociationCollection`
- `reco` is a `edm4hep::ReconstructedParticle`. This denotes a decay particle,
  i.e. it was used as an input to vertexing.

A few concrete examples are shown now, a few more can be found
[below](#in-lcio-i-do-this-how-do-i-do-it-in-edm4hep) where a few more details
about the differences between LCIO and EDM4hep can be found.

### Getting all decay particles from a `ReconstructedParticle`

```cpp
const auto vtx = particle.getDecayVertex()
const auto decayParticles = vtx.getParticles();
```

### Getting the start vertex for a decay particle

This is a small helper function that encapsulates the main functionality

```cpp
std::optional<edm4hep::Vertex> getStartVertex(edm4hep::ReconstructedParticle p, 
                                              const edm4hep::RecoParticleVertexAssociationCollection& assocs) {
  for (const auto assoc : assocs) {
    if (assoc.getRec() == rec) {
      return assoc.getVertex();
    }
  }
  return std::nullopt;
}
```

## Creating `Vertex` and (high level) `ReconstructedParticle` objects

In the following examples we will assume, that we have a list of
`ReconstructedParticle`s that we want to use in vertexing. We will produce the
following outputs
- a `Vertex` collection, containing all the vertices that we could find.
- a `ReconstructedParticle` collection, that represent the particles that
  decayed at these vertices. Each of them will have exactly one `decayVertex`
  attached to them.
- a `RecoParticleVertexAssociation` collection that links each of the input
  `ReconstructedParticle`s back to the `Vertex` from which they emerged.

All of the steps will use function stubs whereever necessary and will mainly
focus on setting the relations / associations.

### Creating vertices

The main point here is to attach the decay particles to the `particles` field of
each `Vertex`. In this example we will assume that that is not already done by
the `magicVertexing` function; mainly to make this part very explicit.

```cpp
// This is a possible signature that works in the example below
std::vector<std::pair<edm4hep::Vertex, std::vector<edm4hep::ReconstructedParticle>>
magicVertexing(const edm4hep::ReconstructedParticleCollection&);

edm4hep::VertexCollection createVertices(const edm4hep::ReconstructedParticleCollection& particles) {
  auto vertices = edm4hep::VertexCollection{};

  // magicVertexing returns
  for (const auto& [vtx, decayParticles] : magicVertexing(particles)) {
    for (const auto p : decayParticles) {
      vtx.addToParticles(p);
    }
    vertices.push_back(vtx);
  }

  return vertices;
}
```

### Creating high level reconstruction particles

This is the most simplest implementation possible. It simply sums up all four
vectors of the decay particles and sets that into the newly created particles.
Again, this is mainly done for illustration purposes, as the important part is
the relation setting.

```cpp
// This is a possible signature for that works in the example below
edm4hep::LorentzVectorM sumFourMomenta(const podio::RelationRange<edm4hep::ReconstructedParticle>&);

edm4hep::ReconstructedParticleCollection createVertexRecos(const edm4hep::VertexCollection& vertices) {
  auto recos = edm4hep::ReconstructedParticleCollection{};
  for (const auto vtx : vertices) {
    auto reco = recos.create();
    reco.setDecayVertex(vtx);

    auto fourMom = sumFourMomenta(vtx.getParticles());
    reco.setMass(fourMom.M());
    reco.setMomentum(fourMom.X(), fourMom.Y(), fourMom.Z());
  }

  return recos;
}
```

#### Creating associations from `Vertex` to high level `ReconstructedParticle`

This is a potentially optional step that makes it possible to more easily access
the `ReconstructedParticle` that decayed at a `Vertex`

```cpp
edm4hep::RecoParticleVertexAssociationCollection
createVtxParticleAssociations(const edm4hep::ReconstructedParticleCollection& particles) {
  auto vtxPartAssocs = edm4hep::RecoParticleVertexAssociationCollection{};
  for (const auto p : particles) {
    auto assoc = vtxPartAssocs.create();
    assoc.setRec(p);
    assoc.setVertex(p.getDecayVertex());
  }
  
  return vtxPartAssocs;
}
```

### Creating start vertex associations

This is a potentially optional step, depending on whether it is necessary to
allow for easier navigation from the particles back to their start vertices.

```cpp
edm4hep::RecoParticleVertexAssociationCollection 
createStartVtxAssociations(const edm4hep::VertexCollection& vertices) {
  auto startVtxAssocs = edm4hep::RecoParticleVertexAssociationCollection{};
  for (const auto vtx : vertices) {
    for (const auto particle : vtx.getParticles()) {
      auto assoc = startVtxAssocs.create();
      assoc.setVertex(vtx);
      assoc.setRec(particle);
    }
  }
  return startVtxAssocs;
}
```

## EDM4hep vs LCIO

### The concept in LCIO

In LCIO the `Vertex` and `ReconstructedParticle` have similar but slightly
different relations (we will express them in podio/EDM4hep terminology here):
- a `Vertex` has a `OneToOneRelation` to an `associatedParticle`
- a `ReconstructeParticle` has a `OneToOneRelation` to a `startVertex`
- there is a similar association (`LCRelation`) between `Vertex` and
  `ReconstructedParticle` as in EDM4hep

The intended way to fill these in LCIO is
- a `Vertex` points back to the particle that decayed via the
  `associatedParticle`
- the `associateParticle` has the decay products in its `particles` relation
- each of the decay particles points back to the `Vertex` via the `startVertex`
  relation.

The main issue with this format is, that it's impossible to fill this
information in the EDM4hep mutability model without either
- doing the reconstruction of all particles, the vertexing and the high level
  reconstruction in one step
- or cloning collections of objects several times in order to be able to fill
  all the information consistently
  
In many cases the `Vertex` and `ReconstructedParticle` collection containing the
high level particle are created in one step in LCIO to circumvent parts of these
limitations.

### In LCIO I do this, how do I do it in EDM4hep?

#### Get the decay products from a `Vertex` 

This case is rather similar between the two datamodels. There is effectively
just one additional step involved for LCIO.

<table>
<tr>
<th>LCIO</th><th>EDM4hep</th>
</tr>
<tr>
<td>

```cpp
const auto& dps = vtx->getAssociatedParticle()->getParticles();
```

`dps` will be a `std::vector<EVENT::ReconstructedParticle*>`.

</td>
<td>

```cpp
const auto dps = vtx.getParticles();
```

`dps` will be a `podio::RelationRange<edm4hep::ReconstructeParticle>`.

</td>
</tr>
</table>

#### Get the particle associated to a `Vertex`

In this case there is a conceptual difference that requires to switch approaches
a bit. The example below assumes that you are looping over all vertices to
figure out the associated `ReconstructedParticle`. The main difference in this
case is that in EDM4hep the iteration does not go over a `Vertex` collection,
but rather a `RecoParticleVertexAssociation` collection.

<table>
<tr>
<th>LCIO</th><th>EDM4hep</th>
</tr>
<tr>
<td>

```cpp
const auto vtxColl = event->getCollection("vertices");

for (size_t i = 0; i < vtxColl->getNumberOfElements(); ++i) {
  const auto vtx = dynamic_cast<EVENT::Vertex*>(vtxColl->getElementAt(i));
  const auto reco = vtx->getAssociatedParticle();
  // .. do something with reco and vtx
}
```

</td>
<td>

```cpp
const auto& assocColl =
  event.get<edm4hep::RecoParticleVertexAssociationCollection>("vtx_particle_associations");

for (const auto assoc : assocColl) {
  const auto vtx = assoc.getVertex();
  const auto reco = assoc.getRec();
  // .. do something with reco and vtx
}
```

</td>
</tr>
</table>
