
#include "DelphesMain.h"
#include "DelphesPythia8Reader.h"

int main(int argc, char *argv[]) {
  DelphesPythia8Reader inputReader = DelphesPythia8Reader();
  return doit(argc, argv, inputReader);
}
