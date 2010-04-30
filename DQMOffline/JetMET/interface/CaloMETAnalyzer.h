#ifndef CaloMETAnalyzer_H
#define CaloMETAnalyzer_H

/** \class CaloMETAnalyzer
 *
 *  DQM monitoring source for CaloMET
 *
 *  $Date: 2010/03/02 02:11:49 $
 *  $Revision: 1.16 $
 *  \author F. Chlebana - Fermilab
 *          K. Hatakeyama - Rockefeller University
 */

#include <memory>
#include <fstream>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DQMOffline/JetMET/interface/CaloMETAnalyzerBase.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
//#include "DataFormats/METReco/interface/CaloMETCollection.h"
//#include "DataFormats/METReco/interface/CaloMET.h"
//#include "DataFormats/METReco/interface/CaloMETFwd.h"
//
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "RecoMET/METAlgorithms/interface/HcalNoiseRBXArray.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/METReco/interface/BeamHaloSummary.h"
#include "RecoJets/JetAlgorithms/interface/JetIDHelper.h"

#include "DataFormats/MuonReco/interface/MuonMETCorrectionData.h"
#include "DataFormats/Common/interface/ValueMap.h"  
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonMETCorrectionData.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

class CaloMETAnalyzer : public CaloMETAnalyzerBase {
 public:

  /// Constructor
  CaloMETAnalyzer(const edm::ParameterSet&);
  
  /// Destructor
  virtual ~CaloMETAnalyzer();

  /// Inizialize parameters for histo binning
  void beginJob(DQMStore * dbe);

  /// Finish up a job
  void endJob();

  /// Get the analysis
  void analyze(const edm::Event&, const edm::EventSetup&, 
               const edm::TriggerResults&);

  /// Initialize run-based parameters
  void beginRun(const edm::Run&,  const edm::EventSetup&);

  /// Finish up a run
  void endRun(const edm::Run& iRun, const edm::EventSetup& iSetup, DQMStore *dbe);

  // Book MonitorElements
  void bookMESet(std::string);
  void bookMonitorElement(std::string, bool);

  // Fill MonitorElements
  void fillMESet(const edm::Event&, std::string, const reco::CaloMET&);
  void fillMonitorElement(const edm::Event&, std::string, std::string, const reco::CaloMET&, bool);
  void makeRatePlot(std::string, double);

  void validateMET(const reco::CaloMET&, edm::Handle<edm::View<Candidate> >);
  void computeEmHaMET(edm::Handle<edm::View<Candidate> >);

  bool selectHighPtJetEvent(const edm::Event&);
  bool selectLowPtJetEvent(const edm::Event&);
  bool selectWElectronEvent(const edm::Event&);
  bool selectWMuonEvent(const edm::Event&);

  int evtCounter;

 private:
  // ----------member data ---------------------------
  
  edm::ParameterSet parameters;
  // Switch for verbosity
  int _verbose;
  int _print;

  std::string metname;
  std::string _source; // HLT? FU?
  
  edm::InputTag theCaloMETCollectionLabel;

  edm::InputTag theCaloTowersLabel;
  edm::InputTag theJetCollectionLabel;
  edm::InputTag HcalNoiseRBXCollectionTag;
  edm::InputTag HcalNoiseSummaryTag;
  edm::InputTag BeamHaloSummaryTag;
  edm::InputTag vertexTag;
  edm::InputTag gtTag;

  edm::InputTag inputMuonLabel;
  edm::InputTag inputBeamSpotLabel;

  // list of Jet or MB HLT triggers
  std::vector<std::string > HLTPathsJetMBByName_;

  std::string _hlt_HighPtJet;
  std::string _hlt_LowPtJet;
  std::string _hlt_HighMET;
  std::string _hlt_LowMET;
  std::string _hlt_Ele;
  std::string _hlt_Muon;

  edm::ParameterSet theCleaningParameters;
  std::string _hlt_PhysDec;

  std::vector<unsigned > _techTrigsAND;
  std::vector<unsigned > _techTrigsOR;
  std::vector<unsigned > _techTrigsNOT;

  bool _doPVCheck;
  bool _doHLTPhysicsOn;

  bool     _tightBHFiltering;
  int      _tightJetIDFiltering;
  bool     _tightHcalFiltering;

  int _nvtx_min;
  int _nvtxtrks_min;
  int _vtxndof_min;
  double _vtxchi2_max;
  double _vtxz_max;

  int _trig_JetMB;
  int _trig_HighPtJet;
  int _trig_LowPtJet;
  int _trig_HighMET;
  int _trig_LowMET;
  int _trig_Ele;
  int _trig_Muon;
  int _trig_PhysDec;

  double _highPtJetThreshold;
  double _lowPtJetThreshold;
  double _highMETThreshold;
  double _lowMETThreshold;

  // Et threshold for MET plots
  double _etThreshold;

  // JetID helper
  reco::helper::JetIDHelper *jetID;

  //
  bool _allhist;
  bool _allSelection;

  //
  std::vector<std::string> _FolderNames;

  //
  double _EmMEx;
  double _EmMEy;
  double _EmMET;
  double _EmCaloEz;
  double _EmSumEt;
  double _EmMetPhi;
  //
  double _HaMEx;
  double _HaMEy;
  double _HaMET;
  double _HaCaloEz;
  double _HaSumEt;
  double _HaMetPhi;

  //
  math::XYZPoint bspot;

  edm::Handle< edm::ValueMap<reco::MuonMETCorrectionData> > corMetGlobalMuons_ValueMap_Handle;
  edm::Handle< reco::MuonCollection > muon_h;
  edm::Handle< reco::BeamSpot > beamSpot_h;

  //
  DQMStore *_dbe;

  //trigger histos
  MonitorElement* hTriggerName_HighPtJet;
  MonitorElement* hTriggerName_LowPtJet;
  MonitorElement* hTriggerName_HighMET;
  MonitorElement* hTriggerName_LowMET;
  MonitorElement* hTriggerName_Ele;
  MonitorElement* hTriggerName_Muon;

  //the histos
  MonitorElement* hCaloMETRate;

  MonitorElement* hmetME;
  MonitorElement* hNevents;
  MonitorElement* hCaloMEx;
  MonitorElement* hCaloMEy;
  MonitorElement* hCaloEz;
  MonitorElement* hCaloMETSig;
  MonitorElement* hCaloMET;
  MonitorElement* hCaloMETPhi;
  MonitorElement* hCaloSumET;
  
  MonitorElement* hCaloMET_logx;
  MonitorElement* hCaloSumET_logx;
  
  MonitorElement* hCaloMETIonFeedbck;
  MonitorElement* hCaloMETHPDNoise;
  MonitorElement* hCaloMETRBXNoise;
  
  MonitorElement* hCaloMETPhi002;
  MonitorElement* hCaloMETPhi010;
  MonitorElement* hCaloMETPhi020;
  
  MonitorElement* hCaloMExLS;
  MonitorElement* hCaloMEyLS;
  
  MonitorElement* hCaloMaxEtInEmTowers;
  MonitorElement* hCaloMaxEtInHadTowers;
  MonitorElement* hCaloEtFractionHadronic;
  MonitorElement* hCaloEmEtFraction;
  
  MonitorElement* hCaloEmEtFraction002;
  MonitorElement* hCaloEmEtFraction010;
  MonitorElement* hCaloEmEtFraction020;
  
  MonitorElement* hCaloHadEtInHB;
  MonitorElement* hCaloHadEtInHO;
  MonitorElement* hCaloHadEtInHE;
  MonitorElement* hCaloHadEtInHF;
  MonitorElement* hCaloEmEtInHF;
  MonitorElement* hCaloEmEtInEE;
  MonitorElement* hCaloEmEtInEB;
  
  MonitorElement* hCaloEmMEx;
  MonitorElement* hCaloEmMEy;
  MonitorElement* hCaloEmEz;
  MonitorElement* hCaloEmMET;
  MonitorElement* hCaloEmMETPhi;
  MonitorElement* hCaloEmSumET;
  
  MonitorElement* hCaloHaMEx;
  MonitorElement* hCaloHaMEy;
  MonitorElement* hCaloHaEz;
  MonitorElement* hCaloHaMET;
  MonitorElement* hCaloHaMETPhi;
  MonitorElement* hCaloHaSumET;
  
  MonitorElement* hmuPt;
  MonitorElement* hmuEta;
  MonitorElement* hmuNhits;
  MonitorElement* hmuChi2;
  MonitorElement* hmuD0;
  MonitorElement* hMExCorrection;
  MonitorElement* hMEyCorrection;
  MonitorElement* hMuonCorrectionFlag;
  
};
#endif
