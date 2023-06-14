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

from ROOT import edm4hep


class Datamodel:
    """
    Factory class to dynamically serve EDM4hep classes
    """
    def __getattr__(self, name):
        try:
            return getattr(edm4hep, name)
        except AttributeError:
            raise AttributeError(f'EDM4hep has no class named {name}')


class Utils:
    """
    Factory class to dynamically serve EDM4hep::utils classes
    """
    def __init__(self):
        self.utils = edm4hep.utils

    def __getattr__(self, name):
        try:
            return getattr(self.utils, name)
        except AttributeError:
            raise AttributeError(f'ROOT has no class named {name}')
