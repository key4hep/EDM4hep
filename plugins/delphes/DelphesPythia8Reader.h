#ifndef DELPHESEDM4HEP_DELPHESPYTHIA8READER
#define DELPHESEDM4HEP_DELPHESPYTHIA8READER

#include <iostream>

#include "DelphesInputReader.h"

#include "TObjArray.h"
#include "TChain.h"
#include "TClonesArray.h"

#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"
#include "classes/DelphesStream.h"
#include "classes/DelphesLHEFReader.h"
#include "modules/Delphes.h"

#include "Pythia.h"
#include "Pythia8Plugins/CombineMatchingInput.h"

using std::runtime_error;
using std::stringstream;

void ConvertInput(Long64_t eventCounter,
                  Pythia8::Pythia *pythia,
                  ExRootTreeBranch *branch,
                  DelphesFactory *factory,
                  TObjArray *allParticleOutputArray,
                  TObjArray *stableParticleOutputArray,
                  TObjArray *partonOutputArray,
                  TStopwatch *readStopWatch,
                  TStopwatch *procStopWatch) {

  HepMCEvent *element;
  Candidate *candidate;
  TDatabasePDG *pdg;
  TParticlePDG *pdgParticle;
  Int_t pdgCode;

  Int_t pid, status;
  Double_t px, py, pz, e, mass;
  Double_t x, y, z, t;

  // event information
  element = new HepMCEvent(); //(branch->NewEntry());

  element->Number = eventCounter;

  element->ProcessID = pythia->info.code();
  element->MPI = 1;
  element->Weight = pythia->info.weight();
  element->Scale = pythia->info.QRen();
  element->AlphaQED = pythia->info.alphaEM();
  element->AlphaQCD = pythia->info.alphaS();

  element->ID1 = pythia->info.id1();
  element->ID2 = pythia->info.id2();
  element->X1 = pythia->info.x1();
  element->X2 = pythia->info.x2();
  element->ScalePDF = pythia->info.QFac();
  element->PDF1 = pythia->info.pdf1();
  element->PDF2 = pythia->info.pdf2();

  element->ReadTime = readStopWatch->RealTime();
  element->ProcTime = procStopWatch->RealTime();

  pdg = TDatabasePDG::Instance();

  for(int i = 1; i < pythia->event.size(); ++i) {
    Pythia8::Particle &particle = pythia->event[i];

    pid = particle.id();
    status = particle.statusHepMC();
    px = particle.px();
    py = particle.py();
    pz = particle.pz();
    e = particle.e();
    mass = particle.m();
    x = particle.xProd();
    y = particle.yProd();
    z = particle.zProd();
    t = particle.tProd();

    candidate = factory->NewCandidate();

    candidate->PID = pid;
    pdgCode = TMath::Abs(candidate->PID);

    candidate->Status = status;

    candidate->M1 = particle.mother1() - 1;
    candidate->M2 = particle.mother2() - 1;

    candidate->D1 = particle.daughter1() - 1;
    candidate->D2 = particle.daughter2() - 1;

    pdgParticle = pdg->GetParticle(pid);
    candidate->Charge = pdgParticle ? Int_t(pdgParticle->Charge() / 3.0) : -999;
    candidate->Mass = mass;

    candidate->Momentum.SetPxPyPzE(px, py, pz, e);

    candidate->Position.SetXYZT(x, y, z, t);

    allParticleOutputArray->Add(candidate);

    if(!pdgParticle) continue;

    if(status == 1)
    {
      stableParticleOutputArray->Add(candidate);
    }
    else if(pdgCode <= 5 || pdgCode == 21 || pdgCode == 15)
    {
      partonOutputArray->Add(candidate);
    }
  }
}

/*
Single-particle gun. The particle must be a colour singlet.
Input: flavour, energy, direction (theta, phi).
If theta < 0 then random choice over solid angle.
Optional final argument to put particle at rest => E = m.
from pythia8 example 21
*/

void fillParticle(int id, double pMax, double etaMax,
  Pythia8::Event &event, Pythia8::ParticleData &pdt, Pythia8::Rndm &rndm)
{
  // Reset event record to allow for new event.
  event.reset();

  // Generate uniform pt and eta.
  double pt, eta, phi, pp, ee, mm;

  // pMin = 0.1 GeV for single particles
  pp = pow(10, -1.0 + (log10(pMax) + 1.0) * rndm.flat());
  eta = (2.0 * rndm.flat() - 1.0) * etaMax;
  phi = 2.0 * M_PI * rndm.flat();
  mm = pdt.mSel(id);
  ee = Pythia8::sqrtpos(pp * pp + mm * mm);
  pt = pp / cosh(eta);

  // Store the particle in the event record.
  event.append(id, 1, 0, 0, pt * cos(phi), pt * sin(phi), pt * sinh(eta), ee, mm);
}

//---------------------------------------------------------------------------

void fillPartons(int id, double pMax, double etaMax,
  Pythia8::Event &event, Pythia8::ParticleData &pdt, Pythia8::Rndm &rndm)
{
  // Reset event record to allow for new event.
  event.reset();

  // Generate uniform pt and eta.
  double pt, eta, phi, pp, ee, mm;

  // pMin = 1 GeV for jets
  pp = pow(10, log10(pMax) * rndm.flat());
  eta = (2.0 * rndm.flat() - 1.0) * etaMax;
  phi = 2.0 * M_PI * rndm.flat();
  mm = pdt.mSel(id);
  ee = Pythia8::sqrtpos(pp * pp + mm * mm);
  pt = pp / cosh(eta);

  if((id == 4 || id == 5) && pt < 10.0) return;

  if(id == 21)
  {
    event.append(21, 23, 101, 102, pt * cos(phi), pt * sin(phi), pt * sinh(eta), ee);
    event.append(21, 23, 102, 101, -pt * cos(phi), -pt * sin(phi), -pt * sinh(eta), ee);
  }
  else
  {
    event.append(id, 23, 101, 0, pt * cos(phi), pt * sin(phi), pt * sinh(eta), ee, mm);
    event.append(-id, 23, 0, 101, -pt * cos(phi), -pt * sin(phi), -pt * sinh(eta), ee, mm);
  }
}

//---------------------------------------------------------------------------


class DelphesPythia8Reader: public DelphesInputReader {
  public:
  inline DelphesPythia8Reader() {};
  inline ~DelphesPythia8Reader() {delete pythia;};
  inline bool init(Delphes* modularDelphes, int argc, char *argv[], std::string& outputfile) {
    outputfile = argv[3];

    // Initialize Pythia
    pythia = new Pythia8::Pythia;

    // jet matching
    matching = combined->getHook(*pythia);
    if(!matching)
    {
      throw runtime_error("can't do matching");
    }
    pythia->setUserHooksPtr(matching);

    if(pythia == NULL)
    {
      throw runtime_error("can't create Pythia instance");
    }

    // Read in commands from configuration file

    stringstream message;
    std::string pythia8configname(argv[2]);
    if(!pythia->readFile(pythia8configname))
    {
      message << "can't read Pythia8 configuration file " << pythia8configname << endl;
      throw runtime_error(message.str());
    }

    // Extract settings to be used in the main program
    numberOfEvents = pythia->mode("Main:numberOfEvents");

    m_numberOfEvents = pythia->mode("Main:numberOfEvents");
    timesAllowErrors = pythia->mode("Main:timesAllowErrors");
    spareFlag1 = pythia->flag("Main:spareFlag1");
    spareMode1 = pythia->mode("Main:spareMode1");
    spareParm1 = pythia->parm("Main:spareParm1");
    spareParm2 = pythia->parm("Main:spareParm2");

    // Check if particle gun
    if(!spareFlag1) {
      inputFile = fopen(pythia->word("Beams:LHEF").c_str(), "r");
      if(inputFile) {
        reader = new DelphesLHEFReader;
        reader->SetInputFile(inputFile);

        branchEventLHEF = treeWriter->NewBranch("EventLHEF", LHEFEvent::Class());
        branchWeightLHEF = treeWriter->NewBranch("WeightLHEF", LHEFWeight::Class());

        allParticleOutputArrayLHEF = modularDelphes->ExportArray("allParticlesLHEF");
        stableParticleOutputArrayLHEF = modularDelphes->ExportArray("stableParticlesLHEF");
        partonOutputArrayLHEF = modularDelphes->ExportArray("partonsLHEF");
      }
    }

    pythia->init();

    return true;

    };
  inline int getNumberOfEvents() {return m_numberOfEvents;}

  inline std::string getUsage() {return m_appName;};

  inline bool readEvent(Delphes* modularDelphes, TObjArray* allParticleOutputArray,
  TObjArray* stableParticleOutputArray, TObjArray* partonOutputArray) {

    auto factory = modularDelphes->GetFactory();
      while(reader && reader->ReadBlock(factory, allParticleOutputArrayLHEF, stableParticleOutputArrayLHEF, partonOutputArrayLHEF) && !reader->EventReady()) ;

      if(spareFlag1) {
        if((spareMode1 >= 1 && spareMode1 <= 5) || spareMode1 == 21) {
          fillPartons(spareMode1, spareParm1, spareParm2, pythia->event, pythia->particleData, pythia->rndm);
        } else {
          fillParticle(spareMode1, spareParm1, spareParm2, pythia->event, pythia->particleData, pythia->rndm);
        }
      }

      if(!pythia->next()) {
        // If failure because reached end of file then exit event loop
        if(pythia->info.atEndOfFile()) {
          cerr << "Aborted since reached end of Les Houches Event File" << endl;
          return false;
        }
        // First few failures write off as "acceptable" errors, then quit
        if(++errorCounter > timesAllowErrors) {
          cerr << "Event generation aborted prematurely, owing to error!" << endl;
          return false;
        }
        modularDelphes->Clear();
        reader->Clear();
      }
      readStopWatch.Stop();
      procStopWatch.Start();
      ConvertInput(eventCounter, pythia, branchEvent, factory,
        allParticleOutputArray, stableParticleOutputArray, partonOutputArray,
        &readStopWatch, &procStopWatch);
    return true;
    };

    inline bool finished() {return m_entry < m_numberOfEvents;};

private:
  const std::string m_appName = "DelphesPythia8";
  const std::string m_usage;
  int m_numberOfEvents;
  int m_entry = 0;
  Pythia8::Pythia* pythia;

  FILE *inputFile = 0;
  TFile *outputFile = 0;
  TStopwatch readStopWatch, procStopWatch;
  ExRootTreeWriter *treeWriter = 0;
  ExRootTreeBranch *branchEvent = 0;
  ExRootTreeBranch *branchEventLHEF = 0, *branchWeightLHEF = 0;
  ExRootConfReader *confReader = 0;
  TObjArray *stableParticleOutputArrayLHEF = 0, *allParticleOutputArrayLHEF = 0, *partonOutputArrayLHEF = 0;
  DelphesLHEFReader *reader = 0;
  Long64_t eventCounter, errorCounter;
  Long64_t numberOfEvents, timesAllowErrors;
  Bool_t spareFlag1;
  Int_t spareMode1;
  Double_t spareParm1, spareParm2;

  TClonesArray* m_branchParticle;
  TClonesArray* m_branchHepMCEvent;

  // for matching
  Pythia8::CombineMatchingInput *combined = 0;
  Pythia8::UserHooks *matching = 0;
};


#endif
