#include "TrackingTools/GsfTools/interface/GsfPropagatorAdapter.h"

#include "TrackingTools/GsfTools/src/MultiStatePropagation.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

GsfPropagatorAdapter::GsfPropagatorAdapter (const Propagator& aPropagator) :
  Propagator(aPropagator.propagationDirection()),
  thePropagator(aPropagator.clone()) {}

std::pair<TrajectoryStateOnSurface,double> 
GsfPropagatorAdapter::propagateWithPath (const TrajectoryStateOnSurface& tsos, 
					 const Plane& plane) const {
  MultiStatePropagation<Plane> multiPropagator(*thePropagator);
  return multiPropagator.propagateWithPath(tsos,plane);
}

std::pair<TrajectoryStateOnSurface,double> 
GsfPropagatorAdapter::propagateWithPath (const TrajectoryStateOnSurface& tsos, 
					 const Cylinder& cylinder) const {
  MultiStatePropagation<Cylinder> multiPropagator(*thePropagator);
  return multiPropagator.propagateWithPath(tsos,cylinder);
}

std::pair<TrajectoryStateOnSurface,double> 
GsfPropagatorAdapter::propagateWithPath (const FreeTrajectoryState& fts, 
					 const Plane& plane) const {
  edm::LogInfo("GsfPropagatorAdapter") << "GsfPropagator used from FTS = single state mode!";
  return thePropagator->propagateWithPath(fts,plane);
}

std::pair<TrajectoryStateOnSurface,double> 
GsfPropagatorAdapter::propagateWithPath (const FreeTrajectoryState& fts, 
					 const Cylinder& cylinder) const {
  edm::LogInfo("GsfPropagatorAdapter") << "GsfPropagator used from FTS = single state mode!";
  return thePropagator->propagateWithPath(fts,cylinder);
}

void GsfPropagatorAdapter::setPropagationDirection (PropagationDirection dir) const {
  thePropagator->setPropagationDirection(dir);
  Propagator::setPropagationDirection(dir);
}
