#include "write_events.h"
#include "podio/podioVersion.h"
#if PODIO_BUILD_VERSION >= PODIO_VERSION(0, 99, 0)
#include "podio/ROOTWriter.h"
#else
#include "podio/ROOTFrameWriter.h"
namespace podio {
using ROOTWriter = podio::ROOTFrameWriter;
}
#endif

int main(int, char*[]) {

  write<podio::ROOTWriter>("edm4hep_events.root");
}
