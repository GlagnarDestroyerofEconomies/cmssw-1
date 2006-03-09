#ifndef LayerHitMapCache_H
#define LayerHitMapCache_H

/** A cache adressable by DetLayer* and TrackingRegion* .
 *  Used to cache all the hits of a DetLayer.
 */

#include "RecoTracker/TkTrackingRegions/interface/TrackingRegion.h"
#include "RecoTracker/TkHitPairs/interface/SimpleCache.h"
#include "RecoTracker/TkHitPairs/interface/LayerHitMap.h"
#include "RecoTracker/TkHitPairs/interface/LayerWithHits.h"



class LayerHitMapCache {

private:
//  typedef pair<const DetLayer *, const TrackingRegion *> LayerRegionKey;
//  typedef const DetLayer * LayerRegionKey;


  typedef const LayerWithHits * LayerRegionKey;
  typedef TkHitPairs::SimpleCache<LayerRegionKey, LayerHitMap> Cache;
 public:
  LayerHitMapCache(int initSize=50) { theCache = new Cache(initSize); }

  ~LayerHitMapCache() { delete theCache; }

  void clear() { theCache->clear(); }

  const LayerHitMap & operator()(
      const LayerWithHits *layer, const TrackingRegion &region) {
//  LayerRegionKey key(layer,&region);
       LayerRegionKey key(layer);
    const LayerHitMap * lhm = theCache->get(key);
    if (lhm==0) {
      //MP
      //     lhm = new LayerHitMap( region.hits(layer) );
      //      SiPixelRecHitCollection::Range temporary;
      lhm=new LayerHitMap(*layer);
      theCache->add( key, lhm); 
    }
    return *lhm;
  }
public:
  LayerHitMapCache(const LayerHitMapCache &) { }

private:
  Cache * theCache; 
};

#endif

