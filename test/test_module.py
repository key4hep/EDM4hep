#!/usr/bin/env python3

"""Check the attributes of edm4hep package"""

import edm4hep
from pathlib import Path
from ROOT import gInterpreter

if gInterpreter.LoadFile("edm4hep/EDM4hepVersion.h") != 0:
    raise ImportError("Error while loading file edm4hep/EDM4hepVersion.h")

# __version__
version = edm4hep.version.build_version
assert edm4hep.__version__ == f"{version.major}.{version.minor}.{version.patch}"
# __name__
name = "edm4hep"
assert edm4hep.__name__ == name
# __file__
expected_origin = str(Path(__file__).parent.parent / "python" / "edm4hep" / "__init__.py")
assert edm4hep.__file__ == expected_origin
# __path__
expected_path = [str(Path(expected_origin).parent)]
assert edm4hep.__path__ == expected_path
# __spec__
assert edm4hep.__spec__.name == name
assert edm4hep.__spec__.origin == expected_origin
assert edm4hep.__spec__.submodule_search_locations == expected_path
# utils
assert "utils" in dir(edm4hep)
