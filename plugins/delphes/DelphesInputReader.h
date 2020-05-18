#ifndef DELPHESEDM4HEP_DELPHESINPUTREADER
#define DELPHESEDM4HEP_DELPHESINPUTREADER

#include <string>

class TObjArray;
class Delphes;

class DelphesInputReader {
public:

  virtual bool init(int argc, char *argv[]) = 0;

  virtual std::string getUsage() = 0;

  virtual int getNumberOfEvents() = 0;

  virtual void readEvent(Delphes* modularDelphes, TObjArray* allParticleOutputArray,
  TObjArray* stableParticleOutputArray, TObjArray* partonOutputArray) = 0;

};


#endif
