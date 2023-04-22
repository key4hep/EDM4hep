import importlib
import ROOT
ROOT.gSystem.Load('libedm4hepDict.so')


class Datamodel:
    def __init__(self):
        self.edm4hep = importlib.import_module('ROOT').edm4hep

    def __getattr__(self, name):
        try:
            return getattr(self.edm4hep, name)
        except AttributeError:
            raise AttributeError(f'EDM4hep has no class named {name}')
