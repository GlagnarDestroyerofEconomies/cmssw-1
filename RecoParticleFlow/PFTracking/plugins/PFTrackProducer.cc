#include <memory>
#include "RecoParticleFlow/PFTracking/interface/PFTrackProducer.h"
#include "RecoParticleFlow/PFTracking/interface/PFTrackTransformer.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/EgammaReco/interface/ElectronSeed.h"
#include "DataFormats/EgammaReco/interface/ElectronSeedFwd.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"


using namespace std;
using namespace edm;
using namespace reco;
PFTrackProducer::PFTrackProducer(const ParameterSet& iConfig):
  pfTransformer_(0)
{
  produces<reco::PFRecTrackCollection>();

  tracksContainers_ = 
    iConfig.getParameter< vector < InputTag > >("TkColList");

  useQuality_   = iConfig.getParameter<bool>("UseQuality");

  gsfTrackLabel_ = iConfig.getParameter<InputTag>
    ("GsfTrackModuleLabel");  

  trackQuality_=reco::TrackBase::qualityByName(iConfig.getParameter<std::string>("TrackQuality"));
  
  muonColl_ = iConfig.getParameter< InputTag >("MuColl");
  
  trajinev_ = iConfig.getParameter<bool>("TrajInEvents");

  gsfinev_ = iConfig.getParameter<bool>("GsfTracksInEvents");

}

PFTrackProducer::~PFTrackProducer()
{
  delete pfTransformer_;
}

void
PFTrackProducer::produce(Event& iEvent, const EventSetup& iSetup)
{

  //create the empty collections 
  auto_ptr< reco::PFRecTrackCollection > 
    PfTrColl (new reco::PFRecTrackCollection);
  
  //read track collection
  Handle<GsfTrackCollection> gsftrackcoll;
  bool foundgsf = iEvent.getByLabel(gsfTrackLabel_,gsftrackcoll);
  GsfTrackCollection gsftracks;
  if(gsfinev_) {
    if(!foundgsf )
      LogError("PFTrackProducer")
	<<" cannot get GsfTracks (probably in HI events): "
	<< " please set GsfTracksInEvents = False in RecoParticleFlow/PFTracking/python/pfTrack_cfi.py" << endl;
    else
      gsftracks  = *(gsftrackcoll.product());
  }  

  // read muon collection
  Handle< reco::MuonCollection > recMuons;
  iEvent.getByLabel(muonColl_, recMuons);
	   

  for (unsigned int istr=0; istr<tracksContainers_.size();istr++){
    
    //Track collection
    Handle<reco::TrackCollection> tkRefCollection;
    iEvent.getByLabel(tracksContainers_[istr], tkRefCollection);
    reco::TrackCollection  Tk=*(tkRefCollection.product());
    
    vector<Trajectory> Tj(0);
    if(trajinev_) {
      //Trajectory collection
      Handle<vector<Trajectory> > tjCollection;
      bool found = iEvent.getByLabel(tracksContainers_[istr], tjCollection);
      if(!found )
	LogError("PFTrackProducer")
	  <<" cannot get Trajectories of: "
	  <<  tracksContainers_[istr]
	  << " please set TrajInEvents = False in RecoParticleFlow/PFTracking/python/pfTrack_cfi.py" << endl;
      
      Tj =*(tjCollection.product());
    }


    for(unsigned int i=0;i<Tk.size();i++){

      reco::TrackRef trackRef(tkRefCollection, i);
       
      if (useQuality_ &&
	  (!(Tk[i].quality(trackQuality_)))){
	
	bool isMuCandidate = false;

	//TrackRef trackRef(tkRefCollection, i);
	
	if(recMuons.isValid() ) {
	  for(unsigned j=0;j<recMuons->size(); j++) {
	    reco::MuonRef muonref( recMuons, j );
	    if (muonref->track().isNonnull()) 
	      if( muonref->track() == trackRef && muonref->isGlobalMuon()){
		isMuCandidate=true;
		//cout<<" SAVING TRACK "<<endl;
		break;
	      }
	  }
	}

	if(!isMuCandidate){
	  continue;	  
	}
	
      }

     
      // find the pre-id kf track
      bool preId = false;
      if(foundgsf) {
	for (unsigned int igsf=0; igsf<gsftracks.size();igsf++) {
	  GsfTrackRef gsfTrackRef(gsftrackcoll, igsf);
	  if (gsfTrackRef->seedRef().isNull()) continue;
	  ElectronSeedRef ElSeedRef= gsfTrackRef->extra()->seedRef().castTo<ElectronSeedRef>();
	  if (ElSeedRef->ctfTrack().isNonnull()) {
	    if(ElSeedRef->ctfTrack() == trackRef) preId = true;
	  }
	}
      }
      if(preId) {
	// Set PFRecTrack of type KF_ElCAND
	reco::PFRecTrack pftrack( trackRef->charge(), 
				  reco::PFRecTrack::KF_ELCAND, 
				  i, trackRef );

	bool valid = false;
	if(trajinev_) {
	  valid = pfTransformer_->addPoints( pftrack, *trackRef, Tj[i]);
	}
	else {
	  Trajectory FakeTraj;
	  valid = pfTransformer_->addPoints( pftrack, *trackRef, FakeTraj);
	}
	if(valid) {
	  PfTrColl->push_back(pftrack);
	}		

      }
      else {
	reco::PFRecTrack pftrack( trackRef->charge(), 
				  reco::PFRecTrack::KF, 
				  i, trackRef );
	bool valid = false;
	if(trajinev_) {
	  valid = pfTransformer_->addPoints( pftrack, *trackRef, Tj[i]);
	}
	else {
	  Trajectory FakeTraj;
	  valid = pfTransformer_->addPoints( pftrack, *trackRef, FakeTraj);
	}

	if(valid) {
	  PfTrColl->push_back(pftrack);
	}
      }
    }
  }
  iEvent.put(PfTrColl);
}

// ------------ method called once each job just before starting event loop  ------------
void 
PFTrackProducer::beginRun(edm::Run& run,
			  const EventSetup& iSetup)
{
  ESHandle<MagneticField> magneticField;
  iSetup.get<IdealMagneticFieldRecord>().get(magneticField);
  pfTransformer_= new PFTrackTransformer(math::XYZVector(magneticField->inTesla(GlobalPoint(0,0,0))));
  if(!trajinev_)
    pfTransformer_->OnlyProp();
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PFTrackProducer::endRun() {
  delete pfTransformer_;
}
