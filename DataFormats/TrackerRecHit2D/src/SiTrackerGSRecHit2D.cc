#include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSRecHit2D.h"


SiTrackerGSRecHit2D::SiTrackerGSRecHit2D( const LocalPoint& pos, const LocalError& err,
					  const DetId& id,
					  const unsigned int& simtrackId,
					  const PSimHit& simhit):
  BaseSiTrackerRecHit2DLocalPos(pos,err,id) ,
  simtrackId_ (simtrackId) ,
  simhit_(simhit) {}
