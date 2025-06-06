"""Python module for the EDM4HEP datamodel and utilities."""

import sys

from .__version__ import __version__
import ROOT

if ROOT.gSystem.Load("libedm4hepDict") != 0:
    raise RuntimeError("Failed to load libedm4hepDict")

if ROOT.gSystem.Load("libedm4hepRDF") != 0:
    raise RuntimeError("Failed to load libedm4hepRDF")

if ROOT.gInterpreter.LoadFile("edm4hep/utils/kinematics.h") != 0:
    raise RuntimeError("Failed to load kinematics.h")

if ROOT.gInterpreter.LoadFile("edm4hep/utils/dataframe.h") != 0:
    raise RuntimeError("Failed to load dataframe.h")

if ROOT.gInterpreter.LoadFile("edm4hep/Constants.h") != 0:
    raise RuntimeError("Failed to load Constants.h")

if ROOT.gSystem.Load("libedm4hepOldSchemas") != 0:
    raise RuntimeError("Failed to load edm4hep legacy schemas library")

from ROOT import edm4hep  # noqa: E402

from podio.pythonizations import load_pythonizations  # noqa: E402

load_pythonizations("edm4hep")

# Make TAB completion work for utils
setattr(edm4hep, "utils", edm4hep.utils)

# set package attributes for edm4hep
edm4hep.__version__ = __version__
edm4hep.__name__ = __name__
edm4hep.__spec__ = __spec__
edm4hep.__path__ = __path__
edm4hep.__file__ = __file__

# Make `import edm4hep` work
sys.modules["edm4hep"] = edm4hep
