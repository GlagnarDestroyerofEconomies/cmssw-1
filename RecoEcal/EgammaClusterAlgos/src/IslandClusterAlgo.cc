#include "RecoEcal/EgammaClusterAlgos/interface/IslandClusterAlgo.h"

// Geometry
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloTopology/interface/EcalEndcapTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelTopology.h"

#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"

//

// Return a vector of clusters from a collection of EcalRecHits:
std::vector<reco::BasicCluster> IslandClusterAlgo::makeClusters(EcalRecHitCollection const *the_rechitsMap_p,
                                                                const CaloSubdetectorGeometry *geometry_p,
                                                                const CaloSubdetectorTopology *topology_p,
                                                                EcalPart ecalPart)
{
  seeds.clear();
  used_s.clear();
  clusters_v.clear();

  rechitsMap_p = the_rechitsMap_p;

  double threshold = 0;
  std::string ecalPart_string;
  if (ecalPart == endcap) 
    {
      threshold = ecalEndcapSeedThreshold;
      ecalPart_string = "EndCap";
    }
  if (ecalPart == barrel) 
    {
      threshold = ecalBarrelSeedThreshold;
      ecalPart_string = "Barrel";
    }

  if (verbosity < pINFO)
    {
      std::cout << "-------------------------------------------------------------" << std::endl;
      std::cout << "Island algorithm invoked for ECAL" << ecalPart_string << std::endl;
      std::cout << "Looking for seeds, energy threshold used = " << threshold << " GeV" <<std::endl;
    }

  EcalRecHitCollection::const_iterator it;
  for(it = rechitsMap_p->begin(); it != rechitsMap_p->end(); it++)
    {
      double energy = it->energy();
      if (energy < threshold) continue; // need to check to see if this line is useful!

      const CaloCellGeometry *thisCell = geometry_p->getGeometry(it->id());
      GlobalPoint position = thisCell->getPosition();
      float ET = it->energy() * sin(position.theta());

      if (ET > threshold) seeds.push_back(*it);
    }
  sort(seeds.begin(), seeds.end(), ecalRecHitLess());

  if (verbosity < pINFO)
    {
      std::cout << "Total number of seeds found in event = " << seeds.size() << std::endl;
    }

  mainSearch(ecalPart, topology_p);
  sort(clusters_v.begin(), clusters_v.end());

  if (verbosity < pINFO)
    {
      std::cout << "-------------------------------------------------------------" << std::endl;
    }
  
  return clusters_v; 
}


void IslandClusterAlgo::mainSearch(EcalPart ecalPart, const CaloSubdetectorTopology *topology_p)
{
  if (verbosity < pINFO)
    {
      std::cout << "Building clusters............" << std::endl;
    }

  // Loop over seeds:
  std::vector<EcalRecHit>::iterator it;
  for (it = seeds.begin(); it != seeds.end(); it++)
    {
      // make sure the current seed does not belong to a cluster already.
      if (used_s.find(it->id()) != used_s.end())
	{
	  if (it == seeds.begin())
	    {
	      if (verbosity < pINFO)
		{
		  std::cout << "##############################################################" << std::endl;
		  std::cout << "DEBUG ALERT: Highest energy seed already belongs to a cluster!" << std::endl;
		  std::cout << "##############################################################" << std::endl;
		}
	    }
	  continue;
	}

      // clear the vector of hits in current cluster
      current_v.clear();

      current_v.push_back(it->id());
      used_s.insert(it->id());

      // Create a navigator at the seed
      CaloNavigator<DetId> navigator(it->id(), topology_p);

      searchNorth(navigator);
      navigator.home();
      searchSouth(navigator);
      navigator.home();
      searchWest(navigator, topology_p);
      navigator.home();
      searchEast(navigator, topology_p);
 
      makeCluster();
   }
}


void IslandClusterAlgo::searchNorth(const CaloNavigator<DetId> &navigator)
{
  DetId southern = navigator.pos();
  EcalRecHitCollection::const_iterator southern_it = rechitsMap_p->find(southern);

  DetId northern = navigator.north();
  if (northern == DetId(0)) return; // This means that we went off the ECAL!
  EcalRecHitCollection::const_iterator northern_it = rechitsMap_p->find(northern);

  // if the crystal to the north belongs to another cluster return
  if (used_s.find(northern_it->id()) != used_s.end()) return;

  if (shouldBeAdded(northern_it, southern_it))
    {
      current_v.push_back(northern);
      used_s.insert(northern_it->id());
      searchNorth(navigator);
    }
}


void IslandClusterAlgo::searchSouth(const CaloNavigator<DetId> &navigator)
{
  DetId northern = navigator.pos();
  EcalRecHitCollection::const_iterator northern_it = rechitsMap_p->find(northern);

  DetId southern = navigator.south();
  if (southern == DetId(0)) return; // This means that we went off the ECAL!
  EcalRecHitCollection::const_iterator southern_it = rechitsMap_p->find(southern);

  if (used_s.find(southern_it->id()) != used_s.end()) return;

  if (shouldBeAdded(southern_it, northern_it))
    {
      current_v.push_back(southern);
      used_s.insert(southern_it->id());
      searchSouth(navigator);
    }
}


void IslandClusterAlgo::searchWest(const CaloNavigator<DetId> &navigator, const CaloSubdetectorTopology* topology)
{
  DetId eastern = navigator.pos();
  EcalRecHitCollection::const_iterator eastern_it = rechitsMap_p->find(eastern);

  DetId western = navigator.west();
  if (western == DetId(0)) return; // This means that we went off the ECAL!
  EcalRecHitCollection::const_iterator western_it = rechitsMap_p->find(western);

  if (shouldBeAdded(western_it, eastern_it))
    {
      CaloNavigator<DetId> nsNavigator(western, topology);

      searchNorth(nsNavigator);
      nsNavigator.home();
      searchSouth(nsNavigator);
      nsNavigator.home();
      searchWest(navigator, topology);

      current_v.push_back(western);
      used_s.insert(western_it->id());
    }
}


void IslandClusterAlgo::searchEast(const CaloNavigator<DetId> &navigator, const CaloSubdetectorTopology* topology)
{
  DetId western = navigator.pos();
  EcalRecHitCollection::const_iterator western_it = rechitsMap_p->find(western);

  DetId eastern = navigator.east();
  if (eastern == DetId(0)) return; // This means that we went off the ECAL!
  EcalRecHitCollection::const_iterator eastern_it = rechitsMap_p->find(eastern);

  if (shouldBeAdded(eastern_it, western_it))
    {
      CaloNavigator<DetId> nsNavigator(eastern, topology);

      searchNorth(nsNavigator);
      nsNavigator.home();
      searchSouth(nsNavigator);
      nsNavigator.home();
      searchEast(navigator, topology);

      current_v.push_back(eastern);
      used_s.insert(eastern_it->id());
    }
}


// returns true if the candidate crystal fulfills the requirements to be added to the cluster:
bool IslandClusterAlgo::shouldBeAdded(EcalRecHitCollection::const_iterator candidate_it, EcalRecHitCollection::const_iterator previous_it)
{
  // crystal should not be included...
  if ((used_s.find(candidate_it->id()) != used_s.end()) || // ...if it already belongs to a cluster
      (candidate_it == rechitsMap_p->end())              || // ...if it corresponds to a hit
      (candidate_it->energy() <= 0)               || // ...if it has a negative or zero energy
      (candidate_it->energy() > previous_it->energy())) // ...or if the previous crystal had lower E
    {
      return false;
    }
  return true;
}


void IslandClusterAlgo::makeCluster()
{
  double energy = 0;
  double chi2   = 0;

  Point position;
  position = PositionCalc::Calculate_Location(current_v);
  
  std::vector<DetId>::iterator it;
  for (it = current_v.begin(); it != current_v.end(); it++)
    {
      EcalRecHitCollection::const_iterator itt = rechitsMap_p->find(*it);
      EcalRecHit hit_p = *itt;
      //      if (hit_p != 0)
      //	{
	  energy += hit_p.energy();
      //	}
      //      else 
      //	{
      //	  std::cout << "DEBUG ALERT: Requested rechit has gone missing from rechits map! :-S" << std::endl;
      //	}
      chi2 += 0;
    }
  chi2 /= energy;

  if (verbosity < pINFO)
    { 
      std::cout << "******** NEW CLUSTER ********" << std::endl;
      std::cout << "No. of crystals = " << current_v.size() << std::endl;
      std::cout << "     Energy     = " << energy << std::endl;
      std::cout << "     Phi        = " << position.phi() << std::endl;
      std::cout << "     Eta        = " << position.eta() << std::endl;
      std::cout << "*****************************" << std::endl;
    }

  clusters_v.push_back(reco::BasicCluster(energy, position, chi2, current_v, reco::island));
}
