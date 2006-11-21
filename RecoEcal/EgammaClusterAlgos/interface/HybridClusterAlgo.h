#ifndef RecoEcal_EgammaClusterAlgos_HybridClusterAlgo_h
#define RecoEcal_EgammaClusterAlgos_HybridClusterAlgo_h

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "RecoCaloTools/Navigation/interface/EcalBarrelNavigator.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"
#include <vector>
#include <set>

//Less than operator for sorting EcalRecHits according to energy.
struct less_mag : public std::binary_function<EcalRecHit, EcalRecHit, bool> {
  bool operator()(EcalRecHit x, EcalRecHit y) { return x.energy() > y.energy() ; }
};

class HybridClusterAlgo
{
 private:
  //Quick typedef for position calculation.
  typedef math::XYZPoint Point;

  //Thresholds for seeds.
  double eb_st;

  //Number of steps in phi that the Hybrid algorithm will take
  //when clustering.  Remember, uses phi_steps in positive direction
  //and then phi_steps in negative direction.
  int phi_steps;

  //Threshold for basic cluster.
  double Ethres;

  //Threshold for becoming a sub-peak in the supercluster.
  double Eseed;

  //Threshold for adding the additional two 'wing' cells to domino. 
  double Ewing;

  //Map of DetId, RecHit relationship.  EcalRecHit knows what DetId it is,
  //but DetId doesn't  know what EcalRecHit it is. 
  //  std::map<DetId, EcalRecHit>  rechits_m;

  // colection of all rechits
  const EcalRecHitCollection *recHits_;

  //Set of DetIds that have already been used.
  std::set<DetId> useddetids;

  // The vector of seeds:
  std::vector<EcalRecHit> seeds;

  //The vector of seed clusters:
  std::vector<reco::BasicCluster> seedClus_;

  //Map of basicclusters and what supercluster they will belong to.
  std::map<int, std::vector<reco::BasicCluster> > clustered_;

  //Control the verbosity.
  int debugLevel_;

  //algo to calulate position of clusters
  PositionCalc posCalculator_;

 public:
   enum DebugLevel { pDEBUG = 0, pINFO = 1, pERROR = 2 }; 
  
  //The default constructor
  HybridClusterAlgo(){ }
  

  //The real constructor
  HybridClusterAlgo(double eb_str, 
		    int step, 
		    double ethresh, 
		    double eseed,
		    double ewing,
                const PositionCalc& posCalculator,
		    DebugLevel debugLevel = pINFO);

  //Hand over the map, the geometry, and I'll hand you back clusters.
  void makeClusters(const EcalRecHitCollection* , const CaloSubdetectorGeometry * geometry, reco::BasicClusterCollection &basicClusters);

  //Make superclusters from the references to the BasicClusters in the event.
  reco::SuperClusterCollection makeSuperClusters(const reco::BasicClusterRefVector&);

  //The routine doing the real work.
  void mainSearch(const EcalRecHitCollection* hits, const CaloSubdetectorGeometry * geometry);
  
  //Make dominos for the hybrid method.
  double makeDomino(EcalBarrelNavigator &navigator, std::vector <EcalRecHit> &cells);

};

#endif
