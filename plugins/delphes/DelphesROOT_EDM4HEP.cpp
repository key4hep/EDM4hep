
#include "DelphesMain.h"
#include "DelphesRootReader.h"

int main(int argc, char *argv[]) {
  DelphesRootReader inputReader = DelphesRootReader();
  return doit(argc, argv, inputReader);
}
