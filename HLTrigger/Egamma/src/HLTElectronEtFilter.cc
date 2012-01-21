/** \class HLTElectronEtFilter
 *
 *
 *  \author Alessio Ghezzi
 *
 */

#include "HLTrigger/Egamma/interface/HLTElectronEtFilter.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronIsolationAssociation.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

//
// constructors and destructor
//
HLTElectronEtFilter::HLTElectronEtFilter(const edm::ParameterSet& iConfig) : HLTFilter(iConfig) {
  candTag_ = iConfig.getParameter< edm::InputTag > ("candTag");
  EtEB_ = iConfig.getParameter<double> ("EtCutEB");
  EtEE_ = iConfig.getParameter<double> ("EtCutEE");
  
  ncandcut_  = iConfig.getParameter<int> ("ncandcut");
  doIsolated_ = iConfig.getParameter<bool> ("doIsolated");

  L1IsoCollTag_= iConfig.getParameter< edm::InputTag > ("L1IsoCand"); 
  L1NonIsoCollTag_= iConfig.getParameter< edm::InputTag > ("L1NonIsoCand"); 
}

HLTElectronEtFilter::~HLTElectronEtFilter(){}


// ------------ method called to produce the data  ------------
bool HLTElectronEtFilter::hltFilter(edm::Event& iEvent, const edm::EventSetup& iSetup, trigger::TriggerFilterObjectWithRefs & filterproduct)
{
  using namespace trigger;
  if (saveTags()) {
    filterproduct.addCollectionTag(L1IsoCollTag_);
    if (not doIsolated_) filterproduct.addCollectionTag(L1NonIsoCollTag_);
  }

  // Ref to Candidate object to be recorded in filter object
  reco::ElectronRef ref;

  edm::Handle<trigger::TriggerFilterObjectWithRefs> PrevFilterOutput;

  iEvent.getByLabel (candTag_,PrevFilterOutput);

  std::vector<edm::Ref<reco::ElectronCollection> > elecands;
  PrevFilterOutput->getObjects(TriggerElectron, elecands);

  
    
  // look at all photons, check cuts and add to filter object
  int n = 0;
  
  for (unsigned int i=0; i<elecands.size(); i++) {
    
    ref = elecands[i];
    float Pt = ref->pt();
    float Eta = fabs(ref->eta());
    
    if ( (Eta < 1.479 && Pt > EtEB_) || (Eta >= 1.479 && Pt > EtEE_) ) {
      n++;
      filterproduct.addObject(TriggerElectron, ref);
    }
    
  }  
  // filter decision
  bool accept(n>=ncandcut_);
  
  return accept;
}
