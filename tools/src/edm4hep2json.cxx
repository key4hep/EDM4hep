// std
#include <filesystem>

// EDM4hep
#include "edm4hep2json.hxx"

// ROOT
#include "podio/ROOTReader.h"

// *nix
#include <getopt.h>

void printHelp() {
  std::cout << "Usage: edm4hep2json [olenvh] FILEPATH\n"
            << "  -o/--out-file           output file path\n"
            << "                            default: events.edm4hep.json\n"
            << "  -l/--coll-list          Comma separated list of collections "
                                         "to be converted\n"
            << "  -e/--events             Comma separated list of events to be processed\n"
            << "  -n/--nevents            maximal number of events to be precessed\n"
            << "  -v/--verbose            be more verbose\n"
            << "  -h/--help               show this help message"
            << std::endl;
}

int main(int argc, char** argv) {
  std::filesystem::path inFile;
  std::filesystem::path outFile;
  std::string requestedCollections;
  std::string requestedEvents;
  bool verboser = false;
  int nEventsMax = -1;

  const char* const short_opts = "o:l:e:n:vh";
  const option long_opts[] = {
    {"out-file", required_argument, nullptr, 'o'},
    {"coll-list", required_argument, nullptr, 'l'},
    {"events", required_argument, nullptr, 'e'},
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
        inFile = std::filesystem::path(optarg);
        break;
      case 'o':
        outFile = std::filesystem::path(optarg);
        break;
      case 'l':
        requestedCollections = std::string(optarg);
        break;
      case 'e':
        requestedEvents = std::string(optarg);
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

  for (int i = optind; i < argc; ++i) {
    inFile = std::string(argv[i]);
  }

  if (inFile.empty()) {
    std::cout << "ERROR: Input .root file not provided!" << std::endl;
    return EXIT_FAILURE;
  }

  if (!std::filesystem::exists(inFile)) {
    std::cout << "ERROR: Input .root file can't be read!" << std::endl;
    return EXIT_FAILURE;
  }

  if (requestedCollections.empty()) {
    requestedCollections =
        "GenParticles,BuildUpVertices,SiTracks,PandoraClusters,VertexJets,EventHeader";
    if (verboser) {
      std::cout << "DEBUG: Using default collection to convert:\n"
                << "       " << requestedCollections << std::endl;
    }
  }

  if (outFile.empty()) {
    outFile = inFile.filename().replace_extension("edm4hep.json");
  }

  read_events<podio::ROOTReader>(inFile,
                                 outFile,
                                 requestedCollections,
                                 requestedEvents,
                                 nEventsMax,
                                 verboser);

  return EXIT_SUCCESS;
}
