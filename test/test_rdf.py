#!/usr/bin/env python3

import ROOT

# Load the implementations of the utility functions
ROOT.gSystem.Load('libedm4hepRDF.so')
# Load the declarations of the utility functions to make them available for JIT
# compilation
ROOT.gInterpreter.LoadFile('edm4hep/utils/dataframe.h')

ROOT.EnableImplicitMT()

print('Cerate RDataFrame ...')
df = ROOT.RDataFrame('events', 'edm4hep_events.root')

print('Apply selectors and define new branches ...')
df2 = (df.Define('MCParticles_pt', 'edm4hep::utils::pt(MCParticles)')
       .Define('MCParticles_eta', 'edm4hep::utils::eta(MCParticles)')
       .Define('MCParticles_cosTheta', 'edm4hep::utils::cos_theta(MCParticles)')
       .Define('SimTrackerHits_r', 'edm4hep::utils::r(SimTrackerHits)')
       .Define('SimTrackerHits_pt', 'edm4hep::utils::pt(SimTrackerHits)')
       .Define('TrackerHits_r', 'edm4hep::utils::r(TrackerHitPlanes)')
       .Define('MCParticle_p4', 'edm4hep::utils::p4M(MCParticles)')
       .Define('MCParticle_energy', 'edm4hep::utils::E(MCParticle_p4)')
       )

filename = 'edm4hep_events_py_rdf.root'
print(f'Writing snapshot to disk ... \t{filename}')

df2.Snapshot('events', filename,
             ['MCParticles_pt',
              'MCParticles_eta',
              'MCParticles_cosTheta',
              'SimTrackerHits_r',
              'SimTrackerHits_pt',
              'TrackerHits_r',
              'MCParticle_energy'])
