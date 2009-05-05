#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <Math/VectorUtil.h>
#include <TMath.h>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpTrackJets.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/JetReco/interface/BasicJetCollection.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "SimDataFormats/JetMatching/interface/JetFlavour.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "SimDataFormats/JetMatching/interface/MatchedPartons.h"
#include "SimDataFormats/JetMatching/interface/JetMatchedPartons.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaJet.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/getRef.h"


// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;
using namespace reco;





// ----------------------------------------------------------------------
HFDumpTrackJets::HFDumpTrackJets(const edm::ParameterSet& iConfig):
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fJetsLabel(iConfig.getUntrackedParameter<string>("jetsLabel", string("ic5TrackJets"))),
  fTracksLabel(iConfig.getUntrackedParameter<string>("tracksLabel", string("trackCandidates"))),
  fGenCandidatesLabel(iConfig.getUntrackedParameter<string>("generatorCandidates", string("genParticles"))), 
  fsourceByRefer(iConfig.getParameter<edm::InputTag>("sourceByRefer"))
  
 
{
  
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpTrackJets constructor" << endl;
  cout << "--- " << fJetsLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;
  
}


// ----------------------------------------------------------------------
HFDumpTrackJets::~HFDumpTrackJets() {
  
}


// ----------------------------------------------------------------------
void HFDumpTrackJets::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  
  nevt++;

  // handle to 0.5 cone ctf track jets  
  Handle<BasicJetCollection> jetsH;
  iEvent.getByLabel(fJetsLabel.c_str(),jetsH);
  const BasicJetCollection *jets   = jetsH.product();  

  if (fVerbose > 0) cout << "==>HFDumpTrackJets> nJets =" << jets->size() << endl;

  //tracks (jet constituents)
  Handle<reco::CandidateView> candidates1Handle;
  iEvent.getByLabel(fTracksLabel.c_str(), candidates1Handle); 

  std::vector<const Candidate *> trkcands;
  trkcands.clear();
  vector<const Candidate *>::const_iterator trkfound = trkcands.begin(); 

  //get tracks 
  for (unsigned int i = 0; i < candidates1Handle->size(); ++ i ) {
    const Candidate &p2 = (*candidates1Handle)[i];
    const Candidate *  p1 = &p2;
    trkcands.push_back( & * p1 );
  }

  //////////////////////////////////////////
  // -- get the collection for flavor matching
  edm::Handle<reco::JetMatchedPartonsCollection> theTagByRef;
  iEvent.getByLabel (fsourceByRefer , theTagByRef);

  // -- get the collection of GenParticles 
  edm::Handle<GenParticleCollection> genParticlesH;
  genParticlesH.clear();
  iEvent.getByLabel (fGenCandidatesLabel.c_str(), genParticlesH);
 
  //GenParticles for Jet Flavor Tagging
  std::vector<const GenParticle *> cands;
  cands.clear();
  vector<const GenParticle *>::const_iterator found = cands.begin();
  for (GenParticleCollection::const_iterator p = genParticlesH->begin(); p != genParticlesH->end(); ++p) {
    cands.push_back( & * p );
  }

  if (jets->size() != theTagByRef->size()) {
    if (fVerbose > 0) cout << "==>HFDumpTrackJets>ERROR: Different Size of JetCollections " << endl; 
  }

  ///////////////////////////////////////////
 

 

  int jetIndex=0; 
  TAnaJet *pTrackJet; 

  //for(BasicJetCollection::const_iterator it= jets->begin();it != jets->end(); it++){
  BasicJetCollection::const_iterator it = jets->begin();
  for ( JetMatchedPartonsCollection::const_iterator j  = theTagByRef->begin(); j != theTagByRef->end(); j ++ ) {
  
    const Jet *aJet             = (*j).first.get();
    const MatchedPartons aMatch = (*j).second; 

    if (it->eta() != aJet->eta()) {
      if (fVerbose > 0) cout << "==>HFDumpTrackJets>ERROR: Different jets in JetCollections " << endl; 
    }
    
    pTrackJet = gHFEvent->addTrackJet();
    pTrackJet->fIndex            = jetIndex;  

    pTrackJet->fPlab.SetPtEtaPhi(it->pt(),
				it->eta(),
				it->phi()
				);
 
    pTrackJet->fQ                = it->charge();
    pTrackJet->fE                = it->energy();
    pTrackJet->fEt               = it->et();
    pTrackJet->fM                = it->mass();
    pTrackJet->fMt               = it->mt();

    pTrackJet->fEMEnergy         = -1;//not used
    pTrackJet->fHADEnergy        = -1;//not used
    pTrackJet->finvisibleEnergy  = -1;//not used

    pTrackJet->fn60              = -9999;//not used
    pTrackJet->fn90              = -9999;//not used
  
    pTrackJet->fJetFlavorHeavy   = -9999;
    pTrackJet->fJetFlavorNear2   = -9999;
    pTrackJet->fJetFlavorNear3   = -9999; 
    pTrackJet->fJetFlavorAlgo    = -9999;
    pTrackJet->fJetFlavorPhys    = -9999;
    pTrackJet->fJetFlavorEne     = -9999;
  
    pTrackJet->fD1               = -9999;//not used
    pTrackJet->fD2               = -9999;//not used
    pTrackJet->fD4               = -9999;//not used
    pTrackJet->fD5               = -9999;//not used
    pTrackJet->fD6               = -9999;//not used
    pTrackJet->fD7               = -9999;//not used  


    //jet constituents
    std::vector< const reco::Candidate * > Constituent = it->getJetConstituentsQuick();
    for (unsigned int i=0; i< Constituent.size(); i++) {

      int index  = -1;
      const reco::Candidate * consti = Constituent[i];
      if (consti) {
	trkfound = find( trkcands.begin(), trkcands.end(), consti );
	if ( trkfound != trkcands.end() ) index = trkfound - trkcands.begin() ;
	pTrackJet->addTrack(index);
	//if (fVerbose) cout << index << " pt " << consti->pt() << " phi " << consti->phi() << " eta " << consti->eta() << endl;
      } 
 
    } 


    //jet tag
    const GenParticleRef theHeaviest = aMatch.heaviest() ;
    if(theHeaviest.isNonnull()) {
      int index = -1;
      found = find(cands.begin(), cands.end(), theHeaviest.get());
      if (found != cands.end()) {
	index = found - cands.begin();
	pTrackJet->fJetFlavorHeavy = index;
      } 
      if (fVerbose > 0) {
	cout << "theHeaviest flav idx (p,eta,phi)= " 
	     << theHeaviest.get()->pdgId() << " " 
	     << index
	     << " (" << theHeaviest.get()->p() 
	     << ","  << theHeaviest.get()->eta() 
	     << ","  << theHeaviest.get()->phi() << ") " << endl; 
      }
    }
    const GenParticleRef theNearest2 = aMatch.nearest_status2() ;
    if(theNearest2.isNonnull()) {
      int index = -1;
      found = find(cands.begin(), cands.end(), theNearest2.get());
      if (found != cands.end()) {
	index = found - cands.begin();
	pTrackJet->fJetFlavorNear2 = index;
      }
      if (fVerbose > 0) {
	cout << "theNearest Stat2  flav idx (p,eta,phi)= " 
	     << theNearest2.get()->pdgId() << " " 
	     << index
	     << " (" << theNearest2.get()->p() 
	     << ","  << theNearest2.get()->eta() 
	     << ","  << theNearest2.get()->phi() << ") " << endl; 
      }
    }
    const GenParticleRef theNearest3 = aMatch.nearest_status3() ;
    if(theNearest3.isNonnull()) {
      int index = -1;
      found = find(cands.begin(), cands.end(), theNearest3.get());
      if (found != cands.end()) {
	index = found - cands.begin();
	pTrackJet->fJetFlavorNear3 = index;
      }
      if (fVerbose > 0) {
	cout << "theNearest Stat3  flav idx (p,eta,phi)= " 
	     << theNearest3.get()->pdgId() << " " 
	     << index
	     << " (" << theNearest3.get()->p() 
	     << ","  << theNearest3.get()->eta() 
	     << ","  << theNearest3.get()->phi() << ") " << endl; 
      }
    }
    const GenParticleRef thePhyDef = aMatch.physicsDefinitionParton() ;
    if(thePhyDef.isNonnull()) {
      int index = -1;
      found = find(cands.begin(), cands.end(), thePhyDef.get());
      if (found != cands.end()) {
	index = found - cands.begin();
	pTrackJet->fJetFlavorPhys = index;
      }
      if (fVerbose > 0) {
	cout << "thePhysDefinition flav idx (p,eta,phi)= " 
	     << thePhyDef.get()->pdgId() << " " 
	     << index 
	     << " (" << thePhyDef.get()->p() 
	     << ","  << thePhyDef.get()->eta() 
	     << ","  << thePhyDef.get()->phi() << ") " << endl; 
      }
     
    }
    const GenParticleRef theAlgDef = aMatch.algoDefinitionParton() ;
    if(theAlgDef.isNonnull()) {
      int index = -1;
      found = find(cands.begin(), cands.end(), theAlgDef.get());
      if (found != cands.end()) {
	index = found - cands.begin();
	pTrackJet->fJetFlavorAlgo = index;
      }
      if (fVerbose > 0) {
	cout << "theAlgoDefinition flav idx (p,eta,phi)= " 
	     << theAlgDef.get()->pdgId() << " " 
	     << index 
	     << " (" << theAlgDef.get()->p() 
	     << ","  << theAlgDef.get()->eta() 
	     << ","  << theAlgDef.get()->phi() << ") " << endl; 
      } 
      
    }

    if (fVerbose > 0) pTrackJet->dump();
    jetIndex++;
    it++;
  }

 



}

// ------------ method called once each job just before starting event loop  ------------
void  HFDumpTrackJets::beginJob(const edm::EventSetup& setup) {
  nevt=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpTrackJets::endJob() { 
  cout << "HFDumpJet>    Summary: Events processed: " << nevt << endl;
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpTrackJets);
