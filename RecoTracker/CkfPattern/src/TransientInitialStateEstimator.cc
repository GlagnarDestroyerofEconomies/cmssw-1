#include "RecoTracker/CkfPattern/interface/TransientInitialStateEstimator.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "MagneticField/Engine/interface/MagneticField.h"

#include "TrackingTools/MaterialEffects/interface/PropagatorWithMaterial.h"
#include "TrackingTools/KalmanUpdators/interface/KFUpdator.h"
#include "TrackingTools/KalmanUpdators/interface/Chi2MeasurementEstimator.h"

#include "TrackingTools/TrackFitters/interface/KFTrajectoryFitter.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"

using namespace std;

TransientInitialStateEstimator::TransientInitialStateEstimator( const edm::EventSetup& es,
								const edm::ParameterSet& conf)
{
  thePropagatorAlongName    = conf.getParameter<std::string>("propagatorAlongTISE");   
  thePropagatorOppositeName = conf.getParameter<std::string>("propagatorOppositeTISE");   

  // let's avoid breaking compatibility now
  es.get<TrackingComponentsRecord>().get(thePropagatorAlongName,thePropagatorAlong);
  es.get<TrackingComponentsRecord>().get(thePropagatorOppositeName,thePropagatorOpposite);
}

void TransientInitialStateEstimator::setEventSetup( const edm::EventSetup& es ) {
  es.get<TrackingComponentsRecord>().get(thePropagatorAlongName,thePropagatorAlong);
  es.get<TrackingComponentsRecord>().get(thePropagatorOppositeName,thePropagatorOpposite);
}

std::pair<TrajectoryStateOnSurface, const GeomDet*> 
TransientInitialStateEstimator::innerState( const Trajectory& traj) const
{
  if(traj.firstMeasurement().forwardPredictedState().isValid()){    
    // The firstMeasurement fwd state is valid. Therefore the backward fitting has already been done
    // and we don't have to repeat it.
    TSOS firstState = traj.firstMeasurement().forwardPredictedState();
    firstState.rescaleError(100.);    
    return std::pair<TrajectoryStateOnSurface, const GeomDet*>( firstState, 
								traj.firstMeasurement().recHit()->det());
  }

  int nhits = traj.foundHits();
  //int lastFitted = 4;
  int lastFitted = nhits; //BM
  if (nhits-1 < lastFitted) lastFitted = nhits-1;

  std::vector<TrajectoryMeasurement> measvec = traj.measurements();
  TransientTrackingRecHit::ConstRecHitContainer firstHits;

  bool foundLast = false;
  int actualLast = -99;

  for (int i=lastFitted; i >= 0; i--) {
    if(measvec[i].recHit()->det()){
      if(!foundLast){
	actualLast = i; 
	foundLast = true;
      }
      firstHits.push_back( measvec[i].recHit());
    }
  }
  TSOS startingState = measvec[actualLast].updatedState();
  startingState.rescaleError(100.);

  KFTrajectoryFitter backFitter( *thePropagatorAlong,
				 KFUpdator(),
				 Chi2MeasurementEstimator( 100., 3),
				 firstHits.size());

  PropagationDirection backFitDirection = traj.direction() == alongMomentum ? oppositeToMomentum: alongMomentum;

  // only direction matters in this contest
  TrajectorySeed fakeSeed = TrajectorySeed(PTrajectoryStateOnDet() , 
					   edm::OwnVector<TrackingRecHit>(),
					   backFitDirection);

  vector<Trajectory> fitres = backFitter.fit( fakeSeed, firstHits, startingState);


  if (fitres.size() != 1) {
    // cout << "FitTester: first hits fit failed!" << endl;
    return std::pair<TrajectoryStateOnSurface, const GeomDet*>();
  }

  TrajectoryMeasurement firstMeas = fitres[0].lastMeasurement();
  TSOS firstState = firstMeas.updatedState();
  firstState.rescaleError(100.);
  
  return std::pair<TrajectoryStateOnSurface, const GeomDet*>( firstState, 
							      firstMeas.recHit()->det());
}

