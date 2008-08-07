/*
 *  See header file for a description of this class.
 *
 *  $Date: $
 *  $Revision: $
 *  \author D. Trocino - University and INFN Torino
 */

#include "TrackingTools/TrackRefitter/interface/SeedTransformer.h"

// System include files
#include <memory>
#include <Riostream.h>

// Framework
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// Services and Tools
#include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"

// Geometry and Magnetic field
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

// Other include files
#include "TrackingTools/Records/interface/TransientRecHitRecord.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/PatternTools/interface/TrajectoryFitter.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"

using namespace std;
using namespace edm;
using namespace reco;


SeedTransformer::SeedTransformer(const ParameterSet& iConfig) {

  LogTrace("Reco|TrackingTools|SeedTransformer") << "SeedTransformer constructor called." << endl << endl;

  ParameterSet serviceParameters = iConfig.getParameter<ParameterSet>("ServiceParameters");
  theService = new MuonServiceProxy(serviceParameters);

  theFitterName = iConfig.getParameter<string>("Fitter");  
  theMuonRecHitBuilderName = iConfig.getParameter<string>("MuonRecHitBuilder");
  thePropagatorName = iConfig.getParameter<string>("Propagator");

  nMinRecHits = iConfig.getParameter<unsigned int>("NMinRecHits");

}

SeedTransformer::~SeedTransformer() {

  LogTrace("Reco|TrackingTools|SeedTransformer") << "SeedTransformer destructor called." << endl << endl;

  if (theService) delete theService;

}

void SeedTransformer::setServices(const EventSetup& iSetup) {

  // Update the services
  theService->update(iSetup);

  iSetup.get<GlobalTrackingGeometryRecord>().get(theTrackingGeometry); 
  iSetup.get<IdealMagneticFieldRecord>().get(theMagneticField);
  iSetup.get<TrackingComponentsRecord>().get(theFitterName,theFitter);
  iSetup.get<TransientRecHitRecord>().get(theMuonRecHitBuilderName,theMuonRecHitBuilder);
  iSetup.get<TrackingComponentsRecord>().get(thePropagatorName,thePropagator);

}

vector<Trajectory> SeedTransformer::seedTransform(const TrajectorySeed& aSeed) const {

  const string metname = "Reco|TrackingTools|SeedTransformer";

  LogTrace(metname) << "  Number of valid RecHits:      " << aSeed.nHits() << endl;

  if( aSeed.nHits() < nMinRecHits ) {
    LogTrace(metname) << "    --- Too few RecHits, no refit performed! ---" << endl;
    return vector<Trajectory>();
  }

  TrajectoryStateOnSurface aTSOS(seedTransientState(aSeed));

  // Rescale errors before refit, not to bias the result
  aTSOS.rescaleError(10.);

  vector<TransientTrackingRecHit::ConstRecHitPointer> recHits;
  unsigned int countRH = 0;

  for(TrajectorySeed::recHitContainer::const_iterator itRecHits=aSeed.recHits().first; itRecHits!=aSeed.recHits().second; ++itRecHits, ++countRH) {
    if((*itRecHits).isValid()) {
      TransientTrackingRecHit::ConstRecHitPointer ttrh(theMuonRecHitBuilder->build(&(*itRecHits)));
      recHits.push_back(ttrh);
    }
  } // end for(TrajectorySeed::recHitContainer::const_iterator itRecHits=aSeed.recHits().first; itRecHits!=aSeed.recHits().second; ++itRecHits, ++countRH)

  TrajectoryStateOnSurface aInitTSOS = thePropagator->propagate(aTSOS, recHits.front()->det()->surface());

  vector<Trajectory> refittedSeed = theFitter->fit(aSeed, recHits, aInitTSOS);

  if(refittedSeed.empty()) {
    LogTrace(metname) << "    --- Seed fit failed! ---" << endl;
    return vector<Trajectory>();
  }

  else if(!refittedSeed.front().isValid()) {
    LogTrace(metname) << "    --- Seed fitted, but trajectory not valid! ---" << endl;
    return vector<Trajectory>();
  }

  else
    LogTrace(metname) << "    +++ Seed fit succeded! +++" << endl;

  return refittedSeed;

}

TrajectoryStateOnSurface SeedTransformer::seedTransientState(const TrajectorySeed& tmpSeed) const {

  PTrajectoryStateOnDet tmpTSOD = tmpSeed.startingState();
  DetId tmpDetId(tmpTSOD.detId());
  const GeomDet* tmpGeomDet = theTrackingGeometry->idToDet(tmpDetId);

  TrajectoryStateTransform tsTransform;
  TrajectoryStateOnSurface tmpTSOS = tsTransform.transientState(tmpTSOD, &(tmpGeomDet->surface()), &(*theMagneticField));

  return tmpTSOS;

}
