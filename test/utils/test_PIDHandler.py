import unittest
from edm4hep import ReconstructedParticleCollection, ParticleIDCollection, ReconstructedParticle
import edm4hep
import cppyy


def create_recos():
    coll = ReconstructedParticleCollection()
    for i in range(1, 10):
        reco = coll.create()
        reco.setPDG(i)
    return coll


def create_particle_ids(recos, likelihood):
    coll = ParticleIDCollection()
    for reco in recos:
        pid = coll.create()
        pid.setLikelihood(likelihood)
        pid.setParticle(reco)
        pid.addToParameters(1.23)
        pid.addToParameters(3.14)
    return coll


class TestPIDHandler(unittest.TestCase):
    def test_pid_handler_basics(self):
        reco_coll = create_recos()
        pid_coll1 = create_particle_ids(reco_coll, 1.0)
        pid_coll2 = create_particle_ids(reco_coll, 2.0)

        handler = edm4hep.utils.PIDHandler()
        handler.addColl(pid_coll1)
        handler.addColl(pid_coll2)

        reco = reco_coll[0]
        pids = handler.getPIDs(reco)
        self.assertEqual(len(pids), 2)
        self.assertEqual(pids[0].getParticle(), reco)
        self.assertEqual(pids[1].getParticle(), reco)
        self.assertAlmostEqual(pids[0].getParameters()[0], 1.23, places=5)
        self.assertAlmostEqual(pids[0].getParameters()[1], 3.14, places=5)

        llh1 = pids[0].getLikelihood()
        llh2 = pids[1].getLikelihood()
        if llh1 == 1.0:
            self.assertAlmostEqual(llh2, 2.0, places=5)
        else:
            self.assertAlmostEqual(llh2, 1.0, places=5)

        unknown_reco = ReconstructedParticle()
        pids = handler.getPIDs(unknown_reco)
        self.assertEqual(len(pids), 0)

    def test_pid_handler_with_addMetaInfo(self):
        handler = edm4hep.utils.PIDHandler()
        reco_coll = create_recos()
        pid_coll1 = create_particle_ids(reco_coll, 1.0)
        for pid in pid_coll1:
            pid.setAlgorithmType(42)
        pid_info1 = edm4hep.utils.ParticleIDMeta("fancyAlgo", 42, ["p1", "p2"])
        handler.addColl(pid_coll1, pid_info1)

        self.assertEqual(handler.getAlgoType("fancyAlgo").value_or(0), 42)
        self.assertEqual(handler.getParamIndex(42, "p2").value_or(-1), 1)
        retrieved_pid = handler.getPID(reco_coll[0], 42)
        self.assertEqual(retrieved_pid.value(), pid_coll1[0])

        # Technically, we can even just add meta data without having a corresponding
        # ParticleID collection to match
        handler.addMetaInfo(edm4hep.utils.ParticleIDMeta("anotherAlgo", 123, []))
        self.assertEqual(handler.getAlgoType("anotherAlgo").value(), 123)

        # Expected exceptions also get thrown
        with self.assertRaises(cppyy.gbl.std.runtime_error):
            handler.addMetaInfo(edm4hep.utils.ParticleIDMeta("anotherAlgo", 321, ["param"]))
        # No information about this meta data can be obtained
        self.assertFalse(handler.getParamIndex(321, "param").has_value())

        with self.assertRaises(cppyy.gbl.std.runtime_error):
            handler.addMetaInfo(edm4hep.utils.ParticleIDMeta("newAlgo", 42, ["PARAM"]))
        # Existing meta info is unchanged
        self.assertFalse(handler.getParamIndex(42, "PARAM").has_value())
        self.assertEqual(handler.getParamIndex(42, "p2").value_or(-1), 1)
