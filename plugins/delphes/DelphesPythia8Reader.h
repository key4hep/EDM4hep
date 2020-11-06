#ifndef DELPHESEDM4HEP_DELPHESPYTHIA8READER
#define DELPHESEDM4HEP_DELPHESPYTHIA8READER

#include <iostream>

#include "DelphesInputReader.h"
#include "DelphesPythia8Common.h"

#include "TObjArray.h"
#include "TChain.h"
#include "TClonesArray.h"
#include "TParticlePDG.h"
#include "TStopwatch.h"
#include "TDatabasePDG.h"

#include "classes/DelphesClasses.h"
#include "classes/DelphesStream.h"
#include "classes/DelphesLHEFReader.h"
#include "modules/Delphes.h"
#include "ExRootAnalysis/ExRootTreeWriter.h"

#include "Pythia.h"
#include "Pythia8Plugins/CombineMatchingInput.h"

#include <iostream>

//---------------------------------------------------------------------------


class DelphesPythia8Reader: public DelphesInputReader {
  public:
  inline DelphesPythia8Reader() {};
    inline ~DelphesPythia8Reader() {
      if (pythia) {
        delete pythia;
      }
    };

  inline bool init(Delphes* modularDelphes, int argc, char *argv[], std::string& outputfile) {
    if (argc != 5) {
      std::cout << "Usage: " << m_appName << "config_file output_config_file pythia_card output_file\n"
                << "config_file - configuration file in Tcl format,\n"
                << "output_config_file - configuration file steering the content of the edm4hep output in Tcl format,\n"
                << "pythia_card - Pythia8 configuration file,\n"
                << "output_file - output file in ROOT format." << std::endl;
      return false;
    }
    outputfile = argv[4];
    // Initialize Pythia
    pythia = new Pythia8::Pythia;

    // jet matching
#if PYTHIA_VERSION_INTEGER < 8300
    matching = combined->getHook(*pythia);
    if(!matching)
    {
      throw std::runtime_error("can't do matching");
    }
    pythia->setUserHooksPtr(matching);
#endif

    if(pythia == NULL)
    {
      throw std::runtime_error("can't create Pythia instance");
    }

    // Read in commands from configuration file

    std::stringstream message;
    std::string pythia8configname(argv[3]);
    if(!pythia->readFile(pythia8configname))
    {
      message << "can't read Pythia8 configuration file " << pythia8configname << std::endl;
      throw std::runtime_error(message.str());
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
          std::cerr << "Aborted since reached end of Les Houches Event File" << std::endl;
          return false;
        }
        // First few failures write off as "acceptable" errors, then quit
        if(++errorCounter > timesAllowErrors) {
          std::cerr << "Event generation aborted prematurely, owing to error!" << std::endl;
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
      ++m_entry;
    return true;
    };

  inline bool finished() {return m_entry >= m_numberOfEvents;};

private:
  const std::string m_appName = "DelphesPythia8";
  const std::string m_usage;
  int m_numberOfEvents;
  int m_entry = 0;
  Pythia8::Pythia* pythia{nullptr};
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
