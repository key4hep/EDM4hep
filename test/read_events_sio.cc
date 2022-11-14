#include "read_events.h"

#include "podio/SIOFrameReader.h"

int main(int, char**) {
  read_events<podio::SIOFrameReader>("edm4hep_events.sio");

  return 0;
}
