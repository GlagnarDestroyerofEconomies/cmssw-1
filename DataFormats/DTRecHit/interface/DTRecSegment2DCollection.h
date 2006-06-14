#ifndef DTRecHit_DTRecSegment2DCollection_h
#define DTRecHit_DTRecSegment2DCollection_h

/** \class DTRecSegment2DCollection
 *
 * Collection of DTRecSegment2D
 *  
 * $Date: 2006/05/02 07:08:42 $
 * $Revision: 1.2 $
 * \author Stefano Lacaprara - INFN Legnaro <stefano.lacaprara@pd.infn.it>
 * \author Riccardo Bellan - INFN TO <riccardo.bellan@cern.ch>
 *
 */

/* Base Class Headers */
#include <functional>

/* Collaborating Class Declarations */
#include "DataFormats/Common/interface/RangeMap.h"
#include "DataFormats/Common/interface/ClonePolicy.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "DataFormats/DTRecHit/interface/DTSLRecSegment2D.h"
#include "DataFormats/MuonDetId/interface/DTSuperLayerId.h"

/* C++ Headers */

/* ====================================================================== */

typedef edm::RangeMap<DTSuperLayerId, edm::OwnVector<DTSLRecSegment2D> > DTRecSegment2DCollection;

#endif // DTRecHit_DTRecSegment2DCollection_h
