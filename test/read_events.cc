#include "read_events.h"
#if PODIO_VERSION_MAJOR > 0 || (PODIO_VERSION_MAJOR == 0 && PODIO_VERSION_MINOR >= 99)
#include "podio/ROOTReader.h"
#else
#include "podio/ROOTFrameReader.h"
#endif

int main() {
#if PODIO_VERSION_MAJOR > 0 || (PODIO_VERSION_MAJOR == 0 && PODIO_VERSION_MINOR >= 99)
  read_events<podio::ROOTReader>("edm4hep_events.root");
#else
  read_events<podio::ROOTFrameReader>("edm4hep_events.root");
#endif

  return 0;
}
