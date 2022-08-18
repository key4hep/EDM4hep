#include "podio/SIOReader.h"
#include "read_events.h"

int main(int, char**) {
  read_events<podio::SIOReader>("edm4hep_events.sio");

  return 0;
}
