//////////////////////////////////////////////////////////////////////////
// Adapted from a HepMC by  Matt.Dobbs@Cern.CH, Feb 2000
// Example of building an event and a particle data table from scratch
// and converting it to the edm4hep data model
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <memory>
#include <unordered_map>

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Print.h"
#include "HepMC3/Units.h"

#include "HepPDT/ParticleID.hh"

#include "podio/Frame.h"
#if PODIO_VERSION_MAJOR > 0 || (PODIO_VERSION_MAJOR == 0 && PODIO_VERSION_MINOR >= 99)
#include "podio/ROOTWriter.h"
#else
#include "podio/ROOTFrameWriter.h"
#endif

#include "edm4hep/MCParticleCollection.h"

using namespace HepMC3;

edm4hep::MutableMCParticle convert(std::shared_ptr<GenParticle> hepmcParticle) {
  auto edm_particle = edm4hep::MutableMCParticle();
  edm_particle.setPDG(hepmcParticle->pdg_id());
  edm_particle.setGeneratorStatus(hepmcParticle->status());
  // look up charge from pdg_id
  HepPDT::ParticleID particleID(hepmcParticle->pdg_id());
  edm_particle.setCharge(particleID.charge());
  //  convert momentum
  auto p = hepmcParticle->momentum();
  edm_particle.setMomentum({float(p.px()), float(p.py()), float(p.pz())});

  return edm_particle;
}

int main() {
  // Part 1: Using the HepMC example code to "generate" an event
  //
  // In this example we will place the following event into HepMC "by hand"
  //
  //     name status pdg_id  parent Px       Py    Pz       Energy      Mass
  //  1  !p+!    3   2212    0,0    0.000    0.000 7000.000 7000.000    0.938
  //  2  !p+!    3   2212    0,0    0.000    0.000-7000.000 7000.000    0.938
  //=========================================================================
  //  3  !d!     3      1    1,1    0.750   -1.569   32.191   32.238    0.000
  //  4  !u~!    3     -2    2,2   -3.047  -19.000  -54.629   57.920    0.000
  //  5  !W-!    3    -24    1,2    1.517   -20.68  -20.605   85.925   80.799
  //  6  !gamma! 1     22    1,2   -3.813    0.113   -1.833    4.233    0.000
  //  7  !d!     1      1    5,5   -2.445   28.816    6.082   29.552    0.010
  //  8  !u~!    1     -2    5,5    3.962  -49.498  -26.687   56.373    0.006

  // now we build the graph, which will look like
  //                       p7                         #
  // p1                   /                           #
  //   \v1__p3      p5---v4                           #
  //         \_v3_/       \                           #
  //         /    \        p8                         #
  //    v2__p4     \                                  #
  //   /            p6                                #
  // p2                                               #
  //                                                  #

  // First create the event container
  auto evt = new GenEvent();
  // define the units
  evt->set_units(HepMC3::Units::GEV, HepMC3::Units::MM);
  //
  // create vertex 1 and vertex 2, together with their inparticles
  GenVertex* v1 = new GenVertex();
  evt->add_vertex(v1);
  v1->add_particle_in(new GenParticle(FourVector(0, 0, 7000, 7000), 2212, 3));
  GenVertex* v2 = new GenVertex();
  evt->add_vertex(v2);
  v2->add_particle_in(new GenParticle(FourVector(0, 0, -7000, 7000), 2212, 3));
  //
  // create the outgoing particles of v1 and v2
  GenParticle* p3 = new GenParticle(FourVector(.750, -1.569, 32.191, 32.238), 1, 3);
  v1->add_particle_out(p3);
  GenParticle* p4 = new GenParticle(FourVector(-3.047, -19., -54.629, 57.920), -2, 3);
  v2->add_particle_out(p4);
  //
  // create v3
  GenVertex* v3 = new GenVertex();
  evt->add_vertex(v3);
  v3->add_particle_in(p3);
  v3->add_particle_in(p4);
  v3->add_particle_out(new GenParticle(FourVector(-3.813, 0.113, -1.833, 4.233), 22, 1));
  GenParticle* p5 = new GenParticle(FourVector(1.517, -20.68, -20.605, 85.925), -24, 3);
  v3->add_particle_out(p5);
  //
  // create v4
  GenVertex* v4 = new GenVertex(FourVector(0.12, -0.3, 0.05, 0.004));
  evt->add_vertex(v4);
  v4->add_particle_in(p5);
  v4->add_particle_out(new GenParticle(FourVector(-2.445, 28.816, 6.082, 29.552), 1, 1));
  v4->add_particle_out(new GenParticle(FourVector(3.962, -49.498, -26.687, 56.373), -2, 1));
  //
  // the event is complete, we now print it out to the screen
  Print::content(*evt);

  // now clean-up by deleteing all objects from memory
  //
  // deleting the event deletes all contained vertices, and all particles
  // contained in those vertices

  // Part2: Convert the particles and write to file
  std::unordered_map<unsigned int, edm4hep::MutableMCParticle> hepmcToEdmMap;
  unsigned int particle_counter{0};
  for (auto particle_i : evt->particles()) {
    std::cout << "Converting particle with PDG ID: " << particle_i->pdg_id() << std::endl;
    std::cout << "\t and id: " << particle_i->id() << std::endl;

    if (hepmcToEdmMap.find(particle_i->id()) == hepmcToEdmMap.end()) {
      auto edm_particle = convert(particle_i);
      hepmcToEdmMap.insert({particle_i->id(), edm_particle});
    }

    // mother/daughter links
    auto prodvertex = particle_i->production_vertex();
    if (nullptr != prodvertex) {

      for (auto particle_mother : prodvertex->particles_in()) {
        if (hepmcToEdmMap.find(particle_mother->id()) == hepmcToEdmMap.end()) {
          auto edm_particle = convert(particle_mother);
          hepmcToEdmMap.insert({particle_mother->id(), edm_particle});
        }
        hepmcToEdmMap[particle_i->id()].addToParents(hepmcToEdmMap[particle_mother->id()]);
      }
    }

    auto endvertex = particle_i->end_vertex();
    if (nullptr != prodvertex) {

      for (auto particle_daughter : prodvertex->particles_in()) {
        if (hepmcToEdmMap.find(particle_daughter->id()) == hepmcToEdmMap.end()) {
          auto edm_particle = convert(particle_daughter);
          hepmcToEdmMap.insert({particle_daughter->id(), edm_particle});
        }
        hepmcToEdmMap[particle_i->id()].addToDaughters(hepmcToEdmMap[particle_daughter->id()]);
      }
    }

    particle_counter++;
  }

  auto edm_particle_collection = edm4hep::MCParticleCollection();
  for (auto b_p : hepmcToEdmMap) {
    edm_particle_collection.push_back(b_p.second);
  }
  auto event = podio::Frame();
  event.put(std::move(edm_particle_collection), "TestParticles2");

#if PODIO_VERSION_MAJOR > 0 || (PODIO_VERSION_MAJOR == 0 && PODIO_VERSION_MINOR >= 99)
  auto writer = podio::ROOTWriter("edm4hep_testhepmc.root");
#else
  auto writer = podio::ROOTFrameWriter("edm4hep_testhepmc.root");
#endif
  writer.writeFrame(event, "events");

  // after all events
  writer.finish();

  return 0;
}
