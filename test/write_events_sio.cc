#include "write_events.h"

#include "podio/SIOFrameWriter.h"

int main(int, char**) {
  write<podio::SIOFrameWriter>("edm4hep_events.sio");
  return 0;
}
