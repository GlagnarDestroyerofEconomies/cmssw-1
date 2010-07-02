#include "RecoParticleFlow/PFProducer/interface/PFMuonAlgo.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElement.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementGsfTrack.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

bool
PFMuonAlgo::isMuon( const reco::PFBlockElement& elt ) {

  const reco::PFBlockElementTrack* eltTrack 
    = dynamic_cast<const reco::PFBlockElementTrack*>(&elt);

  assert ( eltTrack );
  reco::MuonRef muonRef = eltTrack->muonRef();
  
  return isMuon(muonRef);

}

bool
PFMuonAlgo::isLooseMuon( const reco::PFBlockElement& elt ) {

  const reco::PFBlockElementTrack* eltTrack 
    = dynamic_cast<const reco::PFBlockElementTrack*>(&elt);

  assert ( eltTrack );
  reco::MuonRef muonRef = eltTrack->muonRef();

  return isLooseMuon(muonRef);

}

bool
PFMuonAlgo::isGlobalTightMuon( const reco::PFBlockElement& elt ) {

  const reco::PFBlockElementTrack* eltTrack 
    = dynamic_cast<const reco::PFBlockElementTrack*>(&elt);

  assert ( eltTrack );
  reco::MuonRef muonRef = eltTrack->muonRef();
  
  return isGlobalTightMuon(muonRef);

}

bool
PFMuonAlgo::isGlobalLooseMuon( const reco::PFBlockElement& elt ) {

  const reco::PFBlockElementTrack* eltTrack 
    = dynamic_cast<const reco::PFBlockElementTrack*>(&elt);

  assert ( eltTrack );
  reco::MuonRef muonRef = eltTrack->muonRef();

  return isGlobalLooseMuon(muonRef);

}

bool
PFMuonAlgo::isTrackerTightMuon( const reco::PFBlockElement& elt ) {

  const reco::PFBlockElementTrack* eltTrack 
    = dynamic_cast<const reco::PFBlockElementTrack*>(&elt);

  assert ( eltTrack );
  reco::MuonRef muonRef = eltTrack->muonRef();

  return isTrackerTightMuon(muonRef);

}

bool
PFMuonAlgo::isIsolatedMuon( const reco::PFBlockElement& elt ) {

  const reco::PFBlockElementTrack* eltTrack 
    = dynamic_cast<const reco::PFBlockElementTrack*>(&elt);

  assert ( eltTrack );
  reco::MuonRef muonRef = eltTrack->muonRef();

  return isIsolatedMuon(muonRef);

}


bool
PFMuonAlgo::isMuon(const reco::MuonRef& muonRef ){

  return isGlobalTightMuon(muonRef) || isTrackerTightMuon(muonRef) || isIsolatedMuon(muonRef);
}

bool
PFMuonAlgo::isLooseMuon(const reco::MuonRef& muonRef ){

  return isGlobalLooseMuon(muonRef) || isTrackerLooseMuon(muonRef);

}

bool
PFMuonAlgo::isGlobalTightMuon( const reco::MuonRef& muonRef ) {


  if ( !muonRef.isNonnull() ) return false;
  if ( !muonRef->isGlobalMuon() ) return false;
  // if ( !muonRef->isTrackerMuon() ) return false;
  if ( !muonRef->isStandAloneMuon() ) return false;

  reco::TrackRef standAloneMu = muonRef->standAloneMuon();
  reco::TrackRef combinedMu = muonRef->combinedMuon();
  reco::TrackRef trackerMu = muonRef->track();
 
  /*
  std::cout << " Global  Muon pt error " 
	    << combinedMu->ptError()/combinedMu->pt() << " " 
	    << combinedMu->pt() 
	    << std::endl; 
  std::cout << " Tracker Muon pt error " 
	    << trackerMu->ptError()/trackerMu->pt() << " " 
	    << trackerMu->pt() 
	    << std::endl;
  std::cout << " STAlone Muon pt error " 
	    << standAloneMu->ptError()/standAloneMu->pt() << " " 
	    << standAloneMu->pt() 
	    << std::endl; 
  */

  if ( muonRef->isTrackerMuon() ) { 
    

    
    bool result = muon::isGoodMuon(*muonRef,muon::GlobalMuonPromptTight);

    // Quality check on the hits in the muon chambers 
    // (at least two stations hit, or last station hit)


    bool isTM2DCompatibilityTight =  muon::isGoodMuon(*muonRef,muon::TM2DCompatibilityTight);

    const reco::Track& track = *trackerMu;
     bool quality = isTM2DCompatibilityTight || ((standAloneMu->hitPattern().numberOfValidMuonDTHits() + 2*standAloneMu->hitPattern().numberOfValidMuonCSCHits() > 12) && track.hitPattern().numberOfValidTrackerHits() > 12);


    return result && quality;

  } else {
    // No tracker muon -> Request a perfect stand-alone muon, or an even better global muon
    bool result = false;
      
    // Check the quality of the stand-alone muon : 
    // good chi**2 and large number of hits and good pt error
    if ( ( standAloneMu->hitPattern().numberOfValidMuonDTHits() < 22 &&
	   standAloneMu->hitPattern().numberOfValidMuonCSCHits() < 15 ) ||
	 standAloneMu->normalizedChi2() > 10. || 
	 standAloneMu->ptError()/standAloneMu->pt() > 0.20 ) {
      result = false;
    } else { 
      // If the stand-alone muon is good, check the global muon
      if ( combinedMu->normalizedChi2() > standAloneMu->normalizedChi2() ) {
	// If the combined muon is worse than the stand-alone, it 
	// means that either the corresponding tracker track was not 
	// reconstructed, or that the sta muon comes from a late 
	// pion decay (hence with a momentum smaller than the track)
	// Take the stand-alone muon only if its momentum is larger
	// than that of the track
	result = standAloneMu->pt() > trackerMu->pt() ;
      } else { 
	// If the combined muon is better (and good enough), take the 
	// global muon
	result = 
	  combinedMu->ptError()/combinedMu->pt() < 
	  std::min(0.20,standAloneMu->ptError()/standAloneMu->pt());
      }
    }      
    /*
    if ( result ) 
      std::cout << " Not a tracker muon !" << std::endl
		<< " pt (STA/TRA) : " << standAloneMu->pt() 
		<< " +/- " << standAloneMu->ptError()/standAloneMu->pt() 
		<< " and " << trackerMu->pt() 
		<< " +/- " << trackerMu->ptError()/trackerMu->pt() 
		<< " and " << combinedMu->pt() 
		<< " +/- " << combinedMu->ptError()/combinedMu->pt() 
		<< " eta : " << standAloneMu->eta() << std::endl
		<< " DT Hits : " << standAloneMu->hitPattern().numberOfValidMuonDTHits()
		<< "/" << standAloneMu->hitPattern().numberOfLostMuonDTHits()
		<< " CSC Hits : " << standAloneMu->hitPattern().numberOfValidMuonCSCHits()
		<< "/" << standAloneMu->hitPattern().numberOfLostMuonCSCHits()
		<< " RPC Hits : " << standAloneMu->hitPattern().numberOfValidMuonRPCHits()
		<< "/" << standAloneMu->hitPattern().numberOfLostMuonRPCHits() << std::endl
		<< " chi**2 STA : " << standAloneMu->normalizedChi2()
		<< " chi**2 GBL : " << combinedMu->normalizedChi2()
		<< std::endl 
		<< "TMLastStationLoose               "
		<< muon::isGoodMuon(*muonRef,muon::TMLastStationLoose) << std::endl       
		<< "TMLastStationTight               "
		<< muon::isGoodMuon(*muonRef,muon::TMLastStationTight) << std::endl    
		<< "TM2DCompatibilityLoose           "
		<< muon::isGoodMuon(*muonRef,muon::TM2DCompatibilityLoose) << std::endl 
		<< "TM2DCompatibilityTight           "
		<< muon::isGoodMuon(*muonRef,muon::TM2DCompatibilityTight) << std::endl
		<< "TMOneStationLoose                "
		<< muon::isGoodMuon(*muonRef,muon::TMOneStationLoose) << std::endl       
		<< "TMOneStationTight                "
		<< muon::isGoodMuon(*muonRef,muon::TMOneStationTight) << std::endl       
		<< "TMLastStationOptimizedLowPtLoose " 
		<< muon::isGoodMuon(*muonRef,muon::TMLastStationOptimizedLowPtLoose) << std::endl
		<< "TMLastStationOptimizedLowPtTight " 
		<< muon::isGoodMuon(*muonRef,muon::TMLastStationOptimizedLowPtTight) << std::endl 
		<< std::endl;
    */
    return result;    
  }

  return false;

}

bool
PFMuonAlgo::isTrackerTightMuon( const reco::MuonRef& muonRef ) {

  if ( !muonRef.isNonnull() ) return false;
  if(!muonRef->isTrackerMuon()) return false;


  reco::TrackRef trackerMu = muonRef->track();
  const reco::Track& track = *trackerMu;

  unsigned nTrackerHits =  track.hitPattern().numberOfValidTrackerHits();
  
  if(nTrackerHits<=12) return false;

  bool isAllArbitrated = muon::isGoodMuon(*muonRef,muon::AllArbitrated);
  
  bool isTM2DCompatibilityTight = muon::isGoodMuon(*muonRef,muon::TM2DCompatibilityTight);

  if(!isAllArbitrated || !isTM2DCompatibilityTight)  return false;

  if((trackerMu->ptError()/trackerMu->pt() > 0.10)){
    //std::cout<<" PT ERROR > 10 % "<< trackerMu->pt() <<std::endl;
    return false;
  }
  return true;
  
}

bool
PFMuonAlgo::isGlobalLooseMuon( const reco::MuonRef& muonRef ) {

  if ( !muonRef.isNonnull() ) return false;
  if ( !muonRef->isGlobalMuon() ) return false;
  //if ( !muonRef->isTrackerMuon() ) return false;
  if ( !muonRef->isStandAloneMuon() ) return false;
  
  reco::TrackRef standAloneMu = muonRef->standAloneMuon();
  reco::TrackRef combinedMu = muonRef->combinedMuon();
  reco::TrackRef trackerMu = muonRef->track();
 
  unsigned nMuonHits =
      standAloneMu->hitPattern().numberOfValidMuonDTHits() +
      2*standAloneMu->hitPattern().numberOfValidMuonCSCHits();
 
   bool quality = false;

  if ( muonRef->isTrackerMuon() ){

    
    bool onestation =
      muon::isGoodMuon(*muonRef,muon::TMOneStationTight);
    
    bool laststation =
      muon::isGoodMuon(*muonRef,muon::TMLastStationAngTight);
    
    
    const reco::Track& track = *trackerMu;
    unsigned nTrackerHits =  track.hitPattern().numberOfValidTrackerHits();
    
    
    // Lot's of STA (! Tracker) that maybe could be recovered
    //if(!muonRef->isTrackerMuon() && nmuonhits && !isMuon(muonRef)) std::cout<<" STALOOSE? "<<muonRef->pt()<<std::endl;
    
    //if(trackerMu->pt() < 20. || combinedMu->pt() < 20. || standAloneMu->pt() < 20.) return false;
    
    quality = ((onestation && nMuonHits > 12) || (laststation && nTrackerHits > 12))  && combinedMu->normalizedChi2() < 100.;
    
  }
  else{

    // Check the quality of the stand-alone muon : 
    // good chi**2 and large number of hits and good pt error
    if (  nMuonHits <=12  ||
	  standAloneMu->normalizedChi2() > 10. || 
	  standAloneMu->ptError()/standAloneMu->pt() > 0.20 ) {
      quality = false;
    } 
    else { 
      // If the stand-alone muon is good, check the global muon
      if ( combinedMu->normalizedChi2() > standAloneMu->normalizedChi2() ) {
	// If the combined muon is worse than the stand-alone, it 
	// means that either the corresponding tracker track was not 
	// reconstructed, or that the sta muon comes from a late 
	// pion decay (hence with a momentum smaller than the track)
	// Take the stand-alone muon only if its momentum is larger
	// than that of the track
	
	if(standAloneMu->pt() > trackerMu->pt() || combinedMu->normalizedChi2()<5.) quality =  true;
	
      } 
      else { 
	// If the combined muon is better (and good enough), take the 
	// global muon
	if(combinedMu->ptError()/combinedMu->pt() < std::min(0.20,standAloneMu->ptError()/standAloneMu->pt())) 
	  quality = true;
	
      }
    }      
    
    //std::cout<<" STA qual =  "<<quality<<" nMuonHits "<<nMuonHits<<" chi2 "<<standAloneMu->normalizedChi2()<<" sta err "<<standAloneMu->ptError()/standAloneMu->pt()<<" STA pt "<<standAloneMu->pt()<<" TR pt "<<trackerMu->pt()<<" combined pt "<<combinedMu->pt()<<std::endl;
  }
  

  return quality;

}


bool
PFMuonAlgo::isTrackerLooseMuon( const reco::MuonRef& muonRef ) {

  if ( !muonRef.isNonnull() ) return false;
  if(!muonRef->isTrackerMuon()) return false;

  reco::TrackRef trackerMu = muonRef->track();

  if(trackerMu->ptError()/trackerMu->pt() > 0.20) return false;

  // this doesn't seem to be necessary on the small samples looked at, but keep it around as insurance
  if(trackerMu->pt()>20.) return false;
    
  bool isAllArbitrated = muon::isGoodMuon(*muonRef,muon::AllArbitrated);
  bool isTMLastStationAngTight = muon::isGoodMuon(*muonRef,muon::TMLastStationAngTight);

  bool quality = isAllArbitrated && isTMLastStationAngTight;

  return quality;
  
}

bool
PFMuonAlgo::isIsolatedMuon( const reco::MuonRef& muonRef ){
  if ( !muonRef.isNonnull() ) return false;
  if ( !muonRef->isIsolationValid() ) return false;
  
  // Isolated Muons which are missed by standard cuts are nearly always global+tracker
  if ( !muonRef->isGlobalMuon() ) return false;

  // If it's not a tracker muon, only take it if there are valid muon hits

  reco::TrackRef standAloneMu = muonRef->standAloneMuon();

  if ( !muonRef->isTrackerMuon() ){
    if(standAloneMu->hitPattern().numberOfValidMuonDTHits() == 0 &&
       standAloneMu->hitPattern().numberOfValidMuonCSCHits() ==0) return false;
  }
  
  // for isolation, take the smallest pt available to reject fakes

  reco::TrackRef combinedMu = muonRef->combinedMuon();
  double smallestMuPt = combinedMu->pt();
  
  if(standAloneMu->pt()<smallestMuPt) smallestMuPt = standAloneMu->pt();
  
  if(muonRef->isTrackerMuon())
    {
      reco::TrackRef trackerMu = muonRef->track();
      if(trackerMu->pt() < smallestMuPt) smallestMuPt= trackerMu->pt();
    }
     
  double sumPtR03 = muonRef->isolationR03().sumPt;
  double emEtR03 = muonRef->isolationR03().emEt;
  double hadEtR03 = muonRef->isolationR03().hadEt;
  
  double relIso = (sumPtR03 + emEtR03 + hadEtR03)/smallestMuPt;
  //double relIso = sumPtR03/muonRef->pt();

  /*
  // protection against fake tracks -- only needed for tracker only, which isn't found in signal sample
  if(muonRef->isTrackerMuon() && !muonRef->isGlobalMuon() && !muonRef->isStandAloneMuon())
    {
      reco::TrackRef trackerMu = muonRef->track();
      
      if(trackerMu->ptError()/trackerMu->pt() > 0.20) return false;
    }
  //std::cout<<" relIso "<<relIso<<std::endl;
  */

  if(relIso<0.1) return true;
  else return false;
}

void 
PFMuonAlgo::printMuonProperties(const reco::MuonRef& muonRef){
  
  if ( !muonRef.isNonnull() ) return;
  
  bool isGL = muonRef->isGlobalMuon();
  bool isTR = muonRef->isTrackerMuon();
  bool isST = muonRef->isStandAloneMuon();

  std::cout<<" GL: "<<isGL<<" TR: "<<isTR<<" ST: "<<isST<<std::endl;
  
  if ( muonRef->isGlobalMuon() ){
    reco::TrackRef combinedMu = muonRef->combinedMuon();
    std::cout<<" GL,  pt: " << combinedMu->pt() 
	<< " +/- " << combinedMu->ptError()/combinedMu->pt() 
	     << " chi**2 GBL : " << combinedMu->normalizedChi2()<<std::endl;
    
  }
  if ( muonRef->isStandAloneMuon() ){
    reco::TrackRef standAloneMu = muonRef->standAloneMuon();
    std::cout<<" ST,  pt: " << standAloneMu->pt() 
	<< " +/- " << standAloneMu->ptError()/standAloneMu->pt() 
	<< " eta : " << standAloneMu->eta()  
	<< " DT Hits : " << standAloneMu->hitPattern().numberOfValidMuonDTHits()
	<< "/" << standAloneMu->hitPattern().numberOfLostMuonDTHits()
	<< " CSC Hits : " << standAloneMu->hitPattern().numberOfValidMuonCSCHits()
	<< "/" << standAloneMu->hitPattern().numberOfLostMuonCSCHits()
	<< " RPC Hits : " << standAloneMu->hitPattern().numberOfValidMuonRPCHits()
	<< "/" << standAloneMu->hitPattern().numberOfLostMuonRPCHits()
	     << " chi**2 STA : " << standAloneMu->normalizedChi2()<<std::endl;
      }


  if ( muonRef->isTrackerMuon() ){
    reco::TrackRef trackerMu = muonRef->track();
    const reco::Track& track = *trackerMu;
    std::cout<<" TR,  pt: " << trackerMu->pt() 
	<< " +/- " << trackerMu->ptError()/trackerMu->pt() 
	     << " chi**2 TR : " << trackerMu->normalizedChi2()<<std::endl;    
    std::cout<<" nTrackerHits "<<track.hitPattern().numberOfValidTrackerHits()<<std::endl;
    std::cout<< "TMLastStationAngLoose               "
	<< muon::isGoodMuon(*muonRef,muon::TMLastStationAngLoose) << std::endl       
	<< "TMLastStationAngTight               "
	<< muon::isGoodMuon(*muonRef,muon::TMLastStationAngTight) << std::endl          
	<< "TMLastStationLoose               "
	<< muon::isGoodMuon(*muonRef,muon::TMLastStationLoose) << std::endl       
	<< "TMLastStationTight               "
	<< muon::isGoodMuon(*muonRef,muon::TMLastStationTight) << std::endl          
	<< "TMOneStationLoose                "
	<< muon::isGoodMuon(*muonRef,muon::TMOneStationLoose) << std::endl       
	<< "TMOneStationTight                "
	<< muon::isGoodMuon(*muonRef,muon::TMOneStationTight) << std::endl       
	<< "TMLastStationOptimizedLowPtLoose " 
	<< muon::isGoodMuon(*muonRef,muon::TMLastStationOptimizedLowPtLoose) << std::endl
	<< "TMLastStationOptimizedLowPtTight " 
	<< muon::isGoodMuon(*muonRef,muon::TMLastStationOptimizedLowPtTight) << std::endl 
	<< "TMLastStationOptimizedBarrelLowPtLoose " 
	<< muon::isGoodMuon(*muonRef,muon::TMLastStationOptimizedBarrelLowPtLoose) << std::endl
	<< "TMLastStationOptimizedBarrelLowPtTight " 
	<< muon::isGoodMuon(*muonRef,muon::TMLastStationOptimizedBarrelLowPtTight) << std::endl 
	<< std::endl;

  }

  std::cout<< "TM2DCompatibilityLoose           "
      << muon::isGoodMuon(*muonRef,muon::TM2DCompatibilityLoose) << std::endl 
      << "TM2DCompatibilityTight           "
	   << muon::isGoodMuon(*muonRef,muon::TM2DCompatibilityTight) << std::endl;



  if (	    muonRef->isGlobalMuon() &&  muonRef->isTrackerMuon() &&  muonRef->isStandAloneMuon() ){
    reco::TrackRef combinedMu = muonRef->combinedMuon();
    reco::TrackRef trackerMu = muonRef->track();
    reco::TrackRef standAloneMu = muonRef->standAloneMuon();
    
    double sigmaCombined = combinedMu->ptError()/(combinedMu->pt()*combinedMu->pt()); 	 
    double sigmaTracker = trackerMu->ptError()/(trackerMu->pt()*trackerMu->pt()); 	 
    double sigmaStandAlone = standAloneMu->ptError()/(standAloneMu->pt()*standAloneMu->pt()); 	 
    
    bool combined = combinedMu->ptError()/combinedMu->pt() < 0.20; 	 
    bool tracker = trackerMu->ptError()/trackerMu->pt() < 0.20; 	 
    bool standAlone = standAloneMu->ptError()/standAloneMu->pt() < 0.20; 	 
  
    double delta1 =  combined && tracker ? 	 
      fabs(1./combinedMu->pt() -1./trackerMu->pt()) 	 
      /sqrt(sigmaCombined*sigmaCombined + sigmaTracker*sigmaTracker) : 100.; 	 
    double delta2 = combined && standAlone ? 	 
      fabs(1./combinedMu->pt() -1./standAloneMu->pt()) 	 
      /sqrt(sigmaCombined*sigmaCombined + sigmaStandAlone*sigmaStandAlone) : 100.; 	 
    double delta3 = standAlone && tracker ? 	 
      fabs(1./standAloneMu->pt() -1./trackerMu->pt()) 	 
      /sqrt(sigmaStandAlone*sigmaStandAlone + sigmaTracker*sigmaTracker) : 100.; 	 
    
    double delta = 	 
      standAloneMu->hitPattern().numberOfValidMuonDTHits()+ 	 
      standAloneMu->hitPattern().numberOfValidMuonCSCHits() > 0 ? 	 
      std::min(delta3,std::min(delta1,delta2)) : std::max(delta3,std::max(delta1,delta2)); 	 

    std::cout << "delta = " << delta << " delta1 "<<delta1<<" delta2 "<<delta2<<" delta3 "<<delta3<<std::endl; 	 
    
    double ratio = 	 
      combinedMu->ptError()/combinedMu->pt() 	 
      / (trackerMu->ptError()/trackerMu->pt()); 	 
    //if ( ratio > 2. && delta < 3. ) std::cout << "ALARM ! " << ratio << ", " << delta << std::endl;
    std::cout<<" ratio "<<ratio<<" combined mu pt "<<combinedMu->pt()<<std::endl;
    //bool quality3 =  ( combinedMu->pt() < 50. || ratio < 2. ) && delta <  3.;


  }

    double sumPtR03 = muonRef->isolationR03().sumPt;
    double emEtR03 = muonRef->isolationR03().emEt;
    double hadEtR03 = muonRef->isolationR03().hadEt;    
    double relIsoR03 = (sumPtR03 + emEtR03 + hadEtR03)/muonRef->pt();
    double sumPtR05 = muonRef->isolationR05().sumPt;
    double emEtR05 = muonRef->isolationR05().emEt;
    double hadEtR05 = muonRef->isolationR05().hadEt;    
    double relIsoR05 = (sumPtR05 + emEtR05 + hadEtR05)/muonRef->pt();
    std::cout<<" 0.3 Radion Rel Iso: "<<relIsoR03<<" sumPt "<<sumPtR03<<" emEt "<<emEtR03<<" hadEt "<<hadEtR03<<std::endl;
    std::cout<<" 0.5 Radion Rel Iso: "<<relIsoR05<<" sumPt "<<sumPtR05<<" emEt "<<emEtR05<<" hadEt "<<hadEtR05<<std::endl;
  return;

}
