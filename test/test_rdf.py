#!/usr/bin/env python3

import ROOT
import edm4hep  # noqa: F401

ROOT.EnableImplicitMT()

print('Create RDataFrame ...')
df = ROOT.RDataFrame('events', 'edm4hep_events.root')

print('Apply selectors and define new branches ...')
df2 = (df.Define('MCParticles_pt', 'edm4hep::utils::pt(MCParticles)')
       .Define('MCParticles_eta', 'edm4hep::utils::eta(MCParticles)')
       .Define('MCParticles_cosTheta', 'edm4hep::utils::cos_theta(MCParticles)')
       .Define('SimTrackerHits_r', 'edm4hep::utils::r(SimTrackerHits)')
       .Define('SimTrackerHits_pt', 'edm4hep::utils::pt(SimTrackerHits)')
       .Define('TrackerHits_r', 'edm4hep::utils::r(TrackerHitPlanes)')
       )

filename = 'edm4hep_events_py_rdf.root'
print(f'Writing snapshot to disk ... \t{filename}')

df2.Snapshot('events', filename,
             ['MCParticles_pt',
              'MCParticles_eta',
              'MCParticles_cosTheta',
              'SimTrackerHits_r',
              'SimTrackerHits_pt',
              'TrackerHits_r'])
