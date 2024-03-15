#!/usr/bin/env python3

import unittest
import cppyy

import edm4hep

# A few shorthands for slightly easier to read tests below
p4 = edm4hep.utils.p4
LVM = edm4hep.LorentzVectorM
LVE = edm4hep.LorentzVectorE
SetEnergy = edm4hep.utils.SetEnergy
SetMass = edm4hep.utils.SetMass
UseEnergy = edm4hep.utils.UseEnergy
UseMass = edm4hep.utils.UseMass


class TestKinematics(unittest.TestCase):
  """Tests to ensure that the kinematics header only library also works in
  python"""

  def test_p4(self):
    """Test the p4 functionality since that has shown to trip-up cppyy"""
    # podio doesn't expose the Mutable constructors so we have to use the full
    # glory of the immutable types here
    p = edm4hep.MCParticle(11,  # PDG
                           -1,  # generatorStatus
                           -1,  # simulatorStatus
                           1.0,  # charge
                           0.0,  # time
                           125.0,  # charge
                           edm4hep.Vector3d(0, 0, 0),  # vertex
                           edm4hep.Vector3d(0, 0, 0),  # endpoint
                           edm4hep.Vector3d(1.0, 2.0, 3.0),  # momentum
                           edm4hep.Vector3d(0, 0, 0),  # momentumAtEndpoint
                           edm4hep.Vector3f(0, 0, 0),  # spin
                           edm4hep.Vector2i(0, 0)  # colorFlow
                           )

    self.assertEqual(p4(p), LVM(1.0, 2.0, 3.0, 125.0))
    self.assertEqual(p4(p, SetEnergy(42.0)), LVE(1.0, 2.0, 3.0, 42.0))
    self.assertEqual(p4(p, SetMass(250.0)), LVM(1.0, 2.0, 3.0, 250.0))

    p = edm4hep.ReconstructedParticle(11,  # type
                                      250.0,  # energy
                                      edm4hep.Vector3f(1.0, 2.0, 3.0),  # momentum
                                      edm4hep.Vector3f(0, 0, 0),  # referencePoint
                                      1.0,  # charge
                                      125.0,  # mass
                                      0.0,  # goodnessOfPID
                                      edm4hep.CovMatrix4f() # covMatrix
                                      )

    self.assertEqual(p4(p), LVM(1.0, 2.0, 3.0, 125.0))
    self.assertEqual(p4(p, UseEnergy), LVE(1.0, 2.0, 3.0, 250.0))
    self.assertEqual(p4(p, UseMass), LVM(1.0, 2.0, 3.0, 125.0))
    self.assertEqual(p4(p, SetMass()), LVM(1.0, 2.0, 3.0, 0.0))
    self.assertEqual(p4(p, SetEnergy(42.0)), LVE(1.0, 2.0, 3.0, 42.0))


if __name__ == '__main__':
  unittest.main()
