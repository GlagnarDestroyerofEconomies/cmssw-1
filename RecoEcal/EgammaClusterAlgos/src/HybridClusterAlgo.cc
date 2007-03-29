#include "RecoEcal/EgammaClusterAlgos/interface/HybridClusterAlgo.h"
#include "RecoCaloTools/Navigation/interface/EcalBarrelNavigator.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloTopology/interface/EcalBarrelHardcodedTopology.h"
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"
#include <iostream>
#include <map>
#include <vector>
#include <set>


//The real constructor
HybridClusterAlgo::HybridClusterAlgo(double eb_str, 
                  int step, 
                  double ethresh, 
                  double eseed,
                  double ewing,
                  const PositionCalc& posCalculator,
                  DebugLevel debugLevel) :
   eb_st(eb_str),
   phi_steps(step), Ethres(ethresh), Eseed(eseed),  Ewing(ewing), debugLevel_(debugLevel)
{
   posCalculator_ = posCalculator;
}

// Return a vector of clusters from a collection of EcalRecHits:
void HybridClusterAlgo::makeClusters(const EcalRecHitCollection*recColl, 
				     const CaloSubdetectorGeometry*geometry,
				     reco::BasicClusterCollection &basicClusters,
				     bool regional,
				     const std::vector<EcalEtaPhiRegion>& regions)
{

  //clear vector of seeds
  seeds.clear();
  //clear map of supercluster/basiccluster association
  clustered_.clear();
  //clear set of used detids
  useddetids.clear();
  //clear vector of seed clusters
  seedClus_.clear();
  //Pass in a pointer to the collection.
  recHits_ = recColl;
  
  if ( debugLevel_ == pDEBUG ) {
  std::cout << "Cleared vectors, starting clusterization..." << std::endl;
  }

  int nregions=0;
  if(regional) nregions=regions.size();

  if(!regional || nregions) {

    EcalRecHitCollection::const_iterator it;

    for (it = recHits_->begin(); it != recHits_->end(); it++){
    
      //Make the vector of seeds that we're going to use.
      //One of the few places position is used, needed for ET calculation.    
      const CaloCellGeometry *this_cell = (*geometry).getGeometry(it->id());
      GlobalPoint position = this_cell->getPosition();
      
      // Require that RecHit is within clustering region in case
      // of regional reconstruction
      bool withinRegion = false;
      if (regional) {
	std::vector<EcalEtaPhiRegion>::const_iterator region;
	for (region=regions.begin(); region!=regions.end(); region++) {
	  if (region->inRegion(position)) {
	    withinRegion =  true;
	    break;
	  }
	}
      }
      
      if (!regional || withinRegion) {
	float ET = it->energy() * sin(position.theta());

	//Must pass seed threshold.
	if (ET > eb_st){
	  seeds.push_back(*it);
	  if ( debugLevel_ == pDEBUG ){
	    std::cout << "Seed ET: " << ET << std::endl;
	    std::cout << "Seed E: " << it->energy() << std::endl;
	  }
	}
      }
    }
    
  }
  

  //Yay sorting.
  if ( debugLevel_ == pDEBUG )
    std::cout << "Built vector of seeds, about to sort them...";
  
  //Needs three argument sort with seed comparison operator
  sort(seeds.begin(), seeds.end(), less_mag());
  
  if ( debugLevel_ == pDEBUG )
    std::cout << "done" << std::endl;

  //Now to do the work.
  if ( debugLevel_ ==pDEBUG ) 
    std::cout << "About to call mainSearch...";
  mainSearch(recColl,geometry);
  if ( debugLevel_ == pDEBUG ) 
    std::cout << "done" << std::endl;
  
  //Hand the basicclusters back to the producer.  It has to 
  //put them in the event.  Then we can make superclusters.
  std::map<int, reco::BasicClusterCollection>::iterator bic; 
  for (bic= clustered_.begin();bic!=clustered_.end();bic++){
    reco::BasicClusterCollection bl = bic->second;
    for (int j=0;j<int(bl.size());++j){
      basicClusters.push_back(bl[j]);
    }
  }

  //Yay more sorting.
  sort(basicClusters.begin(), basicClusters.end());
  //Done!
  if ( debugLevel_ == pDEBUG )
    std::cout << "returning to producer. " << std::endl;
}


void HybridClusterAlgo::mainSearch(const EcalRecHitCollection* hits, const CaloSubdetectorGeometry*geometry)
{
 
  if ( debugLevel_ ==pDEBUG ) {
    std::cout << "HybridClusterAlgo Algorithm - looking for clusters" << std::endl;
    std::cout << "Found the following clusters:" << std::endl;
  }

  // Loop over seeds:
  std::vector<EcalRecHit>::iterator it;
  int clustercounter=0;

  EcalBarrelHardcodedTopology *topo = new EcalBarrelHardcodedTopology();
  for (it = seeds.begin(); it != seeds.end(); it++){
    std::vector <reco::BasicCluster> thisseedClusters;
    DetId itID = it->id();

    // make sure the current seed has not been used/will not be used in the future:
    std::set<DetId>::iterator seed_in_rechits_it = useddetids.find(itID);

    if (seed_in_rechits_it != useddetids.end()) continue;
    //If this seed is already used, then don't use it again.

    // output some info on the hit:
    if ( debugLevel_ == pDEBUG ){
      std::cout << "*****************************************************" << std::endl;
      std::cout << "Seed of energy E = " << it->energy() 
		<< std::endl;
      std::cout << "*****************************************************" << std::endl;
    }

    //Make a navigator, and set it to the seed cell.
    EcalBarrelNavigator navigator(itID, topo);

    //Now use the navigator to start building dominoes.
    
    //Walking positive in phi:
    std::vector <double> dominoEnergyPhiPlus;  //Here I will store the results of the domino sums
    std::vector <std::vector <EcalRecHit> > dominoCellsPhiPlus; //These are the actual EcalRecHit for dominos.
    
    //Walking negative in phi
    std::vector <double> dominoEnergyPhiMinus;  //Here I will store the results of the domino sums
    std::vector <std::vector <EcalRecHit> > dominoCellsPhiMinus; //These are the actual EcalRecHit for dominos.
    
    //The two sets together.
    std::vector <double> dominoEnergy;  //Here I will store the results of the domino sums
    std::vector <std::vector <EcalRecHit> > dominoCells; //These are the actual EcalRecHit for dominos.
    
    //First, the domino about the seed:
    std::vector <EcalRecHit> initialdomino;
    double e_init = makeDomino(navigator, initialdomino);
    
    //Positive phi steps.
    for (int i=0;i<phi_steps;++i){
      //remember, this always increments the current position of the navigator.
      DetId centerD = navigator.north();

      EcalBarrelNavigator dominoNav(centerD, topo);
      
      //Go get the new domino.
      std::vector <EcalRecHit> dcells;
      double etemp = makeDomino(dominoNav, dcells);
      
      //save this information
      dominoEnergyPhiPlus.push_back(etemp);
      dominoCellsPhiPlus.push_back(dcells);
    }

    if ( debugLevel_ == pDEBUG )
      std::cout << "Got positive dominos" << std::endl;
    //return to initial position
    navigator.home();
    
    //Negative phi steps.
    for (int i=0;i<phi_steps;++i){
      //remember, this always decrements the current position of the navigator.
      DetId centerD = navigator.south();
      EcalBarrelNavigator dominoNav(centerD, topo);
      
      //Go get the new domino.
      std::vector <EcalRecHit> dcells;
      double etemp = makeDomino(dominoNav, dcells);
      
      //save this information
      dominoEnergyPhiMinus.push_back(etemp);
      dominoCellsPhiMinus.push_back(dcells);
    }
    
    if ( debugLevel_ == pDEBUG )
      std::cout << "Got negative dominos: " << std::endl;

    //Assemble this information:
    for (int i=int(dominoEnergyPhiMinus.size())-1;i >= 0;--i){
      dominoEnergy.push_back(dominoEnergyPhiMinus[i]);
      dominoCells.push_back(dominoCellsPhiMinus[i]);
    }
    dominoEnergy.push_back(e_init);
    dominoCells.push_back(initialdomino);
    for (int i=0;i<int(dominoEnergyPhiPlus.size());++i){
      dominoEnergy.push_back(dominoEnergyPhiPlus[i]);
      dominoCells.push_back(dominoCellsPhiPlus[i]);
    }

    //Ok, now I have all I need in order to go ahead and make clusters.
    if ( debugLevel_ == pDEBUG ){
      std::cout << "Dumping domino energies: " << std::endl;
      for (int i=0;i<int(dominoEnergy.size());++i){
	std::cout << "Domino: " << i << " E: " << dominoEnergy[i] << std::endl;
      }
    }


    //Identify the peaks in this set of dominos:
    //Peak==a domino whose energy is greater than the two adjacent dominos.
    //thus a peak in the local sense.
    std::vector <int> PeakIndex;
    for (int i=1;i<int(dominoEnergy.size())-1;++i){
      if (dominoEnergy[i] > dominoEnergy[i-1]
	  && dominoEnergy[i] > dominoEnergy[i+1]
	  && dominoEnergy[i] > Eseed){
	PeakIndex.push_back(i);
      }
    }

    if ( debugLevel_ == pDEBUG )
      std::cout << "Found: " << PeakIndex.size() << " peaks." << std::endl;
    
    //Order these peaks by energy:
    for (int i=0;i<int(PeakIndex.size());++i){
      for (int j=0;j<int(PeakIndex.size())-1;++j){
	if (dominoEnergy[PeakIndex[j]] < dominoEnergy[PeakIndex[j+1]]){
	  int ihold = PeakIndex[j+1];
	  PeakIndex[j+1] = PeakIndex[j];
	  PeakIndex[j] = ihold;
	}
      }
    }
    
    std::vector<int> OwnerShip;
    std::vector<double> LumpEnergy;
    for (int i=0;i<int(dominoEnergy.size());++i) OwnerShip.push_back(-1);
    
    //Loop over peaks.  
    for (int i=0;i<int(PeakIndex.size());++i){
      int idxPeak = PeakIndex[i];
      OwnerShip[idxPeak] = i;
      double lump = dominoEnergy[idxPeak];
      //Loop over adjacent dominos at higher phi
      for (int j=idxPeak+1;j<int(dominoEnergy.size());++j){
	if (OwnerShip[j]==-1 && 
	    dominoEnergy[j] > Ethres
	    && dominoEnergy[j] < dominoEnergy[j-1]){
	  OwnerShip[j]= i;
	  lump+=dominoEnergy[j];
	}
	else{
	  break;
	}
      }
      //loop over adjacent dominos at lower phi.  Sum up energy of lumps.
      for (int j=idxPeak-1;j>=0;--j){
	if (OwnerShip[j]==-1 && 
	    dominoEnergy[j] > Ethres
	    && dominoEnergy[j] < dominoEnergy[j+1]){
	  OwnerShip[j]= i;
	  lump+=dominoEnergy[j];
	}
	else{
	  break;
	}
      }
      LumpEnergy.push_back(lump);
    }

    //Make the basic clusters:
    for (int i=0;i<int(PeakIndex.size());++i){
      bool HasSeedCrystal = false;
      //One cluster for each peak.
      std::vector<EcalRecHit> recHits;
      std::vector<DetId> dets;
      int nhits=0;
      for (int j=0;j<int(dominoEnergy.size());++j){	
	if (OwnerShip[j] == i){
	  std::vector <EcalRecHit> temp = dominoCells[j];
	  for (int k=0;k<int(temp.size());++k){
            dets.push_back(temp[k].id());
	    if (temp[k].id()==itID)
	      HasSeedCrystal = true;
	    recHits.push_back(temp[k]);
	    nhits++;
	  }
	}  
      }
      if ( debugLevel_ == pDEBUG ){
	std::cout << "Adding a cluster with: " << nhits << std::endl;
	std::cout << "total E: " << LumpEnergy[i] << std::endl;
	std::cout << "total dets: " << dets.size() << std::endl;
      }

      //Get Calorimeter position
      Point pos = posCalculator_.Calculate_Location(dets,hits,geometry);
 
      double totChi2=0;
      double totE=0;
      std::vector<DetId> usedHits;
      for (int blarg=0;blarg<int(recHits.size());++blarg){
	totChi2 +=0;
	totE+=recHits[blarg].energy();
	usedHits.push_back(recHits[blarg].id());
	useddetids.insert(recHits[blarg].id());
      }
      if (totE>0)
	totChi2/=totE;
      
      thisseedClusters.push_back(reco::BasicCluster(LumpEnergy[i],pos,totChi2,usedHits));
      if (HasSeedCrystal)
	seedClus_.push_back(reco::BasicCluster(LumpEnergy[i],pos,totChi2,usedHits));
    }
    //Make association so that superclusters can be made later.
    clustered_.insert(std::make_pair(clustercounter, thisseedClusters));    
    clustercounter++;
  }//Seed loop
  delete topo;
}

reco::SuperClusterCollection HybridClusterAlgo::makeSuperClusters(const reco::BasicClusterRefVector& clustersCollection)
{
  //Here's what we'll return.
  reco::SuperClusterCollection SCcoll;

  //Here's our map iterator that gives us the appropriate association.
  std::map<int, reco::BasicClusterCollection>::iterator mapit;
  for (mapit = clustered_.begin();mapit!=clustered_.end();mapit++){
 
    reco::BasicClusterRefVector thissc;
    reco::BasicClusterRef seed;//This is not really a seed, but I need to tell SuperCluster something.
                               //So I choose the highest energy basiccluster in the SuperCluster.
    
    std::vector <reco::BasicCluster> thiscoll = mapit->second; //This is the set of BasicClusters in this
                                                               //SuperCluster

    double ClusterE =0; //Sum of cluster energies for supercluster.
    //Holders for position of this supercluster.
    double posX=0;
    double posY=0;
    double posZ=0;

    //Loop over this set of basic clusters, find their references, and add them to the
    //supercluster.  This could be somehow more efficient.
    for (int i=0;i<int(thiscoll.size());++i){
      reco::BasicCluster thisclus = thiscoll[i]; //The Cluster in question.
      for (int j=0;j<int(clustersCollection.size());++j){
	//Find the appropriate cluster from the list of references
	reco::BasicCluster cluster_p = *clustersCollection[j];
	if (thisclus== cluster_p){ //Comparison based on energy right now.
	  thissc.push_back(clustersCollection[j]);
	  bool isSeed = false;
	  for (int qu=0;qu<int(seedClus_.size());++qu){
	    if (cluster_p == seedClus_[qu])
	      isSeed = true;
	  }
	  if (isSeed) seed = clustersCollection[j];

	  ClusterE += cluster_p.energy();
	  posX += cluster_p.energy() * cluster_p.position().X();
	  posY += cluster_p.energy() * cluster_p.position().Y();
	  posZ += cluster_p.energy() * cluster_p.position().Z();
								     
	}
      }//End loop over finding references.
    }//End loop over clusters.
    posX /= ClusterE;
    posY /= ClusterE;
    posZ /= ClusterE;
    reco::SuperCluster suCl(ClusterE, math::XYZPoint(posX, posY, posZ), seed, thissc);
    SCcoll.push_back(suCl);

    if ( debugLevel_ == pDEBUG ){
      std::cout << "Super cluster sum: " << ClusterE << std::endl;
      std::cout << "Made supercluster with energy E: " << suCl.energy() << std::endl;
    }
  }//end loop over map
  sort(SCcoll.begin(), SCcoll.end());
  return SCcoll;
}

double HybridClusterAlgo::makeDomino(EcalBarrelNavigator &navigator, std::vector <EcalRecHit> &cells)
{
  //At the beginning of this function, the navigator starts at the middle of the domino,
  //and that's where EcalBarrelNavigator::home() should send it.
  //Walk one crystal in eta to either side of the initial point.  Sum the three cell energies.
  //If the resultant energy is larger than Ewing, then go an additional cell to either side.
  //Returns:  Total domino energy.  Also, stores the cells used to create domino in the vector.
  cells.clear();
  double Etot = 0;

  //Ready?  Get the starting cell.
  DetId center = navigator.pos();
  EcalRecHitCollection::const_iterator center_it = recHits_->find(center);
  
  if (center_it!=recHits_->end()){
    EcalRecHit SeedHit = *center_it;
    if (useddetids.find(center) == useddetids.end()){ 
      Etot += SeedHit.energy();
      cells.push_back(SeedHit);
    }
  }
  //One step upwards in Ieta:
  DetId ieta1 = navigator.west();
  EcalRecHitCollection::const_iterator eta1_it = recHits_->find(ieta1);
  if (eta1_it !=recHits_->end()){
    EcalRecHit UpEta = *eta1_it;
    if (useddetids.find(ieta1) == useddetids.end()){
      Etot+=UpEta.energy();
      cells.push_back(UpEta);
    }
  }
  
  //Go back to the middle.
  navigator.home();
  
  //One step downwards in Ieta:
  DetId ieta2 = navigator.east();
  EcalRecHitCollection::const_iterator eta2_it = recHits_->find(ieta2);
  if (eta2_it !=recHits_->end()){
    EcalRecHit DownEta = *eta2_it;
    if (useddetids.find(ieta2)==useddetids.end()){
      Etot+=DownEta.energy();
      cells.push_back(DownEta);
    }
  }
  
  //Now check the energy.  If smaller than Ewing, then we're done.  If greater than Ewing, we have to
  //add two additional cells, the 'wings'
  if (Etot < Ewing) return Etot;  //Done!  Not adding 'wings'.
  
  //Add the extra 'wing' cells.  Remember, we haven't sent the navigator home,
  //we're still on the DownEta cell.
  if (ieta2 != DetId(0)){
    DetId ieta3 = navigator.east(); //Take another step downward.
    EcalRecHitCollection::const_iterator eta3_it = recHits_->find(ieta3);
    if (eta3_it != recHits_->end()){
      EcalRecHit DownEta2 = *eta3_it;
      if (useddetids.find(ieta3)==useddetids.end()){
        Etot+=DownEta2.energy();
        cells.push_back(DownEta2);
      }
    }
  }
  //Now send the navigator home.
  navigator.home();
  navigator.west(); //Now you're on eta1_it
  if (ieta1 != DetId(0)){
    DetId ieta4 = navigator.west(); //Take another step upward.
    EcalRecHitCollection::const_iterator eta4_it = recHits_->find(ieta4);
    if (eta4_it != recHits_->end()){
      EcalRecHit UpEta2 = *eta4_it;
      if (useddetids.find(ieta4) == useddetids.end()){
        Etot+=UpEta2.energy();
        cells.push_back(UpEta2);
      }
    }
  }
  navigator.home();
  return Etot;
}

