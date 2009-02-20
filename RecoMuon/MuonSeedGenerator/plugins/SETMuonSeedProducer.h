#ifndef RecoMuon_MuonSeedGenerator_SETMuonSeedProducer_H
#define RecoMuon_MuonSeedGenerator_SETMuonSeedProducer_H

/** \class SETMuonSeedProducer 
     I. Bloch, E. James, S. Stoynev
  */

#include "FWCore/Framework/interface/EDProducer.h"
#include "RecoMuon/TrackingTools/interface/RecoMuonEnumerators.h"

#include <RecoMuon/TrackingTools/interface/MuonServiceProxy.h>

#include "RecoMuon/MuonSeedGenerator/src/SETFilter.h"
#include "RecoMuon/TransientTrackingRecHit/interface/MuonTransientTrackingRecHit.h" 
#include "RecoMuon/MuonSeedGenerator/src/SETPatternRecognition.h"
#include "RecoMuon/MuonSeedGenerator/src/SETSeedFinder.h"

class TrajectorySeed;
class STAFilter;

#include "FWCore/Framework/interface/Frameworkfwd.h"

class SETMuonSeedProducer : public edm::EDProducer {
  
 public:
  typedef MuonTransientTrackingRecHit::MuonRecHitContainer MuonRecHitContainer;
  typedef std::vector<Trajectory*> TrajectoryContainer;

  /// Constructor with Parameter set 
  SETMuonSeedProducer (const edm::ParameterSet&);
  
  /// Destructor
  virtual ~SETMuonSeedProducer();
  
  // Operations
  virtual void produce(edm::Event&, const edm::EventSetup&);
  
 protected:

 private:
  
  
  // Returns a vector of measurements sets (for later trajectory seed building)
  std::vector < std::pair < TrajectoryStateOnSurface, 
    TransientTrackingRecHit::ConstRecHitContainer > > trajectories(const edm::Event&, const edm::EventSetup& eventSetup);

  /// pre-filter
  SETFilter* filter() const {return theFilter;}
  
  std::vector <seedSet>  fillSeedSets(std::vector <MuonRecHitContainer> & allValidSets);
  //----

  //private:
  
  SETFilter* theFilter;
  void setEvent(const edm::Event&);
 
  //---- SET
  bool apply_prePruning;
  bool useSegmentsInTrajectory;
  MuonServiceProxy *theService;

  SETPatternRecognition thePatternRecognition;
  SETSeedFinder theSeedFinder;
};
#endif
