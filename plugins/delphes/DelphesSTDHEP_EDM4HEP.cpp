
#include "DelphesMain.h"
#include "DelphesSTDHEPInputReader.h"

int main(int argc, char *argv[]) {
  DelphesSTDHEPInputReader inputReader = DelphesSTDHEPInputReader();
  return doit(argc, argv, inputReader);
}
