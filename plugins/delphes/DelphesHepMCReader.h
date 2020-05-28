#ifndef DELPHESEDM4HEP_DELPHESHEPMCREADER
#define DELPHESEDM4HEP_DELPHESHEPMCREADER

#include <iostream>

#include "DelphesInputReader.h"

#include "TObjArray.h"
#include "TChain.h"
#include "TClonesArray.h"

#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"
#include "classes/DelphesStream.h"
#include "classes/DelphesHepMCReader.h"
#include "modules/Delphes.h"

#include "ExRootAnalysis/ExRootTreeBranch.h"
#include "ExRootAnalysis/ExRootTreeReader.h"


//// TODO: handle case of more than one input file

using std::stringstream;
using std::runtime_error;

class DelphesHepMCInputReader: public DelphesInputReader {
  public:
  inline DelphesHepMCInputReader() {};
  inline bool init(Delphes* modularDelphes, int argc, char *argv[], std::string& outputfile) {
    outputfile = argv[2];

    int i = 3;

    branchEvent = new ExRootTreeBranch("Event", HepMCEvent::Class());
    branchWeight = new ExRootTreeBranch("Weight", Weight::Class());
    reader = new DelphesHepMCReader;

      if(i == argc || strncmp(argv[i], "-", 2) == 0)
      {
        cout << "** Reading standard input" << endl;
        inputFile = stdin;
        length = -1;
      }
      else
      {
        cout << "** Reading " << argv[i] << endl;
        inputFile = fopen(argv[i], "r");

        if(inputFile == NULL)
        {
          message << "can't open " << argv[i];
          throw runtime_error(message.str());
        }

        fseek(inputFile, 0L, SEEK_END);
        length = ftello(inputFile);
        fseek(inputFile, 0L, SEEK_SET);

        if(length <= 0)
        {
          fclose(inputFile);
          ++i;
          //continue;
        }
      }

      reader->SetInputFile(inputFile);

    return true;

    };
  inline int getNumberOfEvents() {return m_numberOfEvents;}

  inline std::string getUsage() {return m_appName;};

  inline bool readEvent(Delphes* modularDelphes, TObjArray* allParticleOutputArray,
  TObjArray* stableParticleOutputArray, TObjArray* partonOutputArray) {
      readStopWatch.Start();
      auto factory = modularDelphes->GetFactory();
      do {
        m_finished = reader->ReadBlock(factory, allParticleOutputArray, stableParticleOutputArray, partonOutputArray);
      } while(m_finished && !reader->EventReady());
      readStopWatch.Stop();
      reader->AnalyzeEvent(branchEvent, eventCounter, &readStopWatch, &procStopWatch);
      reader->AnalyzeWeight(branchWeight);
      reader->Clear();
      return m_finished;
    };

    inline bool finished() {return m_finished;};

private:
  const std::string m_appName = "DelphesHepMC";
  const std::string m_usage;
  int m_numberOfEvents;
  int m_entry = 0;
  bool m_finished = false;
  TChain* m_chain;
  ExRootTreeReader* m_treeReader =  nullptr;
  TClonesArray* m_branchParticle;
  TClonesArray* m_branchHepMCEvent;


  std::stringstream message;
  FILE *inputFile = 0;
  TFile *outputFile = 0;
  TStopwatch readStopWatch, procStopWatch;
  ExRootTreeWriter *treeWriter = 0;
  ExRootTreeBranch *branchEvent = 0, *branchWeight = 0;
  ExRootConfReader *confReader = 0;
  Delphes *modularDelphes = 0;
  DelphesFactory *factory = 0;
  TObjArray *stableParticleOutputArray = 0, *allParticleOutputArray = 0, *partonOutputArray = 0;
  DelphesHepMCReader *reader = 0;
  Int_t i, maxEvents, skipEvents;
  Long64_t length, eventCounter;

};


#endif
