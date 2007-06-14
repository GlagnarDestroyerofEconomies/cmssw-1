/** \file
 *
 *  $Date: 2007/03/07 13:20:54 $
 *  $Revision: 1.18 $
 *  \author Rick Wilkinson
 */

#include <RecoMuon/DetLayers/interface/MuRingForwardDoubleLayer.h>
#include <RecoMuon/DetLayers/interface/MuDetRing.h>
#include <Geometry/CommonDetUnit/interface/GeomDet.h>
#include <DataFormats/GeometrySurface/interface/SimpleDiskBounds.h>
#include <TrackingTools/GeomPropagators/interface/Propagator.h>
#include <TrackingTools/PatternTools/interface/MeasurementEstimator.h>

#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

MuRingForwardDoubleLayer::MuRingForwardDoubleLayer(const vector<const ForwardDetRing*>& frontRings,
                                       const vector<const ForwardDetRing*>& backRings) :

  theFrontLayer(frontRings),
  theBackLayer(backRings),
  theRings(frontRings), // add back later
  theComponents(),
  theBasicComponents()
{

  const std::string metname = "Muon|RecoMuon|RecoMuonDetLayers|MuRingForwardDoubleLayer";

  theRings.insert(theRings.end(), backRings.begin(), backRings.end());
  theComponents = std::vector <const GeometricSearchDet*>(theRings.begin(), theRings.end());

  // Cache chamber pointers (the basic components_)
  // and find extension in R and Z
  for (vector<const ForwardDetRing*>::const_iterator it=theRings.begin();
       it!=theRings.end(); it++) {
    vector<const GeomDet*> tmp2 = (*it)->basicComponents();
    theBasicComponents.insert(theBasicComponents.end(),tmp2.begin(),tmp2.end());
  }  
  
  setSurface(computeSurface());
   
  LogTrace(metname) << "Constructing MuRingForwardDoubleLayer: "
                    << basicComponents().size() << " Dets " 
                    << theRings.size() << " Rings "
                    << " Z: " << specificSurface().position().z()
                    << " R1: " << specificSurface().innerRadius()
                    << " R2: " << specificSurface().outerRadius();

  selfTest();
}


BoundDisk * MuRingForwardDoubleLayer::computeSurface() 
{
  const BoundDisk & frontDisk = theFrontLayer.specificSurface();
  const BoundDisk & backDisk  = theBackLayer.specificSurface();

  float rmin = min( frontDisk.innerRadius(), backDisk.innerRadius() );
  float rmax = max( frontDisk.outerRadius(), backDisk.outerRadius() );
  float zmin = frontDisk.position().z() - frontDisk.bounds().thickness()/2.;
  float zmax = backDisk.position().z() + backDisk.bounds().thickness()/2.;
  float zPos = (zmax+zmin)/2.;
  PositionType pos(0.,0.,zPos);
  RotationType rot;

  return new BoundDisk( pos, rot,
                        SimpleDiskBounds( rmin, rmax,
                                          zmin-zPos, zmax-zPos));
}


bool MuRingForwardDoubleLayer::frontIsCloserTo(const TrajectoryStateOnSurface&tsos) const
{
  // decide which face is closest, and do that one first.
  float frontz = theFrontLayer.specificSurface().position().z();
  float backz  = theBackLayer.specificSurface().position().z();
  float statez = tsos.globalPosition().z();

  return ( fabs(frontz-statez) < fabs(backz-statez) );
}

  

std::pair<bool, TrajectoryStateOnSurface>
MuRingForwardDoubleLayer::compatible( const TrajectoryStateOnSurface& startingState, const Propagator& prop,
                                      const MeasurementEstimator& est) const
{
  // mostly copied from ForwardDetLayer, except propagates to closest surface,
  // not to center
  const std::string metname = "Muon|RecoMuon|RecoMuonDetLayers|MuRingForwardDoubleLayer";

  bool frontIsCloser = frontIsCloserTo(startingState);
  const MuRingForwardLayer & closerLayer = (frontIsCloser) ? theFrontLayer : theBackLayer;

  //std::pair<bool, TrajectoryStateOnSurface> result 
  //  = closerLayer.compatible(startingState, prop, est);
  //if(!result.first)
  // {
  //  result = furtherLayer.compatible(startingState, prop, est);
  //}


  TrajectoryStateOnSurface myState = prop.propagate( startingState, closerLayer.specificSurface());
  if ( !myState.isValid()) return make_pair( false, myState);

  // take into account the thickness of the layer
  float deltaR = surface().bounds().thickness()/2. *
    fabs( tan( myState.localDirection().theta()));

  // take into account the error on the predicted state
  const float nSigma = 3.;
  if (myState.hasError()) {
    LocalError err = myState.localError().positionError();
    // ignore correlation for the moment...
    deltaR += nSigma * sqrt(err.xx() + err.yy());
  }

  float zPos = (zmax()+zmin())/2.;
  SimpleDiskBounds tmp( rmin()-deltaR, rmax()+deltaR,
                        zmin()-zPos, zmax()-zPos);

  return make_pair( tmp.inside(myState.localPosition()), myState);
}


vector<GeometricSearchDet::DetWithState> 
MuRingForwardDoubleLayer::compatibleDets(const TrajectoryStateOnSurface& startingState,
				   const Propagator& prop, 
				   const MeasurementEstimator& est) const {
  vector<DetWithState> result;
  const std::string metname = "Muon|RecoMuon|RecoMuonDetLayers|MuRingForwardDoubleLayer";
  pair<bool, TrajectoryStateOnSurface> compat =
    compatible(startingState, prop, est);

  if (!compat.first) {

    LogTrace(metname) << "     MuRingForwardDoubleLayer::compatibleDets: not compatible"
                      << " (should not have been selected!)";
    return result;
  }


  TrajectoryStateOnSurface& tsos = compat.second;

   // standard implementation of compatibleDets() for class which have 
  // groupedCompatibleDets implemented.
  // This code should be moved in a common place intead of being 
  // copied many times.
  vector<DetGroup> vectorGroups = groupedCompatibleDets(tsos,prop,est);
  for(vector<DetGroup>::const_iterator itDG=vectorGroups.begin();
      itDG!=vectorGroups.end();itDG++){
    for(vector<DetGroupElement>::const_iterator itDGE=itDG->begin();
        itDGE!=itDG->end();itDGE++){
      result.push_back(DetWithState(itDGE->det(),itDGE->trajectoryState()));
    }
  }
  return result;  
} 


vector<DetGroup>
MuRingForwardDoubleLayer::groupedCompatibleDets( const TrajectoryStateOnSurface& startingState,
                                           const Propagator& prop,
                                           const MeasurementEstimator& est) const {

  const std::string metname = "Muon|RecoMuon|RecoMuonDetLayers|MuRingForwardDoubleLayer";
  vector<GeometricSearchDet::DetWithState> detWithStates1, detWithStates2;
  
  if(frontIsCloserTo(startingState))
  {
    detWithStates1 = theFrontLayer.compatibleDets(startingState, prop, est);
    detWithStates2 = theBackLayer.compatibleDets(startingState, prop, est);    
  }
  else 
  {
    detWithStates1 = theBackLayer.compatibleDets(startingState, prop, est);
    detWithStates2 = theFrontLayer.compatibleDets(startingState, prop, est);
  }
 
  vector<DetGroup> result;
  if(!detWithStates1.empty()) result.push_back( DetGroup(detWithStates1) );
  if(!detWithStates2.empty()) result.push_back( DetGroup(detWithStates2) );
  LogTrace(metname) << "DoubleLayer Compatible dets: " << result.size();
  return result;
}


bool MuRingForwardDoubleLayer::isCrack(const GlobalPoint & gp) const
{
  const std::string metname = "Muon|RecoMuon|RecoMuonDetLayers|MuRingForwardDoubleLayer";
  // approximate
  bool result = false;
  double r = gp.perp();
  const std::vector<const ForwardDetRing*>& backRings = theBackLayer.rings(); 
  if(backRings.size() > 1)
  {
    const MuDetRing * innerRing = dynamic_cast<const MuDetRing *>(backRings[0]);
    const MuDetRing * outerRing = dynamic_cast<const MuDetRing *>(backRings[1]);
    assert(innerRing && outerRing);
    float crackInner = innerRing->specificSurface().outerRadius();
    float crackOuter = outerRing->specificSurface().innerRadius();
    LogTrace(metname)  << "In a crack:" << crackInner << " " << r << " " << crackOuter;
    if(r > crackInner && r < crackOuter) return true;
  }
  // non-overlapping rings
  //double phi = gp.phi().degrees();
  return result;
}


void MuRingForwardDoubleLayer::selfTest() const
{
  const std::vector<const GeomDet*>& frontDets = theFrontLayer.basicComponents();
  const std::vector<const GeomDet*>& backDets = theBackLayer.basicComponents();

  std::vector<const GeomDet*>::const_iterator frontItr = frontDets.begin(),
                                              lastFront = frontDets.end(),
                                              backItr = backDets.begin(),
                                              lastBack = backDets.end();

  // test that each front z is less than each back z
  for( ; frontItr != lastFront; ++frontItr)
  {
    float frontz = fabs( (**frontItr).surface().position().z() );
    for( ; backItr != lastBack; ++backItr)
    {
      float backz = fabs( (**backItr).surface().position().z() );
      assert(frontz < backz);
    }
  }
}


