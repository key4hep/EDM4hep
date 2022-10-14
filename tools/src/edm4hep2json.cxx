// EDM4hep
#include "edm4hep2json.hxx"

// ROOT
#include "podio/ROOTReader.h"

// *nix
#include <getopt.h>

void printHelp() {
  std::cout << "Usage:\n"
            << "  -i/--in-file            input file path\n"
            << "  -o/--out-file           output file path\n"
            << "                            default: events.edm4hep.json\n"
            << "  -l/--coll-list          Comma separated list of collections "
                                         "to be converted\n"
            << "  -n/--nevents            number of events to be precessed\n"
            << "  -v/--verbose            be more verbose\n"
            << "  -h/--help               show this help message"
            << std::endl;
}

int main(int argc, char** argv) {
  std::string inFile;
  std::string outFile;
  std::string requestedCollections;
  bool verboser = false;
  int nEventsMax = -1;

  const char* const short_opts = "i:o:l:n:vh";
  const option long_opts[] = {
    {"in-file", required_argument, nullptr, 'i'},
    {"out-file", required_argument, nullptr, 'o'},
    {"coll-list", required_argument, nullptr, 'l'},
    {"nevents", required_argument, nullptr, 'n'},
    {"verbose", no_argument, nullptr, 'v'},
    {"help", no_argument, nullptr, 'h'},
    {nullptr, no_argument, nullptr, 0}
  };

  while (true) {
    const auto opt = getopt_long(argc, argv, short_opts, long_opts, 0);

    if (opt == -1) {
      break;
    }

    switch (opt) {
      case 'i':
        inFile = std::string(optarg);
        break;
      case 'o':
        outFile = std::string(optarg);
        break;
      case 'l':
        requestedCollections = std::string(optarg);
        break;
      case 'n':
        nEventsMax = std::stoi(optarg);
        break;
      case 'v':
        verboser = true;
        break;
      case 'h':
      case '?':
      default:
        printHelp();
        return EXIT_SUCCESS;
    }
  }

  if (inFile.empty()) {
    std::cout << "ERROR: Input .root file not provided!" << std::endl;
    return EXIT_FAILURE;
  }

  if (requestedCollections.empty()) {
    requestedCollections =
        "GenParticles,BuildUpVertices,SiTracks,PandoraClusters,VertexJets";
    std::cout << "WARNING: Using default collection to convert:\n"
              << "         " << requestedCollections << std::endl;
  }

  if (outFile.empty()) {
    outFile = "events.edm4hep.json";
    std::cout << "WARNING: Using default output file path:\n"
              << "         " << outFile << std::endl;
  }

  read_events<podio::ROOTReader>(inFile,
                                 outFile,
                                 requestedCollections,
                                 nEventsMax,
                                 verboser);

  return EXIT_SUCCESS;
}
