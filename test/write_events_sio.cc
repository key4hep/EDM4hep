#include "write_events.h"

#include "podio/podioVersion.h"
#if PODIO_BUILD_VERSION >= PODIO_VERSION(0, 99, 0)
#include "podio/SIOWriter.h"
#else
#include "podio/SIOFrameWriter.h"
namespace podio {
using SIOWriter = podio::SIOFrameWriter;
}
#endif

int main(int, char**) {
  write<podio::SIOWriter>("edm4hep_events.sio");
  return 0;
}
