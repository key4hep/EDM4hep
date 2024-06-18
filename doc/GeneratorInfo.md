# Dealing with Generator (meta-)data

EDM4hep provides data types and helper functions to handle the storage of generator meta data, both at run and event level.

## Storing and retrieving run level information
At run level, information about generator tools (name, version, description) are being stored. They can be written via


```cpp
    #include "edm4hep/GeneratorToolInfo.h"
    // ...
    // write some generator tool info into the run
    auto toolInfo = edm4hep::GeneratorToolInfo();
    auto toolInfos = std::vector<edm4hep::GeneratorToolInfo>();
    toolInfo.name = "something";
    toolInfo.version = "v1";
    toolInfo.description = "some tool";
    toolInfos.emplace_back(std::move(toolInfo));

    edm4hep::utils::putGenToolInfos(run, toolInfos);
```

and read-back via:

```cpp
  #include "edm4hep/GeneratorToolInfo.h"
  // ...
  auto toolInfos = edm4hep::utils::getGenToolInfos(run);

```

### Storing and retrieving event specific generator parameters and PDF information

For storing information about event level parameters of generators one can use the type `GeneratorEventParameters`.
For storing information about PDFs, one can use the type `GeneratorPdfInfo`.
