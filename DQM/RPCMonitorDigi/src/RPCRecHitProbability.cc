#include <sstream>
#include <TMath.h>
#include "DQM/RPCMonitorDigi/interface/RPCRecHitProbability.h"
///Data Format
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/MuonReco/interface/Muon.h"
//Geometry
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
//Tracking Tools
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
//FW Core
#include "FWCore/MessageLogger/interface/MessageLogger.h"
//Reco Muon
#include "RecoMuon/TransientTrackingRecHit/interface/MuonTransientTrackingRecHit.h"

RPCRecHitProbability::RPCRecHitProbability( const edm::ParameterSet& pset ):counter(0){
  
  saveRootFile  = pset.getUntrackedParameter<bool>("SaveRootFile", false); 
  RootFileName  = pset.getUntrackedParameter<std::string>("RootFileName", "RPCRecHitProbabilityDQM.root"); 

  muonLabel_ = pset.getParameter<edm::InputTag>("MuonLabel");
  muPtCut_  = pset.getUntrackedParameter<double>("MuonPtCut", 3.0); 
  muEtaCut_ = pset.getUntrackedParameter<double>("MuonEtaCut", 1.6); 
 
  subsystemFolder_ = pset.getUntrackedParameter<std::string>("RPCFolder", "RPC");
  globalFolder_ = pset.getUntrackedParameter<std::string>("GlobalFolder", "SummaryHistograms");
  muonFolder_ = pset.getUntrackedParameter<std::string>("MuonFolder", "Muon");
}

RPCRecHitProbability::~RPCRecHitProbability(){}
void RPCRecHitProbability::beginJob(){}

void RPCRecHitProbability::beginRun(const edm::Run& r, const edm::EventSetup& iSetup){

  edm::LogInfo ("rpcrechitprobability") <<"[RPCRecHitProbability]: Begin Run " ;
  
  /// get hold of back-end interface
  dbe = edm::Service<DQMStore>().operator->();

  std::string  currentFolder = subsystemFolder_ +"/"+muonFolder_+"/"+ globalFolder_;
  dbe->setCurrentFolder(currentFolder); 

  int ptBins = 100 - (int)muPtCut_;

  //General part
  NumberOfMuonEta_ = dbe->get(currentFolder+"/NumberOfMuonEta");
  if(NumberOfMuonEta_) dbe->removeElement(NumberOfMuonEta_->getName());
  NumberOfMuonEta_ = dbe->book1D("NumberOfMuonEta", "Muons vs Eta", 32, -1.6, 1.6);

  NumberOfMuonPt_B_ = dbe->get(currentFolder+"/NumberOfMuonPt_Barrel");
  if(NumberOfMuonPt_B_) dbe->removeElement(NumberOfMuonPt_B_->getName());
  NumberOfMuonPt_B_ = dbe->book1D("NumberOfMuonPt_Barrel", "Muons vs Pt - Barrel", ptBins, muPtCut_, 100);

  NumberOfMuonPt_EP_ = dbe->get(currentFolder+"/NumberOfMuonPt_EndcapP");
  if(NumberOfMuonPt_EP_) dbe->removeElement(NumberOfMuonPt_EP_->getName());
  NumberOfMuonPt_EP_ = dbe->book1D("NumberOfMuonPt_EndcapP", "Muons vs Pt - Endcap+", ptBins , muPtCut_ , 100);

  NumberOfMuonPt_EM_ = dbe->get(currentFolder+"/NumberOfMuonPt_EndcapM");
  if(NumberOfMuonPt_EM_) dbe->removeElement(NumberOfMuonPt_EM_->getName());
  NumberOfMuonPt_EM_ = dbe->book1D("NumberOfMuonPt_EndcapM", "Muons vs Pt - Endcap-", ptBins , muPtCut_ , 100);

  NumberOfMuonPhi_B_ = dbe->get(currentFolder+"/NumberOfMuonPhi_Barrel");
  if(NumberOfMuonPhi_B_) dbe->removeElement(NumberOfMuonPhi_B_->getName());
  NumberOfMuonPhi_B_ = dbe->book1D("NumberOfMuonPhi_Barrel", "Muons vs Phi - Barrel", 144, -TMath::Pi(), TMath::Pi());

  NumberOfMuonPhi_EP_ = dbe->get(currentFolder+"/NumberOfMuonPhi_EndcapP");
  if(NumberOfMuonPhi_EP_) dbe->removeElement(NumberOfMuonPhi_EP_->getName());
  NumberOfMuonPhi_EP_ = dbe->book1D("NumberOfMuonPhi_EndcapP", "Muons vs Phi - Endcap+", 144,  -TMath::Pi(), TMath::Pi() );

  NumberOfMuonPhi_EM_ = dbe->get(currentFolder+"/NumberOfMuonPhi_EndcapM");
  if(NumberOfMuonPhi_EM_) dbe->removeElement(NumberOfMuonPhi_EM_->getName());
  NumberOfMuonPhi_EM_ = dbe->book1D("NumberOfMuonPhi_EndcapM", "Muons vs Phi - Endcap-", 144, -TMath::Pi(), TMath::Pi());

  //RPC part
  RPCRecHitMuonEta_ = dbe->get(currentFolder+"/RPCRecHitMuonEta");
  if(RPCRecHitMuonEta_) dbe->removeElement(RPCRecHitMuonEta_->getName());
  RPCRecHitMuonEta_ = dbe->book2D("RPCRecHitMuonEta", "Number Of RecHits per Muons vs Eta", 32, -1.6, 1.6, 7, 0.5, 7.5);
  
  std::stringstream name, title;
  for(int i = 0 ; i< 6 ; i++) {
    name.str("");
    title.str("");
    name<<(i+1)<<"RecHitMuonEta";
    title<<"At least " <<(i+1)<<" Cluster vs Eta";
    recHitEta_[i] = dbe->book1D(name.str(), title.str(), 32, -1.6, 1.6);

    name.str("");
    title.str("");
    name<<(i+1)<<"RecHitMuonPhiB";
    title<<"At least " <<(i+1)<<" Cluster vs Phi-Barrel";
    recHitPhi_B_[i] = dbe->book1D(name.str(), title.str(), 144,  -TMath::Pi(), TMath::Pi());

    name.str("");
    title.str("");
    name<<(i+1)<<"RecHitMuonPtB";
    title<<"At least " <<(i+1)<<" Cluster vs Pt-Barrel";
    recHitPt_B_[i] = dbe->book1D(name.str(), title.str(), ptBins , muPtCut_ , 100);

    name.str("");
    title.str("");
    name<<(i+1)<<"RecHitMuonPhiEP";
    title<<"At least " <<(i+1)<<" Cluster vs Phi-Endcap+";
    recHitPhi_EP_[i] = dbe->book1D(name.str(), title.str(), 144, -TMath::Pi(), TMath::Pi() );

    name.str("");
    title.str("");
    name<<(i+1)<<"RecHitMuonPtEP";
    title<<"At least " <<(i+1)<<" Cluster vs Pt-Endcap+";
    recHitPt_EP_[i] = dbe->book1D(name.str(), title.str(), ptBins , muPtCut_ , 100);

    name.str("");
    title.str("");
    name<<(i+1)<<"RecHitMuonPhiEM";
    title<<"At least " <<(i+1)<<" Cluster vs Phi-Endcap-";
    recHitPhi_EM_[i] = dbe->book1D(name.str(), title.str(), 144, -TMath::Pi(), TMath::Pi());

    name.str("");
    title.str("");
    name<<(i+1)<<"RecHitMuonPtEM";
    title<<"At least " <<(i+1)<<" Cluster vs Pt-Endcap-";
    recHitPt_EM_[i] = dbe->book1D(name.str(), title.str(), ptBins , muPtCut_ , 100);

  }

  dcs_ = true;
}

void RPCRecHitProbability::endJob(void){
  if(saveRootFile) dbe->save(RootFileName); 
  dbe = 0;
}

void RPCRecHitProbability::endLuminosityBlock(edm::LuminosityBlock const& L, edm::EventSetup const&  E){}


void RPCRecHitProbability::analyze(const edm::Event& event,const edm::EventSetup& setup ){
  
  //Check HV status
  this->makeDcsInfo(event);
  if( !dcs_){
    edm::LogWarning ("rpcrechitprobability") <<"[RPCRecHitProbability]: DCS bit OFF" ;  
    return;//if RPC not ON there's no need to continue
  }

  counter++;
  edm::LogInfo ("rpcrechitprobability") <<"[RPCRecHitProbability]: Beginning analyzing event " << counter;
 
  //Muons
  edm::Handle<reco::CandidateView> muonCands;
  event.getByLabel(muonLabel_, muonCands);
   std::map<RPCDetId  , std::vector<RPCRecHit> > rechitMuon;

  if(muonCands.isValid()){

    int nStaMuons = muonCands->size();
    
    for( int i = 0; i < nStaMuons; i++ ) {
      
      const reco::Candidate & goodMuon = (*muonCands)[i];
      const reco::Muon * muCand = dynamic_cast<const reco::Muon*>(&goodMuon);
     
      if(!muCand->isGlobalMuon())continue;
      float eta = muCand->eta();
      float pt = muCand->pt();
      if( pt < muPtCut_  ||  fabs(eta)>muEtaCut_) continue;
      
      float phi = muCand->phi();
     
      NumberOfMuonEta_ -> Fill(eta);
      
      if(eta > 0.8){
	NumberOfMuonPt_EP_ -> Fill(pt);
	NumberOfMuonPhi_EP_ -> Fill(phi);
      }else if (eta < -0.8){
	NumberOfMuonPt_EM_ -> Fill(pt);
	NumberOfMuonPhi_EM_ -> Fill(phi);
      }else{
	NumberOfMuonPt_B_ -> Fill(pt);
	NumberOfMuonPhi_B_ -> Fill(phi);
      }

      reco::Track muTrack = (*(muCand->outerTrack()));
      std::vector<TrackingRecHitRef > rpcTrackRecHits;
     
      //loop on mu rechits

      int  recHitCounter = 0;
      for ( trackingRecHit_iterator it= muTrack.recHitsBegin(); it !=  muTrack.recHitsEnd() ; it++) {
	if (!(*it)->isValid ()) continue;
	int muSubDetId = (*it)->geographicalId().subdetId();
	if(muSubDetId == MuonSubdetId::RPC)  {
	  recHitCounter ++;
	}
      }// end loop on mu rechits

      RPCRecHitMuonEta_ -> Fill(eta, recHitCounter); 
     
      int j = 0;
      while (recHitCounter >= j+1 && j<6){

	if(recHitEta_[j]) recHitEta_[j]->Fill(eta);
	if(eta > 0.8) {
	  recHitPt_EP_[j] -> Fill(pt);
	  recHitPhi_EP_[j] -> Fill(phi);
	}else if (eta < -0.8){
	  recHitPt_EM_[j]-> Fill(pt);
	  recHitPhi_EM_[j]-> Fill(phi);
	}else{
	  recHitPt_B_[j]-> Fill(pt);
	  recHitPhi_B_[j]-> Fill(phi);
	}

	  j++;
      }

    
    }
  }else{
    edm::LogError ("rpcrechitprobability") <<"[RPCRecHitProbability]: Muons - Product not valid for event" << counter;
  }
    
}

void  RPCRecHitProbability::makeDcsInfo(const edm::Event& e) {

  edm::Handle<DcsStatusCollection> dcsStatus;

  if ( ! e.getByLabel("scalersRawToDigi", dcsStatus) ){
    dcs_ = true;
    return;
  }
  
  if ( ! dcsStatus.isValid() ) 
  {
    edm::LogWarning("RPCDcsInfo") << "scalersRawToDigi not found" ;
    dcs_ = true; // info not available: set to true
    return;
  }
    
  for (DcsStatusCollection::const_iterator dcsStatusItr = dcsStatus->begin(); 
                            dcsStatusItr != dcsStatus->end(); ++dcsStatusItr){

      if (!dcsStatusItr->ready(DcsStatus::RPC)) dcs_=false;
  }
      
  return ;
}


