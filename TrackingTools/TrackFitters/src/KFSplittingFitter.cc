#include "TrackingTools/TrackFitters/interface/KFSplittingFitter.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateWithArbitraryError.h"
#include "TrackingTools/TrackFitters/interface/RecHitSplitter.h"
#include "TrackingTools/TrackFitters/interface/RecHitSorter.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
//#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
// #include "CommonDet/BasicDet/interface/InvalidRecHit.h"
// #include "CommonDet/BasicDet/interface/Det.h"
// #include "CommonDet/BasicDet/interface/DetUnit.h"
// #include "CommonDet/DetLayout/interface/DetLayer.h"

vector<Trajectory> KFSplittingFitter::fit(const Trajectory& aTraj) const {

  typedef RecHitSplitter::RecHitContainer        RecHitContainer;

  if(aTraj.empty()) return vector<Trajectory>();
  
  TM firstTM = aTraj.firstMeasurement();
  TSOS firstTsos = 
    TrajectoryStateWithArbitraryError()(firstTM.predictedState());
  
  RecHitContainer hits = aTraj.recHits();
  RecHitContainer result; 
  result.reserve(hits.size());
  for(RecHitContainer::iterator ihit = hits.begin(); ihit != hits.end();
      ihit++) {
    if(!(*ihit).isValid()) result.push_back(ihit.get());
    else if((*ihit).transientHits().size() == 1) result.push_back(ihit.get());
    else {
      RecHitContainer splitted = RecHitSplitter().split((*ihit).transientHits());
      RecHitContainer sorted = 
	RecHitSorter().sortHits(splitted, propagator()->propagationDirection());
      for (RecHitContainer::iterator srt = sorted.begin(); srt != sorted.end(); srt++) {
	result.push_back(srt.get());
	//      result.insert(result.end(), sorted.begin(), sorted.end());
      }
    }
  }
  

  return KFTrajectoryFitter::fit(aTraj.seed(), result, firstTsos);
  
}

vector<Trajectory> KFSplittingFitter::fit(const TrajectorySeed& aSeed,
					  const RecHitContainer& hits, 
					  const TSOS& firstPredTsos) const {

  RecHitContainer result;
  result.reserve(hits.size());
  for(RecHitContainer::const_iterator ihit = hits.begin(); ihit != hits.end();
      ihit++) {
    if(!(*ihit).isValid()) result.push_back(ihit->clone());
    else if((*ihit).transientHits().size() == 1) result.push_back(ihit->clone());
    else {      
      RecHitContainer splitted = RecHitSplitter().split(ihit->clone()->transientHits());
      RecHitContainer sorted = 
	RecHitSorter().sortHits(splitted, propagator()->propagationDirection());
      for (RecHitContainer::iterator srt = sorted.begin(); srt != sorted.end(); srt++) {
	result.push_back(srt.get());
	//      result.insert(result.end(), sorted.begin(), sorted.end());
      }
    }
  }
  
  return KFTrajectoryFitter::fit(aSeed, result, firstPredTsos);
  
}

