
#include "DelphesMainNew.h"
#include "DelphesHepMCReader.h"

int main(int argc, char *argv[]) {
  DelphesHepMCInputReader inputReader = DelphesHepMCInputReader();
  return doit(argc, argv, inputReader);
}
