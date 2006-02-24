#include "TrackingTools/PatternTools/interface/TSCPBuilderNoMaterial.h"
#include "TrackingTools/AnalyticalJacobians/interface/AnalyticalCurvilinearJacobian.h"
#include "Geometry/Surface/interface/BoundPlane.h"
#include "TrackingTools/GeomPropagators/interface/HelixBarrelPlaneCrossingByCircle.h"
#include "TrackingTools/TrajectoryParametrization/interface/TrajectoryStateExceptions.h"

TrajectoryStateClosestToPoint 
TSCPBuilderNoMaterial::operator() (const FTS& originalFTS, 
    const GlobalPoint& referencePoint) const
{
  if (positionEqual(referencePoint, originalFTS.position()))
    return constructTSCP(originalFTS, referencePoint);

  // Now do the propagation or whatever...

  FreeTrajectoryState 
    ftsAtTransverseImpactPoint = createFTSatTransverseImpactPoint(originalFTS, referencePoint);
  return constructTSCP(ftsAtTransverseImpactPoint, referencePoint);
  
}

TrajectoryStateClosestToPoint 
TSCPBuilderNoMaterial::operator() (const TSOS& originalTSOS, 
    const GlobalPoint& referencePoint) const
{
  if (positionEqual(referencePoint, originalTSOS.globalPosition()))
    return constructTSCP(*originalTSOS.freeState(), referencePoint);

  // Now do the propagation
  
  FreeTrajectoryState ftsAtTransverseImpactPoint = 
    createFTSatTransverseImpactPoint(*originalTSOS.freeState(), referencePoint);
  return constructTSCP(ftsAtTransverseImpactPoint, referencePoint);
}

FreeTrajectoryState 
TSCPBuilderNoMaterial::createFTSatTransverseImpactPoint(
	const FTS& originalFTS, const GlobalPoint& referencePoint) const 
{
  //
  // Straight line approximation? |rho|<1.e-10 equivalent to ~ 1um 
  // difference in transversal position at 10m.
  //
  if( fabs(originalFTS.transverseCurvature())<1.e-10 ) {
    return createFTSatTransverseImpactPointNeutral(originalFTS, referencePoint);
  } else {
    return createFTSatTransverseImpactPointCharged(originalFTS, referencePoint);
  }
}

FreeTrajectoryState 
TSCPBuilderNoMaterial::createFTSatTransverseImpactPointCharged(
	const FTS& originalFTS, const GlobalPoint& referencePoint) const 
{

  GlobalVector pvecOrig = originalFTS.momentum();
  GlobalPoint xvecOrig = originalFTS.position();
  double kappa = originalFTS.transverseCurvature();
  double pxOrig = pvecOrig.x();
  double pyOrig = pvecOrig.y();
  double pzOrig = pvecOrig.z();
  double xOrig = xvecOrig.x();
  double yOrig = xvecOrig.y();
  double zOrig = xvecOrig.z();

//  double fac = 1./originalFTS.charge()/MagneticField::inInverseGeV(referencePoint).z();
  double fac = 1./originalFTS.charge()/
    (originalFTS.parameters().magneticField().inTesla(referencePoint).z() * 2.99792458e-3);
  GlobalVectorDouble xOrig2Centre = GlobalVectorDouble(fac * pyOrig, -fac * pxOrig, 0.);
  GlobalVectorDouble xOrigProj = GlobalVectorDouble(xOrig, yOrig, 0.);
  GlobalVectorDouble xRefProj = GlobalVectorDouble(referencePoint.x(), referencePoint.y(), 0.);
  GlobalVectorDouble deltax = xRefProj-xOrigProj-xOrig2Centre;
  GlobalVectorDouble ndeltax = deltax.unit();

  PropagationDirection direction = anyDirection;
  Surface::PositionType pos(referencePoint);
  // Need to define plane with orientation as the
  // ImpactPointSurface
  GlobalVector X(ndeltax.x(), ndeltax.y(), ndeltax.z());
  GlobalVector Y(0.,0.,1.);
  Surface::RotationType rot(X,Y);
  BoundPlane* plane = new BoundPlane(pos,rot);
  // Using Teddy's HelixBarrelPlaneCrossingByCircle for general barrel planes. 
  // A large variety of other,
  // direct solutions turned out to be not so stable numerically.
  HelixBarrelPlaneCrossingByCircle 
    planeCrossing(HelixPlaneCrossing::PositionType(xOrig, yOrig, zOrig),
		  HelixPlaneCrossing::DirectionType(pxOrig, pyOrig, pzOrig), 
		  kappa, direction);
  std::pair<bool,double> propResult = planeCrossing.pathLength(*plane);
  if ( !propResult.first ) 
    throw TrajectoryStateException("Propagation to perigee plane failed!");
  double s = propResult.second;
  HelixPlaneCrossing::PositionType xGen = planeCrossing.position(s);
  GlobalPoint xPerigee = GlobalPoint(xGen.x(),xGen.y(),xGen.z());
  // direction (reconverted to GlobalVector, renormalised)
  HelixPlaneCrossing::DirectionType pGen = planeCrossing.direction(s);
  pGen *= pvecOrig.mag()/pGen.mag();
  GlobalVector pPerigee = GlobalVector(pGen.x(),pGen.y(),pGen.z());
  delete plane;
		  
  if (originalFTS.hasError()) {
    AlgebraicSymMatrix errorMatrix = originalFTS.curvilinearError().matrix();
    AnalyticalCurvilinearJacobian curvilinJacobian(originalFTS.parameters(), xPerigee,
						   pPerigee, s);
    AlgebraicMatrix jacobian = curvilinJacobian.jacobian();
    errorMatrix = errorMatrix.similarity(jacobian);
  
    return FreeTrajectoryState(GlobalTrajectoryParameters(xPerigee, pPerigee, originalFTS.charge(), 
    					&(originalFTS.parameters().magneticField())),
			       CurvilinearTrajectoryError(errorMatrix));
  } 
  else {
    return FreeTrajectoryState(GlobalTrajectoryParameters(xPerigee, pPerigee, originalFTS.charge(), 
    					&(originalFTS.parameters().magneticField())));
  }
  
}


FreeTrajectoryState 
TSCPBuilderNoMaterial::createFTSatTransverseImpactPointNeutral(const FTS& originalFTS, 
    const GlobalPoint& referencePoint) const 
{

  GlobalPoint xvecOrig = originalFTS.position();
  double phi = originalFTS.momentum().phi();
  double theta = originalFTS.momentum().theta();
  double xOrig = xvecOrig.x();
  double yOrig = xvecOrig.y();
  double zOrig = xvecOrig.z();
  double xR = referencePoint.x();
  double yR = referencePoint.y();

  double s2D = (xR - xOrig)  * cos(phi) + (yR - yOrig)  * sin(phi);
  double s = s2D / sin(theta);
  double xGen = xOrig + s2D*cos(phi);
  double yGen = yOrig + s2D*sin(phi);
  double zGen = zOrig + s2D/tan(theta);
  GlobalPoint xPerigee = GlobalPoint(xGen, yGen, zGen);

  GlobalVector pPerigee = originalFTS.momentum();
		  
  if (originalFTS.hasError()) {
    AlgebraicSymMatrix errorMatrix = originalFTS.curvilinearError().matrix();
    AnalyticalCurvilinearJacobian curvilinJacobian(originalFTS.parameters(), xPerigee,
						   pPerigee, s);
    AlgebraicMatrix jacobian = curvilinJacobian.jacobian();
    errorMatrix = errorMatrix.similarity(jacobian);
  
    return FreeTrajectoryState(GlobalTrajectoryParameters(xPerigee, pPerigee, originalFTS.charge(), 
					&(originalFTS.parameters().magneticField())),
			       CurvilinearTrajectoryError(errorMatrix));
  } 
  else {
    return FreeTrajectoryState(GlobalTrajectoryParameters(xPerigee, pPerigee, originalFTS.charge(),
    					&(originalFTS.parameters().magneticField())));
  }
  
}
