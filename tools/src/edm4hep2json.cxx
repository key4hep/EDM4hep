#include "podio/FrameCategories.h"

// EDM4hep
#include "edm4hep2json.hxx"

// std
#include <filesystem>

// *nix
#include <getopt.h>

#include "TFile.h"

void printHelp() {
  std::cout << "Usage: edm4hep2json [olenfvh] FILEPATH\n"
            << "  -o/--out-file           output file path\n"
            << "                            default: \"?edm4hep.root\" --> \".edm4hep.json\"\n"
            << "  -l/--coll-list          comma separated list of collections "
               "to be converted\n"
            << "  -e/--events             comma separated list of events to "
               "be processed\n"
            << "  -n/--nevents            maximal number of events to be "
               "processed\n"
            << "  -f/--frame-name         input frame name\n"
            << "                            default: \"events\"\n"
            << "  -v/--verbose            be more verbose\n"
            << "  -h/--help               show this help message" << std::endl;
}

int main(int argc, char** argv) {
  std::filesystem::path inFilePath;
  std::filesystem::path outFilePath;
  std::string requestedCollections;
  std::string requestedEvents;
  std::string frameName = podio::Category::Event;
  bool verboser = false;
  int nEventsMax = -1;

  const char* const short_opts = "o:l:e:n:f:vh";
  const option long_opts[] = {{"out-file", required_argument, nullptr, 'o'},
                              {"coll-list", required_argument, nullptr, 'l'},
                              {"events", required_argument, nullptr, 'e'},
                              {"nevents", required_argument, nullptr, 'n'},
                              {"frame-name", required_argument, nullptr, 'f'},
                              {"verbose", no_argument, nullptr, 'v'},
                              {"help", no_argument, nullptr, 'h'},
                              {nullptr, no_argument, nullptr, 0}};

  while (true) {
    const auto opt = getopt_long(argc, argv, short_opts, long_opts, 0);

    if (opt == -1) {
      break;
    }

    switch (opt) {
    case 'i':
      inFilePath = std::filesystem::path(optarg);
      break;
    case 'o':
      outFilePath = std::filesystem::path(optarg);
      break;
    case 'l':
      requestedCollections = std::string(optarg);
      break;
    case 'e':
      requestedEvents = std::string(optarg);
      break;
    case 'f':
      frameName = std::string(optarg);
      break;
    case 'n':
      try {
        nEventsMax = std::stoi(optarg);
      } catch (...) {
        if (verboser) {
          std::cout << "ERROR: Provided maximal number of events is "
                       "not a number:\n"
                    << "       " << optarg << "\n";
        }
        return EXIT_FAILURE;
      }
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
    inFilePath = std::string(argv[i]);
  }

  if (inFilePath.empty()) {
    std::cerr << "ERROR: Input .root file not provided!" << std::endl;
    return EXIT_FAILURE;
  }

  if (!std::filesystem::exists(inFilePath)) {
    std::cerr << "ERROR: Input .root file can't be read!\n       " << inFilePath << std::endl;
    return EXIT_FAILURE;
  }

  if (outFilePath.empty()) {
    std::string outFileStr = inFilePath.stem().string();
    if (outFileStr.find(".edm4hep") != std::string::npos) {
      outFileStr = outFileStr.erase(outFileStr.find(".edm4hep"), 8);
    }
    if (outFileStr.find("_edm4hep") != std::string::npos) {
      outFileStr = outFileStr.erase(outFileStr.find("_edm4hep"), 8);
    }
    outFilePath = std::filesystem::path(outFileStr + ".edm4hep.json");
  }

  {
    std::unique_ptr<TFile> inFile(TFile::Open(inFilePath.c_str(), "READ"));
    if (!inFile->GetListOfKeys()->FindObject("podio_metadata")) {
      std::cout << "ERROR: Reading file produced with an incompatible version of EDM4hep. Aborting..." << std::endl;
      return 1;
    }
    inFile->Close();
  }

  return read_frames(inFilePath, outFilePath, requestedCollections, requestedEvents, frameName, nEventsMax, verboser);
}
