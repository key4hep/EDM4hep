#include "DelphesEDM4HepConverter.h"
#include "DelphesInputReader.h"

#include "TObjArray.h"

#include "ExRootAnalysis/ExRootProgressBar.h"
#include "Delphes.h"

#include <iostream>
#include <signal.h> // SIGINT
#include <memory>

// gracefully handle ctrl+c
static bool interrupted = false;
void SignalHandler(int sig) {
  interrupted = true;
}



int doit(int argc, char* argv[], DelphesInputReader& inputReader) {
    Delphes* modularDelphes = new Delphes("Delphes");
    std::string outputfile;
    if (!inputReader.init(modularDelphes, argc, argv, outputfile)) {
        return 1;
    }
    signal(SIGINT, SignalHandler);

    try {
      auto confReader = std::make_unique<ExRootConfReader>();
      confReader->ReadFile(argv[1]);
      modularDelphes->SetConfReader(confReader.get());
      // since even ExRootConfParam::GetSize() is not marked const it is useless
      // to get a const version of it here
      auto branches = confReader->GetParam("TreeWriter::Branch");

      DelphesEDM4HepConverter edm4hepConverter(outputfile, branches, confReader.get());

      // has to happen before InitTask
      TObjArray* allParticleOutputArray = modularDelphes->ExportArray("allParticles");
      TObjArray* stableParticleOutputArray = modularDelphes->ExportArray("stableParticles");
      TObjArray* partonOutputArray = modularDelphes->ExportArray("partons");

      modularDelphes->InitTask();
      modularDelphes->Clear();

      int maxEvents = confReader->GetInt("::MaxEvents", 0);
      ExRootProgressBar progressBar(-1);
      Int_t eventCounter = 0;
      for (Int_t entry = 0;
           !inputReader.finished() && (maxEvents > 0 ?  entry < maxEvents : true) && !interrupted;
           ++entry) {

        bool success = inputReader.readEvent(modularDelphes,
                                             allParticleOutputArray,
                                             stableParticleOutputArray,
                                             partonOutputArray);
        if (!success) {
          break;
        }

        modularDelphes->ProcessTask();
        edm4hepConverter.process(modularDelphes);
        edm4hepConverter.writeEvent();
        modularDelphes->Clear();
        progressBar.Update(eventCounter, eventCounter);
        eventCounter++;
      }

      progressBar.Update(eventCounter, eventCounter, true);
      progressBar.Finish();
      edm4hepConverter.finish();
      modularDelphes->Finish();
      std::cout << "** Exiting ..." << std::endl;


    } catch (std::runtime_error& e) {
      std::cerr << "** ERROR: " << e.what() << std::endl;
      return 1;
    }

    return 0;
}
