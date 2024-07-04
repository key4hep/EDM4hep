#include "write_events.h"

#include "podio/SIOWriter.h"

int main(int, char**) {
  write<podio::SIOWriter>("edm4hep_events.sio");
  return 0;
}
