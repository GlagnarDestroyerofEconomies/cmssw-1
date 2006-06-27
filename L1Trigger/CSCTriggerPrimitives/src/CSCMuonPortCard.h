#ifndef CSCTriggerPrimitives_CSCMuonPortCard_h
#define CSCTriggerPrimitives_CSCMuonPortCard_h

/**
 * \class CSCMuonPortCard
 *
 * Simulates the functionality of the Muon Port Card (MPC).  Each MPC
 * is responsible for 9 Trigger Mother Boards (TMBs).  It takes the up to
 * 18 LCTs (2/TMB) in each (sub)sector every bunch crossing, sorts them,
 * selects up to three best, and puts them into an output collection.
 *
 * A port/reworking of the muon port card from ORCA.
 *
 * \author L. Gray (UF)
 *
 **/

#include <vector>
#include <DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h>
#include <DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h>
#include <L1Trigger/CSCCommonTrigger/interface/CSCTriggerContainer.h>
#include <L1Trigger/CSCCommonTrigger/interface/CSCTrackStub.h>


class CSCMuonPortCard
{
 public:
  CSCMuonPortCard() {};

  typedef CSCTriggerContainer<CSCTrackStub> TrackStubList;


  // Method to load the content of the digi container into a trigger
  // container.  This allows us to sort per BX more easily.
  void loadDigis(const CSCCorrelatedLCTDigiCollection& thedigis);

  // Method to sort all Correlated LCTs generated by the TMB.
  // Returns a vector of TrackStubs indexed by [sorting]
  std::vector<CSCTrackStub> sort(const unsigned endcap, const unsigned station,
				 const unsigned sector, const unsigned subsector,
				 const int bx);

  void clear() { _stubs.clear(); }

 private:
  CSCTriggerContainer<CSCTrackStub> _stubs;
};

#endif
