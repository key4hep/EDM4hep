#include "read_events.h"
#include "podio/ROOTReader.h"

int main() {
  read_events<podio::ROOTReader>("edm4hep_events.root");

  return 0;
}
