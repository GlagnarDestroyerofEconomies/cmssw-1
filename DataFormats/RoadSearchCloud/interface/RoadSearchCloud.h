#ifndef DATAFORMATS_ROADSEARCHCLOUD_H
#define DATAFORMATS_ROADSEARCHCLOUD_H

//
// Package:         DataFormats/RoadSearchCloud
// Class:           RoadSearchCloud
// 
// Description:     Intermediate product of RoadSearch
//                  pattern recongnition. Holds refs to
//                  all RecHits in a Cloud following a Road.
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Sat Jan 14 22:00:00 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/08/29 22:15:40 $
// $Revision: 1.4 $
//

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "DataFormats/Common/interface/ClonePolicy.h"

class RoadSearchCloud {
public:

  typedef edm::OwnVector<TrackingRecHit,edm::ClonePolicy<TrackingRecHit> > RecHitOwnVector;

  RoadSearchCloud() {}
  RoadSearchCloud(RecHitOwnVector rechits): recHits_(rechits) {}

  inline RoadSearchCloud* clone() const { return new RoadSearchCloud(recHits_); }
  inline RecHitOwnVector recHits() const { return recHits_;}
  inline void addHit(TrackingRecHit* input) { recHits_.push_back(input); }
  inline unsigned int size() const { return recHits_.size(); }
  inline RecHitOwnVector::const_iterator begin_hits() const { return recHits_.begin(); }
  inline RecHitOwnVector::const_iterator end_hits()   const { return recHits_.end();   }


private:

  RecHitOwnVector recHits_;

};

#endif // DATAFORMATS_ROADSEARCHCLOUD_H
