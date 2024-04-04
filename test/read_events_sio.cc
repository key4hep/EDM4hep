#include "read_events.h"

#include "podio/podioVersion.h"
#if PODIO_BUILD_VERSION >= PODIO_VERSION(0, 99, 0)
#include "podio/SIOReader.h"
#else
#include "podio/SIOFrameReader.h"
namespace podio {
using SIOReader = podio::SIOFrameReader;
}
#endif

int main(int, char**) {
  read_events<podio::SIOReader>("edm4hep_events.sio");

  return 0;
}
