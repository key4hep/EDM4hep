import ROOT
res = ROOT.gSystem.Load('libedm4hepDict.so')
if res < 0:
    raise RuntimeError('Failed to load libedm4hepDict.so')
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
