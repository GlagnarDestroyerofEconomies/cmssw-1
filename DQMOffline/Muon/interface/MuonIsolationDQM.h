// -*- C++ -*-
// MuonIsolationDQM.h
// Package:    Muon Isolation DQM
// Class:      MuonIsolationDQM
// 
/*

 Description: Muon Isolation DQM class

 NOTE: The static member variable declerations *should* include the key word "static", but 
	 I haven't found an elegant way to initalize the vectors.  Static primatives (e.g. int, 
	 float, ...) and simple static objects are easy to initialze.  Outside of the class 
	 decleration, you would write
	
 		int MuonIsolationDQM::CONST_INT = 5;
 		FooType MuonIsolationDQM::CONST_FOOT = Foo(constructor_argument);
	
	 but you can't do this if you want to, say, initalize a std::vector with a bunch of 
	 different values.  So, you can't make them static and you have to initialize them using 
	 a member method.  To keep it consistent, I've just initialized them all in the same 
	 method, even the simple types.
 
*/
//
// Original Author:  "C. Jess Riedel", UC Santa Barbara
//         Created:  Tue Jul 17 15:58:24 CDT 2007
//

//Base class
#include "FWCore/Framework/interface/EDAnalyzer.h"

//Member types
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//Other include files
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"

//----------------------------------------

//Forward declarations
class TH1;
class TH1I;
class TH1D;
class TH2;
class TProfile;

//------------------------------------------
//  Class Declaration: MuonIsolationDQM
//--------------------------------------
class MuonIsolationDQM : public edm::EDAnalyzer {
  //---------namespace and typedefs--------------
  typedef edm::View<reco::Muon>::const_iterator MuonIterator;
  typedef edm::RefToBase<reco::Muon> MuonBaseRef;
  typedef edm::Handle<reco::IsoDepositMap> MuIsoDepHandle;
  typedef const reco::IsoDeposit MuIsoDepRef;
  
public:
  //---------methods----------------------------
  explicit MuonIsolationDQM(const edm::ParameterSet&);
  ~MuonIsolationDQM();
  
  
private:
  //---------methods----------------------------
  virtual void beginJob(void) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  void InitStatics();
  void RecordData(MuonIterator muon);//Fills Histograms with info from single muon
  void InitHistos();//adds title, bin information to member histograms
  void FillHistos();//Fills histograms with data
  void NormalizeHistos(); //Normalize to number of muons
  TH1* GetTH1FromMonitorElement(MonitorElement* me);
  
  //----------Static Variables---------------
  
  //Collection labels
  edm::InputTag Muon_Tag;
  edm::InputTag tkIsoDeposit_Tag;
  edm::InputTag hcalIsoDeposit_Tag;
  edm::InputTag ecalIsoDeposit_Tag;
  edm::InputTag hoIsoDeposit_Tag;
  
  //root file name
  std::string rootfilename;
  // Directories within the rootfile
  std::string dirName;
  //  std::string subDirName;

  //Histogram parameters
  static const int NUM_VARS = 24; // looking at R03 and R05.  Total of 48 histos.
  double L_BIN_WIDTH;//large bins
  double S_BIN_WIDTH;//small bins
  int LOG_BINNING_ENABLED;//pseudo log binning for profile plots
  int NUM_LOG_BINS;
  double LOG_BINNING_RATIO;
  bool requireGLBMuon;
  bool requireSTAMuon;
  bool requireTRKMuon;

  std::string title_sam;
  std::string title_cone;
  //  std::string title_cd;
  
  std::vector<std::string> main_titles;//[NUM_VARS]
  std::vector<std::string> axis_titles;//[NUM_VARS]
  std::vector<std::string> names;//[NUM_VARS]
  std::vector< std::vector<double> > param;//[NUM_VARS][3]
  std::vector<int> isContinuous;//[NUM_VARS]
  
  //---------------Dynamic Variables---------------------
  
  //MonitorElement
  DQMStore* dbe;
  
  //The Data
  int theMuonData;//[number of muons]
  double theData[NUM_VARS];
  
  //Histograms
  MonitorElement* h_nMuons;
  std::vector<MonitorElement*> h_1D;//[NUM_VARS]
  //  std::vector<MonitorElement*> cd_plots;//[NUM_VARS]
  
  //Counters
  int nEvents;
  int nSTAMuons;
  int nGLBMuons;
  int nTRKMuons;
  
  //enums for monitorElement
  enum {NOAXIS,XAXIS,YAXIS,ZAXIS};
};

