#include "write_events.h"
#include "podio/ROOTWriter.h"

int main(int, char*[]) { write<podio::ROOTWriter>("edm4hep_events.root"); }
