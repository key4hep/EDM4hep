import importlib
import ROOT
res = ROOT.gSystem.Load('libedm4hepDict.so')
if res < 0:
    raise RuntimeError('Failed to load libedm4hepDict.so')


class Datamodel:
    """
    Factory class to dynamically serve EDM4hep classes
    """
    def __init__(self):
        self.edm4hep = importlib.import_module('ROOT').edm4hep

    def __getattr__(self, name):
        try:
            return getattr(self.edm4hep, name)
        except AttributeError:
            raise AttributeError(f'EDM4hep has no class named {name}')
