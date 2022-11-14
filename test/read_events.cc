#include "read_events.h"
#include "podio/ROOTFrameReader.h"

int main() {
  read_events<podio::ROOTFrameReader>("edm4hep_events.root");

  return 0;
}
