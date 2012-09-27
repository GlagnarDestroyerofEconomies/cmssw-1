#include "Validation/HcalHits/interface/HcalSimHitsClient.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

HcalSimHitsClient::HcalSimHitsClient(const edm::ParameterSet& iConfig):conf_(iConfig) {

  outputFile_ = iConfig.getUntrackedParameter<std::string>("outputFile", "myfile.root");

  dbe_ = edm::Service<DQMStore>().operator->();
  if (!dbe_) {
    edm::LogError("HcalSimHitsClient") << "unable to get DQMStore service, upshot is no client histograms will be made";
  }
  if (iConfig.getUntrackedParameter<bool>("DQMStore", false)) {
    if (dbe_) dbe_->setVerbose(0);
  }
 
  debug_ = false;
  verbose_ = true;

  dirName_= iConfig.getParameter<std::string>("DQMDirName");
  if (dbe_) dbe_->setCurrentFolder(dirName_);
 
}


HcalSimHitsClient::~HcalSimHitsClient() { }

void HcalSimHitsClient::beginJob() { }

void HcalSimHitsClient::endJob() {
  if ( outputFile_.size() != 0 && dbe_ ) dbe_->save(outputFile_);
}

void HcalSimHitsClient::beginRun(const edm::Run& run, const edm::EventSetup& c) { }


void HcalSimHitsClient::endRun(const edm::Run& , const edm::EventSetup& ) {
  runClient_();
}

//dummy analysis function
void HcalSimHitsClient::analyze(const edm::Event& , const edm::EventSetup&) { }

void HcalSimHitsClient::endLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup& ) { }

void HcalSimHitsClient::runClient_() {

  if (!dbe_) return; //we dont have the DQMStore so we cant do anything
  dbe_->setCurrentFolder(dirName_);
  
  if (verbose_) std::cout << "\nrunClient" << std::endl; 

  std::vector<MonitorElement*> hcalMEs;
  
  std::vector<std::string> fullPathHLTFolders = dbe_->getSubdirs();
  for (unsigned int i=0;i<fullPathHLTFolders.size();i++) {
    if (verbose_) std::cout <<"\nfullPath: "<< fullPathHLTFolders[i] << std::endl;
    dbe_->setCurrentFolder(fullPathHLTFolders[i]);

    std::vector<std::string> fullSubPathHLTFolders = dbe_->getSubdirs();
    for (unsigned int j=0;j<fullSubPathHLTFolders.size();j++) {

      if (verbose_) std::cout <<"fullSub: "<<fullSubPathHLTFolders[j] << std::endl;

      if (strcmp(fullSubPathHLTFolders[j].c_str(), "HcalHitsV/SimHitsValidationHcal") == 0) {
	hcalMEs = dbe_->getContents(fullSubPathHLTFolders[j]);
	if (verbose_) std::cout <<"hltMES size : "<<hcalMEs.size()<<std::endl;
	if( !SimHitsEndjob(hcalMEs) ) std::cout<<"\nError in SimhitEndjob!"<<std::endl<<std::endl;
      }

    }    

  }
  
}

// called after entering the  directory
// hcalMEs are within that directory
int HcalSimHitsClient::SimHitsEndjob(const std::vector<MonitorElement*> &hcalMEs) {
  
  MonitorElement *Occupancy_map[nTime][nType];
  std::string divisions[nType]={"HB0","HB1","HE0+z","HE1+z","HE2+z","HE0-z","HE1-z",
				"HE2-z","HO0","HFL0+z","HFS0+z","HFL1+z","HFS1+z",
				"HFL2+z","HFS2+z","HFL3+z","HFS3+z","HFL0-z","HFS0-z",
				"HFL1-z","HFS1-z","HFL2-z","HFS2-z","HFL3-z","HFS3-z"};
  
  std::string time[nTime]={"25","50","100","250"};
  char name[40];
  
  MonitorElement *E_HB=0;
  for (unsigned int ih=0; ih<hcalMEs.size(); ih++) {
    if (strcmp(hcalMEs[ih]->getName().c_str(), "Energy_HB") == 0) {
      E_HB = hcalMEs[ih];
    }
  }
  
  for (int i=0; i<nTime; i++) {
    for (int j=0; j<nType;j++) {
      for (unsigned int ih=0; ih<hcalMEs.size(); ih++) {
	sprintf (name, "HcalHitE%s%s", time[i].c_str(),divisions[j].c_str());
	if (strcmp(hcalMEs[ih]->getName().c_str(), name) == 0) {
	  Occupancy_map[i][j]= hcalMEs[ih];
	}
      }
    }
  }
  
  //mean energy 
  
  double nevent = E_HB->getEntries();
  if (verbose_) std::cout<<"nevent : "<<nevent<<std::endl;

  float cont[nTime][nType];
  float fev = float(nevent);
  
  for (int itime=0; itime<nTime; itime++) {
    for (int det=0; det<nType;det++) {
      
      int ny= Occupancy_map[itime][det]->getNbinsY();
      int nx= Occupancy_map[itime][det]->getNbinsX();
      
      for (int i=0; i<=nx; i++) {
	for (int j=0; j<=ny; j++) {
	  
	  cont[itime][det] = Occupancy_map[itime][det]->getBinContent(i,j)/fev ;
	  Occupancy_map[itime][det]->setBinContent(i,j,cont[itime][det]);
	}
      }
    }
  }
  return 1;
}

DEFINE_FWK_MODULE(HcalSimHitsClient);
