#include "read_events.h"
#include "podio/podioVersion.h"
#if PODIO_BUILD_VERSION >= PODIO_VERSION(0, 99, 0)
#include "podio/ROOTReader.h"
#else
#include "podio/ROOTFrameReader.h"
namespace podio {
using ROOTReader = podio::ROOTFrameReader;
}
#endif

int main() {
  read_events<podio::ROOTReader>("edm4hep_events.root");

  return 0;
}
