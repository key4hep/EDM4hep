#!/usr/bin/env python3

"""Check the attributes of edm4hep package"""

import edm4hep
from ROOT import gInterpreter
from importlib.util import find_spec

if gInterpreter.LoadFile("edm4hep/EDM4hepVersion.h") != 0:
    raise ImportError("Error while loading file edm4hep/EDM4hepVersion.h")

# __version__
version = edm4hep.version.build_version
assert edm4hep.__version__ == f"{version.major}.{version.minor}.{version.patch}"

# import attributes
name = "edm4hep"
expected_spec = find_spec(name)
assert edm4hep.__name__ == name
assert edm4hep.__name__ == expected_spec.name
assert edm4hep.__spec__ == expected_spec
assert edm4hep.__path__ == expected_spec.submodule_search_locations
assert edm4hep.__file__ == expected_spec.origin

# utils
assert "utils" in dir(edm4hep)
