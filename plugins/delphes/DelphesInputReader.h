#ifndef DELPHESEDM4HEP_DELPHESINPUTREADER
#define DELPHESEDM4HEP_DELPHESINPUTREADER

#include <string>

class TObjArray;
class Delphes;

class DelphesInputReader {
public:

  virtual bool init(Delphes* modularDelphes, int argc, char *argv[], std::string& outputfile) = 0;

  virtual std::string getUsage() = 0;

  virtual int getNumberOfEvents() = 0;
  virtual bool finished() = 0;

  virtual bool readEvent(Delphes* modularDelphes, TObjArray* allParticleOutputArray,
  TObjArray* stableParticleOutputArray, TObjArray* partonOutputArray) = 0;

};


#endif
