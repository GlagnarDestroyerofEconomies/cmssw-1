// Package:    RecoTracker/SingleTrackPattern
// Class:      CosmicTrackFinder
// Original Author:  Michele Pioppi-INFN perugia
#include <memory>
#include <string>

#include "RecoTracker/SingleTrackPattern/interface/CosmicTrackFinder.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DMatchedLocalPosCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DLocalPosCollection.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "TrackingTools/PatternTools/interface/TSCPBuilderNoMaterial.h"
namespace cms
{

  CosmicTrackFinder::CosmicTrackFinder(edm::ParameterSet const& conf) : 
    cosmicTrajectoryBuilder_(conf) ,
    conf_(conf)
  {
    geometry=conf_.getUntrackedParameter<std::string>("GeometricStructure","STANDARD");
    produces<reco::TrackCollection>();
    produces<TrackingRecHitCollection>();
    produces<reco::TrackExtraCollection>();
  }


  // Virtual destructor needed.
  CosmicTrackFinder::~CosmicTrackFinder() { }  

  // Functions that gets called by framework every event
  void CosmicTrackFinder::produce(edm::Event& e, const edm::EventSetup& es)
  {

    std::string hitProducer = conf_.getParameter<std::string>("HitProducer");

  
    // retrieve seeds
    edm::Handle<TrajectorySeedCollection> seed;
    e.getByType(seed);
    //retrieve PixelRecHits
    edm::Handle<SiPixelRecHitCollection> pixelHits;
    if (geometry!="MTCC")  e.getByType(pixelHits);
    //retrieve StripRecHits
    edm::Handle<SiStripRecHit2DMatchedLocalPosCollection> matchedrecHits;
    e.getByLabel(hitProducer,"matchedRecHit" ,matchedrecHits);
    edm::Handle<SiStripRecHit2DLocalPosCollection> rphirecHits;
    e.getByLabel(hitProducer,"rphiRecHit" ,rphirecHits);
    edm::Handle<SiStripRecHit2DLocalPosCollection> stereorecHits;
    e.getByLabel(hitProducer,"stereoRecHit" ,stereorecHits);

    // Step B: create empty output collection
    std::auto_ptr<reco::TrackCollection> output(new reco::TrackCollection);
    std::auto_ptr<TrackingRecHitCollection> outputRHColl (new TrackingRecHitCollection);
    std::auto_ptr<reco::TrackExtraCollection> outputTEColl(new reco::TrackExtraCollection);


    edm::ESHandle<TrackerGeometry> tracker;
    es.get<TrackerDigiGeometryRecord>().get(tracker);

  
    if((*seed).size()>0){

      bool seedplus=((*(*seed).begin()).direction()==alongMomentum);
  
      if (seedplus)
	LogDebug("CosmicTrackFinder")<<"Reconstruction along momentum ";
      else
	LogDebug("CosmicTrackFinder")<<"Reconstruction opposite to momentum";
      cosmicTrajectoryBuilder_.init(es,seedplus);
      
      
      
      std::vector<Trajectory> trajoutput;
      
      cosmicTrajectoryBuilder_.run(*seed,
				   *stereorecHits,
				   *rphirecHits,
				   *matchedrecHits,
				   *pixelHits,
				   es,
				   e,
				   trajoutput);
   
      
      if(trajoutput.size()>0){
	//Trajectory from the algorithm
	const Trajectory  theTraj =(*trajoutput.begin());
	
	//RecHitCollection	
	const edm::OwnVector< const TransientTrackingRecHit>& transHits = theTraj.recHits();
	for(edm::OwnVector<const TransientTrackingRecHit>::const_iterator j=transHits.begin();
	    j!=transHits.end(); j++){
	  outputRHColl->push_back( ( (j->hit() )->clone()) );
	}

	edm::OrphanHandle <TrackingRecHitCollection> ohRH  = e.put( outputRHColl );


	TSOS UpState;
	if (seedplus)	  UpState=theTraj.lastMeasurement().updatedState();	
	else      UpState=theTraj.firstMeasurement().updatedState();


	//Track construction
	int ndof =theTraj.foundHits()-5;
	if (ndof<0) ndof=0;

	TSCPBuilderNoMaterial tscpBuilder;
	TrajectoryStateClosestToPoint tscp=tscpBuilder(*(UpState.freeState()),
						       UpState.globalPosition());
	reco::perigee::Parameters param = tscp.perigeeParameters();
  
	reco::perigee::Covariance covar = tscp.perigeeError();
  
	
	reco::Track theTrack(theTraj.chiSquared(),
			     int(ndof),
			     theTraj.foundHits(),
			     0,
			     theTraj.lostHits(),
			     param,
			     covar);


	//Track Extra
	GlobalPoint v=UpState.globalPosition();
	GlobalVector p=UpState.globalMomentum();
	math::XYZVector outmom( p.x(), p.y(), p.z() );
	math::XYZPoint  outpos( v.x(), v.y(), v.z() );   
	reco::TrackExtra *theTrackExtra = new reco::TrackExtra(outpos, outmom, true);
	for(edm::OwnVector<const TransientTrackingRecHit>::const_iterator j=transHits.begin();
	    j!=transHits.end(); j++){
	  theTrackExtra->add(TrackingRecHitRef(ohRH,0));
	}

	outputTEColl->push_back(*theTrackExtra);
	edm::OrphanHandle<reco::TrackExtraCollection> ohTE = e.put(outputTEColl);

	reco::TrackExtraRef  theTrackExtraRef(ohTE,0);
	theTrack.setExtra(theTrackExtraRef);
	output->push_back(theTrack);
	e.put(output);

      }

    }
  
  }
}
