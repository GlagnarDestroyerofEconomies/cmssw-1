#ifndef TkDetLayers_TOBLayer_h
#define TkDetLayers_TOBLayer_h


#include "TrackingTools/DetLayers/interface/RodBarrelLayer.h"
#include "RecoTracker/TkDetLayers/interface/TOBRod.h"
#include "Utilities/BinningTools/interface/PeriodicBinFinderInPhi.h"
#include "RecoTracker/TkDetLayers/interface/SubLayerCrossings.h"

/** A concrete implementation for TOB layer 
 *  built out of TOBRods
 */

class TOBLayer : public RodBarrelLayer{
 public:
  typedef PeriodicBinFinderInPhi<double>   BinFinderType;


  TOBLayer(std::vector<const TOBRod*>& innerRods,
	   std::vector<const TOBRod*>& outerRods);
  ~TOBLayer();
  
  // GeometricSearchDet interface
  
  virtual const std::vector<const GeomDet*>& basicComponents() const {return theBasicComps;}

  virtual const std::vector<const GeometricSearchDet*>& components() const {return theComps;}

  
  virtual std::vector<DetWithState> 
  compatibleDets( const TrajectoryStateOnSurface& startingState,
		  const Propagator& prop, 
		  const MeasurementEstimator& est) const;

  virtual std::vector<DetGroup> 
  groupedCompatibleDets( const TrajectoryStateOnSurface& startingState,
			 const Propagator& prop,
			 const MeasurementEstimator& est) const;


  virtual bool hasGroups() const {return true;}  

  // DetLayer interface
  virtual Module   module()   const { return silicon;}
  
 

 private:
  // private methods for the implementation of groupedCompatibleDets()

  SubLayerCrossings computeCrossings( const TrajectoryStateOnSurface& tsos,
				      PropagationDirection propDir) const;
  
  bool addClosest( const TrajectoryStateOnSurface& tsos,
		   const Propagator& prop,
		   const MeasurementEstimator& est,
		   const SubLayerCrossing& crossing,
		   std::vector<DetGroup>& result) const;

  float computeWindowSize( const GeomDet* det, 
			   const TrajectoryStateOnSurface& tsos, 
			   const MeasurementEstimator& est) const;
  
  double calculatePhiWindow( double Xmax, const GeomDet& det,
			     const TrajectoryStateOnSurface& state) const;

  bool overlap( const GlobalPoint& gpos, const GeometricSearchDet& rod, float phiWin) const;


  void searchNeighbors( const TrajectoryStateOnSurface& tsos,
			const Propagator& prop,
			const MeasurementEstimator& est,
			const SubLayerCrossing& crossing,
			float window, 
			std::vector<DetGroup>& result,
			bool checkClosest) const;

  const std::vector<const GeometricSearchDet*>& subLayer( int ind) const {
    return (ind==0 ? theInnerComps : theOuterComps);}
  
  BoundCylinder* cylinder( const std::vector<const GeometricSearchDet*>& rods) const ;


 private:
  std::vector<const GeometricSearchDet*> theComps;
  std::vector<const GeometricSearchDet*> theInnerComps;
  std::vector<const GeometricSearchDet*> theOuterComps;
  std::vector<const GeomDet*> theBasicComps;
  
  BinFinderType    theInnerBinFinder;
  BinFinderType    theOuterBinFinder;

  ReferenceCountingPointer<BoundCylinder>  theInnerCylinder;
  ReferenceCountingPointer<BoundCylinder>  theOuterCylinder;
    
};


#endif 
