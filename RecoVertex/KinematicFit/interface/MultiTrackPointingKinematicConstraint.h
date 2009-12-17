#ifndef PointingMultiKinematicConstraint_H
#define PointingMultiKinematicConstraint_H

#include "RecoVertex/KinematicFitPrimitives/interface/MultiTrackKinematicConstraint.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicState.h"
#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"

/**
 *  Topological constraint making a momentum vector to point to
 *  the given location in space.
 *  Example: if b-meson momentum is reconstructed at b-meson decay position
 *  (secondary vertex), making reconstructed momentum be in parallel to the link from primary 
 *  vertex to secondary vertex.
 *
 * 
 *  Kirill Prokofiev, March 2004
 *  MultiTrack version: Lars Perchalla, Philip Sauerland, Dec 2009
 */
//mother constructed from daughters. no propagation in field in this version!

class PointingMultiKinematicConstraint : public MultiTrackKinematicConstraint
{
public:
	PointingMultiKinematicConstraint(GlobalPoint& ref):refPoint(ref)
	{}
	
	/**
	 * Returns a vector of values of constraint
	 * equations at the point where the input
	 * particles are defined.
	 */
	virtual AlgebraicVector value(const vector<KinematicState> states, const GlobalPoint& point) const;
	
	/**
	 * Returns a matrix of derivatives of
	 * constraint equations w.r.t. 
	 * particle parameters
	 */
	virtual AlgebraicMatrix parametersDerivative(const vector<KinematicState> states, const GlobalPoint& point) const;
	
	/**
	 * Returns a matrix of derivatives of
	 * constraint equations w.r.t. 
	 * vertex position
	 */
	virtual AlgebraicMatrix positionDerivative(const vector<KinematicState> states, const GlobalPoint& point) const;
	
	/**
	 * Number of equations per track used for the fit
	 */
	virtual int numberOfEquations() const;
	
	virtual PointingMultiKinematicConstraint * clone()const
	{
		return new PointingMultiKinematicConstraint(*this);
	}
	
private:
	GlobalPoint refPoint;
	
};
#endif
