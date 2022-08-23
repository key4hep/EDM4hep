#include "write_events.h"
#include "podio/ROOTWriter.h"

int main(int argc, char* argv[]) {

  write<podio::ROOTWriter>("edm4hep_events.root");
}
