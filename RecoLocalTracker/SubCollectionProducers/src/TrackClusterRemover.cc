#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit1D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/ProjectedSiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/Provenance/interface/ProductID.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackerRecHit2D/interface/ClusterRemovalInfo.h"

#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
//
// class decleration
//

class TrackClusterRemover : public edm::EDProducer {
    public:
        TrackClusterRemover(const edm::ParameterSet& iConfig) ;
        ~TrackClusterRemover() ;
        void produce(edm::Event &iEvent, const edm::EventSetup &iSetup) ;
    private:
        struct ParamBlock {
            ParamBlock() : isSet_(false), usesCharge_(false) {}
            ParamBlock(const edm::ParameterSet& iConfig) :
                isSet_(true), 
                usesCharge_(iConfig.exists("maxCharge")),
                usesSize_(iConfig.exists("maxSize")),
                maxChi2_(iConfig.getParameter<double>("maxChi2")),
                maxCharge_(usesCharge_ ? iConfig.getParameter<double>("maxCharge") : 0), 
                maxSize_(usesSize_ ? iConfig.getParameter<uint32_t>("maxSize") : 0) { }
            bool  isSet_, usesCharge_, usesSize_;
            float maxChi2_, maxCharge_;
            size_t maxSize_;
        };
        static const unsigned int NumberOfParamBlocks = 6;

        edm::InputTag trajectories_;
        bool doStrip_, doPixel_;
        edm::InputTag stripClusters_, pixelClusters_;
        bool mergeOld_;
        edm::InputTag oldRemovalInfo_;

        ParamBlock pblocks_[NumberOfParamBlocks];
        void readPSet(const edm::ParameterSet& iConfig, const std::string &name, 
                int id1=-1, int id2=-1, int id3=-1, int id4=-1, int id5=-1, int id6=-1) ;

        std::vector<uint8_t> pixels, strips;                // avoid unneed alloc/dealloc of this
        edm::ProductID pixelSourceProdID, stripSourceProdID; // ProdIDs refs must point to (for consistency tests)

        inline void process(const TrackingRecHit *hit, float chi2);
        inline void process(const SiStripRecHit2D *hit2D, uint32_t subdet);
        inline void process(const SiStripRecHit1D *hit1D, uint32_t subdet);

        template<typename T> 
        std::auto_ptr<edmNew::DetSetVector<T> >
        cleanup(const edmNew::DetSetVector<T> &oldClusters, const std::vector<uint8_t> &isGood, 
                    reco::ClusterRemovalInfo::Indices &refs, const reco::ClusterRemovalInfo::Indices *oldRefs) ;

        // Carries in full removal info about a given det from oldRefs
        void mergeOld(reco::ClusterRemovalInfo::Indices &refs, const reco::ClusterRemovalInfo::Indices &oldRefs) ;

  bool clusterWasteSolution_;
  bool filterTracks_;
  reco::TrackBase::TrackQuality trackQuality_;
  std::map<uint32_t, std::set< SiStripRecHit1D::ClusterRef > > collectedStrip;
  std::map<uint32_t, std::set< SiPixelRecHit::ClusterRef  > > collectedPixel;
};


using namespace std;
using namespace edm;
using namespace reco;

void
TrackClusterRemover::readPSet(const edm::ParameterSet& iConfig, const std::string &name, 
    int id1, int id2, int id3, int id4, int id5, int id6) 
{
    if (iConfig.exists(name)) {
        ParamBlock pblock(iConfig.getParameter<ParameterSet>(name));
        if (id1 == -1) {
            fill(pblocks_, pblocks_+NumberOfParamBlocks, pblock);
        } else {
            pblocks_[id1] = pblock;
            if (id2 != -1) pblocks_[id2] = pblock;
            if (id3 != -1) pblocks_[id3] = pblock;
            if (id4 != -1) pblocks_[id4] = pblock;
            if (id5 != -1) pblocks_[id5] = pblock;
            if (id6 != -1) pblocks_[id6] = pblock;
        }
    }
}

TrackClusterRemover::TrackClusterRemover(const ParameterSet& iConfig):
    trajectories_(iConfig.getParameter<InputTag>("trajectories")),
    doStrip_(iConfig.existsAs<bool>("doStrip") ? iConfig.getParameter<bool>("doStrip") : true),
    doPixel_(iConfig.existsAs<bool>("doPixel") ? iConfig.getParameter<bool>("doPixel") : true),
    stripClusters_(doStrip_ ? iConfig.getParameter<InputTag>("stripClusters") : InputTag("NONE")),
    pixelClusters_(doPixel_ ? iConfig.getParameter<InputTag>("pixelClusters") : InputTag("NONE")),
    mergeOld_(iConfig.exists("oldClusterRemovalInfo")),
    oldRemovalInfo_(mergeOld_ ? iConfig.getParameter<InputTag>("oldClusterRemovalInfo") : InputTag("NONE")),
    clusterWasteSolution_(true)
{
  if (iConfig.exists("clusterLessSolution"))
    clusterWasteSolution_=!iConfig.getParameter<bool>("clusterLessSolution");
  if (doPixel_ && clusterWasteSolution_) produces< edmNew::DetSetVector<SiPixelCluster> >();
  if (doStrip_ && clusterWasteSolution_) produces< edmNew::DetSetVector<SiStripCluster> >();
  if (clusterWasteSolution_) produces< ClusterRemovalInfo >();

    fill(pblocks_, pblocks_+NumberOfParamBlocks, ParamBlock());
    readPSet(iConfig, "Common",-1);
    if (doPixel_) {
        readPSet(iConfig, "Pixel" ,0,1);
        readPSet(iConfig, "PXB" ,0);
        readPSet(iConfig, "PXE" ,1);
    }
    if (doStrip_) {
        readPSet(iConfig, "Strip" ,2,3,4,5);
        readPSet(iConfig, "StripInner" ,2,3);
        readPSet(iConfig, "StripOuter" ,4,5);
        readPSet(iConfig, "TIB" ,2);
        readPSet(iConfig, "TID" ,3);
        readPSet(iConfig, "TOB" ,4);
        readPSet(iConfig, "TEC" ,5);
    }

    bool usingCharge = false;
    for (size_t i = 0; i < NumberOfParamBlocks; ++i) {
        if (!pblocks_[i].isSet_) throw cms::Exception("Configuration Error") << "TrackClusterRemover: Missing configuration for detector with subDetID = " << (i+1);
        if (pblocks_[i].usesCharge_ && !usingCharge) {
            throw cms::Exception("Configuration Error") << "TrackClusterRemover: Configuration for subDetID = " << (i+1) << " uses cluster charge, which is not enabled.";
        }
    }

    if (!clusterWasteSolution_){
      produces<edmNew::DetSetVector<SiPixelClusterRefNew> >();
      produces<edmNew::DetSetVector<SiStripRecHit1D::ClusterRef> >();
    }
    trackQuality_=reco::TrackBase::undefQuality;
    filterTracks_=false;
    if (iConfig.exists("TrackQuality")){
      filterTracks_=true;
      trackQuality_=reco::TrackBase::qualityByName(iConfig.getParameter<std::string>("TrackQuality"));
    }

}


TrackClusterRemover::~TrackClusterRemover()
{
}

void TrackClusterRemover::mergeOld(ClusterRemovalInfo::Indices &refs,
                                            const ClusterRemovalInfo::Indices &oldRefs) 
{
        for (size_t i = 0, n = refs.size(); i < n; ++i) {
            refs[i] = oldRefs[refs[i]];
       }
}

 
template<typename T> 
auto_ptr<edmNew::DetSetVector<T> >
TrackClusterRemover::cleanup(const edmNew::DetSetVector<T> &oldClusters, const std::vector<uint8_t> &isGood, 
			     reco::ClusterRemovalInfo::Indices &refs, const reco::ClusterRemovalInfo::Indices *oldRefs){
    typedef typename edmNew::DetSetVector<T>             DSV;
    typedef typename edmNew::DetSetVector<T>::FastFiller DSF;
    typedef typename edmNew::DetSet<T>                   DS;
    auto_ptr<DSV> output(new DSV());
    output->reserve(oldClusters.size(), oldClusters.dataSize());

    unsigned int countOld=0;
    unsigned int countNew=0;

    // cluster removal loop
    const T * firstOffset = & oldClusters.data().front();
    for (typename DSV::const_iterator itdet = oldClusters.begin(), enddet = oldClusters.end(); itdet != enddet; ++itdet) {
        DS oldDS = *itdet;

        if (oldDS.empty()) continue; // skip empty detsets

        uint32_t id = oldDS.detId();
        DSF outds(*output, id);

        for (typename DS::const_iterator it = oldDS.begin(), ed = oldDS.end(); it != ed; ++it) {
            uint32_t index = ((&*it) - firstOffset);
	    countOld++;
            if (isGood[index]) { 
                outds.push_back(*it);
		countNew++;
                refs.push_back(index); 
                //std::cout << "TrackClusterRemover::cleanup " << typeid(T).name() << " reference " << index << " to " << (refs.size() - 1) << std::endl;
            }

	}
        if (outds.empty()) outds.abort(); // not write in an empty DSV
    }

    if (oldRefs != 0) mergeOld(refs, *oldRefs);
    return output;
}


void TrackClusterRemover::process(const SiStripRecHit2D *hit, uint32_t subdet) {
  SiStripRecHit2D::ClusterRef cluster = hit->cluster();
  if (cluster.id() != stripSourceProdID) throw cms::Exception("Inconsistent Data") <<
    "TrackClusterRemover: strip cluster ref from Product ID = " << cluster.id() <<
    " does not match with source cluster collection (ID = " << stripSourceProdID << ")\n.";
  
  assert(cluster.id() == stripSourceProdID);
  if (pblocks_[subdet-1].usesSize_ && (cluster->amplitudes().size() > pblocks_[subdet-1].maxSize_)) return;

  strips[cluster.key()] = false;  
  collectedStrip[hit->geographicalId()].insert(cluster);
}

void TrackClusterRemover::process(const SiStripRecHit1D *hit, uint32_t subdet) {
    SiStripRecHit1D::ClusterRef cluster = hit->cluster();
    if (cluster.id() != stripSourceProdID) throw cms::Exception("Inconsistent Data") << 
            "TrackClusterRemover: strip cluster ref from Product ID = " << cluster.id() << 
            " does not match with source cluster collection (ID = " << stripSourceProdID << ")\n.";

    assert(cluster.id() == stripSourceProdID);
    if (pblocks_[subdet-1].usesSize_ && (cluster->amplitudes().size() > pblocks_[subdet-1].maxSize_)) return;

//DBG// cout << "Individual HIT " << cluster.key().first << ", INDEX = " << cluster.key().second << endl;
    strips[cluster.key()] = false;

    collectedStrip[hit->geographicalId()].insert(cluster);
}




void TrackClusterRemover::process(const TrackingRecHit *hit, float chi2) {
    DetId detid = hit->geographicalId(); 
    uint32_t subdet = detid.subdetId();

    assert ((subdet > 0) && (subdet <= NumberOfParamBlocks));

    // chi2 cut
    if (chi2 > pblocks_[subdet-1].maxChi2_) return;

    if ((subdet == PixelSubdetector::PixelBarrel) || (subdet == PixelSubdetector::PixelEndcap)) {
        if (!doPixel_) return;
        // this is a pixel, and i *know* it is
        const SiPixelRecHit *pixelHit = static_cast<const SiPixelRecHit *>(hit);

        SiPixelRecHit::ClusterRef cluster = pixelHit->cluster();

        if (cluster.id() != pixelSourceProdID) throw cms::Exception("Inconsistent Data") << 
                "TrackClusterRemover: pixel cluster ref from Product ID = " << cluster.id() << 
                " does not match with source cluster collection (ID = " << pixelSourceProdID << ")\n.";

        assert(cluster.id() == pixelSourceProdID);
//DBG// cout << "HIT NEW PIXEL DETID = " << detid.rawId() << ", Cluster [ " << cluster.key().first << " / " <<  cluster.key().second << " ] " << endl;

        // if requested, cut on cluster size
        if (pblocks_[subdet-1].usesSize_ && (cluster->pixels().size() > pblocks_[subdet-1].maxSize_)) return;

        // mark as used
        pixels[cluster.key()] = false;
	
	collectedPixel[detid.rawId()].insert(cluster);
    } else { // aka Strip
        if (!doStrip_) return;
        const type_info &hitType = typeid(*hit);
        if (hitType == typeid(SiStripRecHit2D)) {
            const SiStripRecHit2D *stripHit = static_cast<const SiStripRecHit2D *>(hit);
//DBG//     cout << "Plain RecHit 2D: " << endl;
            process(stripHit,subdet);}
	else if (hitType == typeid(SiStripRecHit1D)) {
	  const SiStripRecHit1D *hit1D = static_cast<const SiStripRecHit1D *>(hit);
	  process(hit1D,subdet);
        } else if (hitType == typeid(SiStripMatchedRecHit2D)) {
            const SiStripMatchedRecHit2D *matchHit = static_cast<const SiStripMatchedRecHit2D *>(hit);
//DBG//     cout << "Matched RecHit 2D: " << endl;
            process(matchHit->monoHit(),subdet);
            process(matchHit->stereoHit(),subdet);
        } else if (hitType == typeid(ProjectedSiStripRecHit2D)) {
            const ProjectedSiStripRecHit2D *projHit = static_cast<const ProjectedSiStripRecHit2D *>(hit);
//DBG//     cout << "Projected RecHit 2D: " << endl;
            process(&projHit->originalHit(),subdet);
        } else throw cms::Exception("NOT IMPLEMENTED") << "Don't know how to handle " << hitType.name() << " on detid " << detid.rawId() << "\n";
    }
}

/*   Schematic picture of n-th step Iterative removal
 *   (that os removing clusters after n-th step tracking)
 *   clusters:    [ C1 ] -> [ C2 ] -> ... -> [ Cn ] -> [ Cn + 1 ]
 *                   ^                          ^           ^--- OUTPUT "new" ID
 *                   |-- before any removal     |----- Source clusters                                                                             
 *                   |-- OUTPUT "old" ID        |----- Hits in Traj. point here                       
 *                   |                          \----- Old ClusterRemovalInfo "new" ID                 
 *                   \-- Old ClusterRemovalInfo "old" ID                                                  
 */


void
TrackClusterRemover::produce(Event& iEvent, const EventSetup& iSetup)
{
    ProductID pixelOldProdID, stripOldProdID;

    Handle<edmNew::DetSetVector<SiPixelCluster> > pixelClusters;
    if (doPixel_) {
        iEvent.getByLabel(pixelClusters_, pixelClusters);
        pixelSourceProdID = pixelClusters.id();
    }
//DBG// std::cout << "TrackClusterRemover: Read pixel " << pixelClusters_.encode() << " = ID " << pixelSourceProdID << std::endl;

    Handle<edmNew::DetSetVector<SiStripCluster> > stripClusters;
    if (doStrip_) {
        iEvent.getByLabel(stripClusters_, stripClusters);
        stripSourceProdID = stripClusters.id();
    }
//DBG// std::cout << "TrackClusterRemover: Read strip " << stripClusters_.encode() << " = ID " << stripSourceProdID << std::endl;

    auto_ptr<ClusterRemovalInfo> cri;
    if (clusterWasteSolution_){
      if (doStrip_ && doPixel_) cri.reset(new ClusterRemovalInfo(pixelClusters, stripClusters));
      else if (doStrip_) cri.reset(new ClusterRemovalInfo(stripClusters));
      else if (doPixel_) cri.reset(new ClusterRemovalInfo(pixelClusters));
    }

    Handle<ClusterRemovalInfo> oldRemovalInfo;
    if (mergeOld_ && clusterWasteSolution_) { 
        iEvent.getByLabel(oldRemovalInfo_, oldRemovalInfo); 
        // Check ProductIDs
        if ( (oldRemovalInfo->stripNewRefProd().id() == stripClusters.id()) &&
             (oldRemovalInfo->pixelNewRefProd().id() == pixelClusters.id()) ) {

            cri->getOldClustersFrom(*oldRemovalInfo);

            pixelOldProdID = oldRemovalInfo->pixelRefProd().id();
            stripOldProdID = oldRemovalInfo->stripRefProd().id();

        } else {
            throw cms::Exception("Inconsistent Data") << "TrackClusterRemover: " <<
                "Input collection product IDs are [pixel: " << pixelClusters.id() << ", strip: " << stripClusters.id() << "] \n" <<
                "\t but the *old* ClusterRemovalInfo " << oldRemovalInfo_.encode() << " refers as 'new product ids' to " <<
                    "[pixel: " << oldRemovalInfo->pixelNewRefProd().id() << ", strip: " << oldRemovalInfo->stripNewRefProd().id() << "]\n" << 
                "NOTA BENE: when running TrackClusterRemover with an old ClusterRemovalInfo the hits in the trajectory MUST be already re-keyed.\n";
        }
    } else { // then Old == Source
        pixelOldProdID = pixelSourceProdID;
        stripOldProdID = stripSourceProdID;
    }

    Handle<TrajTrackAssociationCollection> trajectories_totrack; 
    iEvent.getByLabel(trajectories_,trajectories_totrack);

    if (doStrip_) {
      strips.resize(stripClusters->dataSize()); fill(strips.begin(), strips.end(), true);
    }
    if (doPixel_) {
      pixels.resize(pixelClusters->dataSize()); fill(pixels.begin(), pixels.end(), true);
    }

    TrajTrackAssociationCollection::const_iterator asst=trajectories_totrack->begin();
    for (;asst!=trajectories_totrack->end();++asst){
      const Track & track = *(asst->val);
      bool goodTk = (track.quality(trackQuality_));
      if (filterTracks_ && !goodTk) continue;
      const Trajectory &tj = *(asst->key);
      const vector<TrajectoryMeasurement> &tms = tj.measurements();
      vector<TrajectoryMeasurement>::const_iterator itm, endtm;
      for (itm = tms.begin(), endtm = tms.end(); itm != endtm; ++itm) {
	const TrackingRecHit *hit = itm->recHit()->hit();
	if (!hit->isValid()) continue; 
	process( hit, itm->estimate() );
      }
    }

    
    if (doPixel_ && clusterWasteSolution_) {
        auto_ptr<edmNew::DetSetVector<SiPixelCluster> > newPixelClusters = cleanup(*pixelClusters, pixels, 
                    cri->pixelIndices(), mergeOld_ ? &oldRemovalInfo->pixelIndices() : 0);
        OrphanHandle<edmNew::DetSetVector<SiPixelCluster> > newPixels = iEvent.put(newPixelClusters); 
//DBG// std::cout << "TrackClusterRemover: Wrote pixel " << newPixels.id() << " from " << pixelSourceProdID << std::endl;
        cri->setNewPixelClusters(newPixels);
    }
    if (doStrip_ && clusterWasteSolution_) {
        auto_ptr<edmNew::DetSetVector<SiStripCluster> > newStripClusters = cleanup(*stripClusters, strips, 
                    cri->stripIndices(), mergeOld_ ? &oldRemovalInfo->stripIndices() : 0);
        OrphanHandle<edmNew::DetSetVector<SiStripCluster> > newStrips = iEvent.put(newStripClusters); 
//DBG// std::cout << "TrackClusterRemover: Wrote strip " << newStrips.id() << " from " << stripSourceProdID << std::endl;
        cri->setNewStripClusters(newStrips);
    }

    
    if (clusterWasteSolution_) {
      //      double fraction_pxl= cri->pixelIndices().size() / (double) pixels.size();
      //      double fraction_strp= cri->stripIndices().size() / (double) strips.size();
      //      edm::LogWarning("TrackClusterRemover")<<" fraction: " << fraction_pxl <<" "<<fraction_strp;
      iEvent.put(cri);
    }

    pixels.clear(); strips.clear(); 

    if (!clusterWasteSolution_){
      auto_ptr<edmNew::DetSetVector<SiPixelClusterRefNew> > removedPixelClsuterRefs(new edmNew::DetSetVector<SiPixelClusterRefNew>());
      auto_ptr<edmNew::DetSetVector<SiStripRecHit1D::ClusterRef> > removedStripClsuterRefs(new edmNew::DetSetVector<SiStripRecHit1D::ClusterRef>());
      
      edm::Handle<edmNew::DetSetVector<SiPixelClusterRefNew> > oldPxlRef;
      edm::Handle<edmNew::DetSetVector<SiStripRecHit1D::ClusterRef> > oldStrRef;
      if (mergeOld_){
	iEvent.getByLabel(oldRemovalInfo_,oldPxlRef);
	iEvent.getByLabel(oldRemovalInfo_,oldStrRef);
      }      
      for (std::map<uint32_t, std::set< SiStripRecHit1D::ClusterRef > >::iterator itskiped= collectedStrip.begin();
	   itskiped!=collectedStrip.end();++itskiped){
	edmNew::DetSetVector<SiStripRecHit1D::ClusterRef>::FastFiller fill(*removedStripClsuterRefs, itskiped->first);
	if (mergeOld_){
	  edmNew::DetSetVector<SiStripRecHit1D::ClusterRef>::const_iterator f=oldStrRef->find(itskiped->first);
	  if (f!=oldStrRef->end())
	    itskiped->second.insert(f->begin(),f->end());
	}
	for (std::set< SiStripRecHit1D::ClusterRef >::iterator topush = itskiped->second.begin();
	     topush!=itskiped->second.end();++topush){
	  fill.push_back(*topush);
	}
	if (fill.empty()) fill.abort();
	itskiped->second.clear();
      }
      iEvent.put( removedStripClsuterRefs );

      for (std::map<uint32_t, std::set< SiPixelRecHit::ClusterRef  > >::iterator itskiped= collectedPixel.begin();
	   itskiped!=collectedPixel.end();++itskiped){  
	edmNew::DetSetVector<SiPixelClusterRefNew>::FastFiller fill(*removedPixelClsuterRefs, itskiped->first);
	if (mergeOld_){
	  edmNew::DetSetVector<SiPixelRecHit::ClusterRef>::const_iterator f=oldPxlRef->find(itskiped->first);
	  if (f!=oldPxlRef->end())
	    itskiped->second.insert(f->begin(),f->end());
	}
	
	for (std::set< SiPixelRecHit::ClusterRef  >::iterator topush = itskiped->second.begin(); 
	     topush!=itskiped->second.end();++topush){ 
	  fill.push_back(*topush); 
	}
	if (fill.empty()) fill.abort();   
	itskiped->second.clear();
      }
      iEvent.put( removedPixelClsuterRefs );
      
    }
    

}

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TrackClusterRemover);
