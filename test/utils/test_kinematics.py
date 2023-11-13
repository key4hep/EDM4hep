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
    p = edm4hep.MutableMCParticle()
    p.setMomentum(edm4hep.Vector3f(1.0, 2.0, 3.0))
    p.setMass(125.0)

    self.assertEqual(p4(p), LVM(1.0, 2.0, 3.0, 125.0))
    self.assertEqual(p4(p, SetEnergy(42.0)), LVE(1.0, 2.0, 3.0, 42.0))
    self.assertEqual(p4(p, SetMass(250.0)), LVM(1.0, 2.0, 3.0, 250.0))

    p = edm4hep.MutableReconstructedParticle()
    p.setMomentum(edm4hep.Vector3f(1.0, 2.0, 3.0))
    p.setMass(125.0)
    p.setEnergy(250.0)

    self.assertEqual(p4(p), LVM(1.0, 2.0, 3.0, 125.0))
    self.assertEqual(p4(p, UseEnergy), LVE(1.0, 2.0, 3.0, 250.0))
    self.assertEqual(p4(p, UseMass), LVM(1.0, 2.0, 3.0, 125.0))
    self.assertEqual(p4(p, SetMass()), LVM(1.0, 2.0, 3.0, 0.0))
    self.assertEqual(p4(p, SetEnergy(42.0)), LVE(1.0, 2.0, 3.0, 42.0))


if __name__ == '__main__':
  unittest.main()
