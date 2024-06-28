#include "read_events.h"

#include "podio/SIOReader.h"

int main(int, char**) {
  read_events<podio::SIOReader>("edm4hep_events.sio");

  return 0;
}
