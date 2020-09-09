#ifndef __CINT__ 
#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/RecoParticleRefCollection.h"
#include "edm4hep/ReconstructedParticleCollection.h"
#endif

#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

#include "TH1F.h"
#include "TLorentzVector.h"

/*
put this into your .rootlogon.C file
and make sure the (DY)LD_LIBRARY_PATH contains the podio and edm4hep installations
{
 gInterpreter->AddIncludePath("$PODIO/include");
 gSystem->Load("libpodio");
 gSystem->Load("libpodioDict");
 gSystem->Load("libpodioRootIO");
 gSystem->Load("libedm4hep");
 gSystem->Load("libedm4hepDict");
}
*/


template<class T>
double inv_mass(const T& p1, const T& p2){
  double e =  p1.getEnergy()     + p2.getEnergy() ;
  double px = p1.getMomentum()[0]+ p2.getMomentum()[0];
  double py = p1.getMomentum()[1]+ p2.getMomentum()[1];
  double pz = p1.getMomentum()[2]+ p2.getMomentum()[2];
  return( sqrt( e*e - px*px - py*py - pz*pz  ) );
}

template<class T>
TLorentzVector v4(const T& p){
  return TLorentzVector( p.getMomentum()[0],p.getMomentum()[1], p.getMomentum()[2],p.getEnergy());
}

/** Example script for creating invariant mass plots from a ee -> HZ-> X mu mu sample
 *  
 *  A suitable input file for the ILC can be found here: 
 *  http://osggridftp02.slac.stanford.edu:8080/sdf/group/lcddata/ilc/prod/ilc/mc-dbd/generated/250-TDR_ws/higgs/E250-TDR_ws.Pe2e2h.Gwhizard-1_95.eR.pL.I106480.001.stdhep
 *  
 *  Use this together with DelphesSTDHEP_EDM4HEP and the delphes card from https://github.com/iLCSoft/ILCDelphes.git.
 */
 
void higgs_recoil_plots(const char* FILEN) {

 int nEvents = 0  ;
 int maxEvt = 100000 ;  // change as needed

//---- create some histograms ----

 TH1F* hmuonmass = new TH1F("hmuonmass","inv. mass - muons", 100,  60. , 120. ) ;
 TH1F* hjetmass  = new TH1F("hjetmass","inv. mass - jets", 100, 0. , 150. ) ;
 TH1F* hrecoilm  = new TH1F("hrecoilm","recoil mass", 100, 110. , 170. ) ;

//---------------------------------
 
 auto reader = podio::ROOTReader();
 reader.openFile( FILEN );

 auto store = podio::EventStore();
 store.setReader(&reader);

  
 //==================== the event loop ============================================================
 const auto nEntries = reader.getEntries();
 for (int entry = 0; entry < nEntries && nEvents++ < maxEvt ; ++entry) {
   
   auto& muons = store.get<edm4hep::RecoParticleRefCollection>("Muon");
   auto& jets  = store.get<edm4hep::ReconstructedParticleCollection>("Jet");

   if( jets.size() != 2){
     store.clear();
     reader.endOfEvent();
     continue;
   }

   if( muons.size() != 2){
     store.clear();
     reader.endOfEvent();
     continue;
   }

   auto& mu1 = muons[0].getParticle(); 
   auto& mu2 = muons[1].getParticle(); 

   hmuonmass->Fill( inv_mass( mu1, mu2) ) ;


   auto j1 = jets[0]; 
   auto j2 = jets[1]; 

   hjetmass->Fill( inv_mass( j1, j2) ) ;
   

   // the recoil mass
   const auto& vm1 = v4(mu1) ;
   const auto& vm2 = v4(mu2) ;
   TLorentzVector ecms(0.,0.,0.,250.) ;
   TLorentzVector recoil = ecms - ( vm1 + vm2 ) ;
   hrecoilm->Fill( recoil.M() ) ;


   store.clear();
   reader.endOfEvent();

 }

 //===================================================================================================
 TCanvas* c1 = new TCanvas("recoil plots");

 c1->Divide(2,2);
 c1->cd(1) ;
 hmuonmass->Draw() ;
 c1->cd(2) ;
 hjetmass->Draw();
 c1->cd(3) ;
 hrecoilm->Draw();

  c1->Print("recoil_plots.pdf") ;
 //===================================================================================================

  std::cout << std::endl 
	    <<  "  " <<  nEvents 
	    << " events read from file: " 
	    << FILEN << std::endl  ;
  
  
}
