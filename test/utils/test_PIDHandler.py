import unittest
from edm4hep import ReconstructedParticleCollection, ParticleIDCollection, ReconstructedParticle
import edm4hep
import cppyy
import podio


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


def create_event_and_metadata():
    event = podio.Frame()
    metadata = podio.Frame()
    recos = event.put(create_recos(), "reco_particles")
    pid_coll1 = create_particle_ids(recos, 1.0)
    pid_coll2 = create_particle_ids(recos, 2.0)
    edm4hep.utils.PIDHandler.setAlgoInfo(
        metadata,
        pid_coll1,
        "particleIds_1",
        edm4hep.utils.ParticleIDMeta("pidAlgo_1", 42, ["first_param", "second_param"]),
    )
    edm4hep.utils.PIDHandler.setAlgoInfo(
        metadata,
        pid_coll2,
        "particleIds_2",
        edm4hep.utils.ParticleIDMeta("algo_2", 123, ["1", "2"]),
    )
    event.put(pid_coll1, "particleIds_1")
    event.put(pid_coll2, "particleIds_2")
    return event, metadata


class TestPIDHandler(unittest.TestCase):
    def check_handler_valid_reco(self, handler, reco):
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

    def test_PIDHandler_basics(self):
        reco_coll = create_recos()
        pid_coll1 = create_particle_ids(reco_coll, 1.0)
        pid_coll2 = create_particle_ids(reco_coll, 2.0)

        handler = edm4hep.utils.PIDHandler()
        handler.addColl(pid_coll1)
        handler.addColl(pid_coll2)

        reco = reco_coll[0]
        self.check_handler_valid_reco(handler, reco)

        unknown_reco = ReconstructedParticle()
        pids = handler.getPIDs(unknown_reco)
        self.assertEqual(len(pids), 0)

    def test_PIDHandler_from_collections(self):
        reco_coll = create_recos()
        pid_coll1 = create_particle_ids(reco_coll, 1.0)
        pid_coll2 = create_particle_ids(reco_coll, 2.0)

        handler = edm4hep.utils.PIDHandler.from_collections(pid_coll1, pid_coll2)

        reco = reco_coll[3]
        self.check_handler_valid_reco(handler, reco)

        unknown_reco = ReconstructedParticle()
        pids = handler.getPIDs(unknown_reco)
        self.assertEqual(len(pids), 0)

    def test_PIDHandler_with_addMetaInfo(self):
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
        self.assertIsNotNone(retrieved_pid)
        self.assertEqual(retrieved_pid, pid_coll1[0])

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

    def test_PIDHandler_from_frame_with_metadata(self):
        event, metadata = create_event_and_metadata()

        handler = edm4hep.utils.PIDHandler.from_frame(event, metadata)

        pid_algo1 = handler.getAlgoType("pidAlgo_1").value()
        pid_algo2 = handler.getAlgoType("algo_2").value()
        self.assertEqual(pid_algo1, 42)
        self.assertEqual(pid_algo2, 123)
        self.assertFalse(handler.getAlgoType("non-existant-algo").has_value())

        # Check that getting a ParticleID object for a reconstructed particle via the
        # algorithmType works
        recos = event.get("reco_particles")
        pid_coll1 = event.get("particleIds_1")
        pid_coll2 = event.get("particleIds_2")
        pid1 = handler.getPID(recos[0], pid_algo1)
        self.assertEqual(pid1.value(), pid_coll1[0])
        pid2 = handler.getPID(recos[0], pid_algo2)
        self.assertEqual(pid2.value(), pid_coll2[0])
        self.assertFalse(handler.getPID(recos[0], -1).has_value())

        # Check that parameter handling works as well
        par_index1 = handler.getParamIndex(pid_algo1, "first_param").value()
        self.assertEqual(par_index1, 0)
        par_index2 = handler.getParamIndex(pid_algo2, "2").value()
        self.assertEqual(par_index2, 1)
        # Valid algo but invalid parameter name
        self.assertFalse(handler.getParamIndex(pid_algo1, "non-existant-param").has_value())
        # Invalid algorithm, the parameter name is not even checked in this case
        self.assertFalse(handler.getParamIndex(-1, "doesn't matter").has_value())

        pid_info = edm4hep.utils.PIDHandler.getAlgoInfo(metadata, "particleIds_1").value()
        self.assertEqual(pid_info.algoName, "pidAlgo_1")
        self.assertEqual(pid_info.algoType(), 42)
        self.assertEqual(len(pid_info.paramNames), 2)
        self.assertEqual(pid_info.paramNames[0], "first_param")
        self.assertEqual(pid_info.paramNames[1], "second_param")

    def test_PIDHandler_from_frame_without_metadata(self):
        event, _ = create_event_and_metadata()

        handler = edm4hep.utils.PIDHandler.from_frame(event)
        # No metadata available info available in this case
        self.assertFalse(handler.getAlgoType("pidAlgo_1").has_value())

        # But the rest should still work as expected
        reco_coll = event.get("reco_particles")

        reco = reco_coll[0]
        self.check_handler_valid_reco(handler, reco)

        unknown_reco = ReconstructedParticle()
        pids = handler.getPIDs(unknown_reco)
        self.assertEqual(len(pids), 0)
