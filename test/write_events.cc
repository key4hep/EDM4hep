#include "write_events.h"

#include "podio/ROOTWriter.h"

#include "edm4hep/MCParticleCollection.h"

int main(int, char*[]) {

  edm4hep::MCParticleCollection mcparticles;
  for (size_t i = 0; i < 10000000; ++i) {
    edm4hep::MutableMCParticle particle;
    mcparticles.push_back(particle);
  }
  // mcparticles.prepareForWrite();
  // write<podio::ROOTWriter>("edm4hep_events.root");
}
