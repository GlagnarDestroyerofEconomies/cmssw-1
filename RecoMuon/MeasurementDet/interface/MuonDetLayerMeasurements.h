#ifndef MeasurementDet_MuonDetLayerMeasurements_H
#define MeasurementDet_MuonDetLayerMeasurements_H

/** \class MuonDetLayerMeasurements
 *  The class to access recHits and TrajectoryMeasurements from DetLayer.  
 *
 *  $Date: 2006/05/12 18:45:01 $
 *  $Revision: 1.1 $
 *  \author C. Liu - Purdue University
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/PatternTools/interface/MeasurementEstimator.h"

#include <vector>

class DetLayer;
class GeomDet;
class TrajectoryMeasurement;
class TransientTrackingRecHit;

typedef std::vector<TransientTrackingRecHit*>       RecHitContainer;
typedef std::vector<TrajectoryMeasurement>          MeasurementContainer;
typedef std::pair<const GeomDet*,TrajectoryStateOnSurface> DetWithState;


class MuonDetLayerMeasurements {
public:

   MuonDetLayerMeasurements();

   virtual ~MuonDetLayerMeasurements();

   /// obtain TrackingRecHits from a DetLayer
   RecHitContainer recHits(const DetLayer* layer, const edm::Event& iEvent) const;

   /// returns TMeasurements in a DetLayer compatible with the TSOS.
   MeasurementContainer
   measurements( const DetLayer* layer,
                 const TrajectoryStateOnSurface& startingState,
                 const Propagator& prop,
                 const MeasurementEstimator& est,
                 const edm::Event& iEvent) const;

 /// faster version in case the TrajectoryState on the surface of the GeomDet is already available
   MeasurementContainer
   fastMeasurements( const DetLayer* layer,
                     const TrajectoryStateOnSurface& theStateOnDet,
                     const TrajectoryStateOnSurface& startingState,
                     const Propagator& prop,
                     const MeasurementEstimator& est,
                     const edm::Event& iEvent) const;

   /// returns TMeasurements in a DetLayer compatible with the TSOS.
   MeasurementContainer
   measurements( const DetLayer* layer,
                 const TrajectoryStateOnSurface& startingState,
                 const Propagator& prop,
                 const MeasurementEstimator& est) const;

 /// faster version in case the TrajectoryState on the surface of the GeomDet is already available
   MeasurementContainer
   fastMeasurements( const DetLayer* layer,
                     const TrajectoryStateOnSurface& theStateOnDet,
                     const TrajectoryStateOnSurface& startingState,
                     const Propagator& prop,
                     const MeasurementEstimator& est) const;

 
  void setEvent(const edm::Event &);  

private:
   std::string  theDTRecHitLabel;
   std::string theCSCRecHitLabel;
   bool theEventFlag;
   const edm::Event* theEvent;   
};
#endif

