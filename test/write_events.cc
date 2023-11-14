#include "write_events.h"
#include "podio/ROOTFrameWriter.h"

int main(int, char*[]) {

  write<podio::ROOTFrameWriter>("edm4hep_events.root");
}
