import ROOT
res = ROOT.gSystem.Load('libedm4hepDict.so')
if res < 0:
    raise RuntimeError('Failed to load libedm4hepDict.so')

kinematics_path = 'edm4hep/utils/kinematics.h'
# The location for tests and after installation is different relative
res = ROOT.gInterpreter.LoadFile(kinematics_path)
if res != 0:
    raise RuntimeError('Failed to load kinematics.h')

dataframe_path = 'edm4hep/utils/dataframe.h'
res = ROOT.gInterpreter.LoadFile(dataframe_path)
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
