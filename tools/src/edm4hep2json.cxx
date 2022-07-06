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
            << "                            default: events.json\n"
            << "  -h/--help               show this help message"
            << std::endl;
}

int main(int argc, char** argv) {
  std::string inFile;
  std::string outFile;

  const char* const short_opts = "i:o:h";
  const option long_opts[] = {
    {"in-file", required_argument, nullptr, 'i'},
    {"out-file", required_argument, nullptr, 'o'},
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

  if (outFile.empty()) {
    outFile = "events.json";
    std::cout << "INFO: Using default output file path:\n"
              << "      " << outFile << std::endl;
  }

  read_events<podio::ROOTReader>(inFile, outFile);

  return EXIT_SUCCESS;
}
