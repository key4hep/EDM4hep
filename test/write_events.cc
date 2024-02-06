#include "write_events.h"
#if PODIO_VERSION_MAJOR > 0 || (PODIO_VERSION_MAJOR == 0 && PODIO_VERSION_MINOR >= 99)
#include "podio/ROOTWriter.h"
#else
#include "podio/ROOTFrameWriter.h"
#endif

int main(int, char*[]) {

#if PODIO_VERSION_MAJOR > 0 || (PODIO_VERSION_MAJOR == 0 && PODIO_VERSION_MINOR >= 99)
  write<podio::ROOTWriter>("edm4hep_events.root");
#else
  write<podio::ROOTFrameWriter>("edm4hep_events.root");
#endif
}
