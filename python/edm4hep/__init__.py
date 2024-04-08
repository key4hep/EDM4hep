"""Python module for the EDM4HEP datamodel and utilities."""
import sys

from .__version__ import __version__
import ROOT
res = ROOT.gSystem.Load('libedm4hepDict.so')
if res < 0:
    raise RuntimeError('Failed to load libedm4hepDict.so')

res = ROOT.gSystem.Load('libedm4hepRDF.so')
if res < 0:
    raise RuntimeError('Failed to load libedm4hepRDF.so')

res = ROOT.gInterpreter.LoadFile('edm4hep/utils/kinematics.h')
if res != 0:
    raise RuntimeError('Failed to load kinematics.h')

res = ROOT.gInterpreter.LoadFile('edm4hep/utils/dataframe.h')
if res != 0:
    raise RuntimeError('Failed to load dataframe.h')

res = ROOT.gInterpreter.LoadFile('edm4hep/Constants.h')
if res != 0:
    raise RuntimeError('Failed to load Constants.h')
from ROOT import edm4hep

# Make TAB completion work for utils
setattr(edm4hep, 'utils', edm4hep.utils)

# Make `import edm4hep` work
sys.modules['edm4hep'] = edm4hep
