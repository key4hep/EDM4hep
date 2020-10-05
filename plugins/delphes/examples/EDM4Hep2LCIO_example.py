'''
Created on October 1st, 2020

Example for creating LCIO events and filling them with tracks from EDM4Hep

@author: <a href="mailto:clement.helsens@cern.ch">Clement Helsens</a>
'''

from pyLCIO import EVENT, IMPL, IOIMPL, UTIL
from time import time
import ROOT as r
import sys, math
#from sixlcio.moves import range

def generateEvents(inputFileName, outputFileName, nEvents):
    
    fin = r.TFile(inputFileName)
    ttree = fin.events

    # define a detector with positions for the tracker planes
    detectorName = 'ToyTracker'
    
    # create a writer and open the output file
    writer = IOIMPL.LCFactory.getInstance().createLCWriter()
    writer.open( outputFileName, EVENT.LCIO.WRITE_NEW )
    
    # create a run header and add it to the file (optional)
    run = IMPL.LCRunHeaderImpl()
    run.setRunNumber( 1 )
    run.setDetectorName(str(detectorName))
    run.setDescription( str('This is a test run' ))
    writer.writeRunHeader( run )
    iEvent=0


    for e in ttree:
        if iEvent>nEvents:break
        # create an event and set its parameters
        event = IMPL.LCEventImpl()
        event.setEventNumber( iEvent )
        event.setDetectorName( str(detectorName ))
        event.setRunNumber( 1 )
        event.setTimeStamp( int( time() * 1000000000. ) )

        tracks = IMPL.LCCollectionVec(EVENT.LCIO.TRACK)
        recops = IMPL.LCCollectionVec(EVENT.LCIO.RECONSTRUCTEDPARTICLE)

        #loop over the reconstructed particles
        for rp in range(e.ReconstructedParticles.size()):

            recp = IMPL.ReconstructedParticleImpl()
            recp.setCharge(e.ReconstructedParticles.at(rp).charge)
            momentum = r.TVector3(e.ReconstructedParticles.at(rp).momentum.x,
                                  e.ReconstructedParticles.at(rp).momentum.y,
                                  e.ReconstructedParticles.at(rp).momentum.z)
            recp.setMomentumVec(momentum)
            recp.setEnergy(e.ReconstructedParticles.at(rp).energy)

            #get the track associated to the reco particle
            track = IMPL.TrackImpl()

            if e.ReconstructedParticles.at(rp).tracks_begin<e.EFlowTrack_1.size():
            
                trkind=e.ReconstructedParticles.at(rp).tracks_begin
            
                track.setD0(e.EFlowTrack_1.at(trkind).D0)
                track.setPhi(e.EFlowTrack_1.at(trkind).phi)
                track.setOmega(e.EFlowTrack_1.at(trkind).omega)
                track.setZ0(e.EFlowTrack_1.at(trkind).Z0)
                track.setTanLambda(e.EFlowTrack_1.at(trkind).tanLambda)
                track.subdetectorHitNumbers().resize(50)

                #In EDM4Hep the covariance matrix is the upper triangle. In LCIO the bottom triangle. Only diagonals terms are filled because correlations are ignored.
                vec = r.std.vector('float')(15)
                vec[0]  = e.EFlowTrack_1.at(trkind).covMatrix[0]
                vec[2]  = e.EFlowTrack_1.at(trkind).covMatrix[5]
                vec[5]  = e.EFlowTrack_1.at(trkind).covMatrix[9]
                vec[9]  = e.EFlowTrack_1.at(trkind).covMatrix[12]
                vec[14] = e.EFlowTrack_1.at(trkind).covMatrix[14]
                
                track.setCovMatrix(vec)
            
                tracks.addElement(track)
            
                recp.addTrack(track)
                recops.addElement(recp)

        event.addCollection(tracks,EVENT.LCIO.TRACK)
        event.addCollection(recops,EVENT.LCIO.RECONSTRUCTEDPARTICLE)

        writer.writeEvent( event )
        iEvent+=1
    writer.flush()
    writer.close()
        

def usage():
    print('Generates an MCParticle with associated SimTrackerHits for each event')
    print('Usage:\n  python %s <inputFile> <outputFile> <nEvents>' % (sys.argv[0]))

if __name__ == '__main__':
    if len( sys.argv ) < 4:
        usage()
        sys.exit( 1 )
    generateEvents( sys.argv[1],sys.argv[2], int( sys.argv[3] ) )
