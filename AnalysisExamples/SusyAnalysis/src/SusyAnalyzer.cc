
#include "AnalysisExamples/SusyAnalysis/interface/SusyAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


using namespace std;
using namespace reco;
using namespace edm;

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
SusyAnalyzer::SusyAnalyzer(const edm::ParameterSet& iConfig)
{
 
  // flag for AOD (hopefully to be removed) 
  useAODOnly = iConfig.getParameter<bool>("useAODOnly") ;

  // flag for FastSim (hopefully to be removed) 
  useFastSimulation = iConfig.getParameter<bool>("useFastSim") ;

  // get names of modules, producing object collections
  m_electronSrc  = iConfig.getParameter<string>("electrons");
  m_muonSrc    = iConfig.getParameter<edm::InputTag>("muons");
  m_tracksSrc  = iConfig.getParameter<string>("tracks");
  m_vertexSrc  = iConfig.getParameter<string>("vertices");
  m_jetsSrc    = iConfig.getParameter<string>("jets");
  //m_jetsCorrectionService    = iConfig.getParameter<string>("JetCorrectionService");
  //m_jetsCorrectionL2RelativeService    = iConfig.getParameter<string>("JetCorrectionL2RelativeService");
  //m_jetsCorrectionL3AbsoluteService    = iConfig.getParameter<string>("JetCorrectionL3AbsoluteService");
  m_jetsCorrectionChainService    = iConfig.getParameter<string>("JetCorrectionChainService");
  m_jetsgenSrc = iConfig.getParameter<string>("jetsgen");
  m_calotowers = iConfig.getParameter<string>("calotowers");
  m_photonSrc  = iConfig.getParameter<string>("photons");
  m_calometSrc = iConfig.getParameter<string>("calomet");
  m_bjettag = iConfig.getParameter<string>("bjettag");
  m_tautag = iConfig.getParameter<string>("tautag");
  m_hlTriggerResults = iConfig.getParameter<edm::InputTag>("HLTriggerResults");
  //m_clusterShapeBarrel = iConfig.getParameter<edm::InputTag>("clusterShapeBarrel");
  //m_clusterShapeEndcap = iConfig.getParameter<edm::InputTag>("clusterShapeEndcap");
  m_reducedEndcapRecHitCollection = iConfig.getParameter<edm::InputTag>("reducedEndcapRecHitCollection");
  m_reducedBarrelRecHitCollection = iConfig.getParameter<edm::InputTag>("reducedBarrelRecHitCollection");

  // get parameters, save them in a structure Config
  
  // get MC input parameters
  InputMC_params =
    iConfig.getParameter<ParameterSet>("InputMCParams");
  myConfig.InputMC_params = InputMC_params;
  
  // get event rejection decisions
  rejectEvent_params =
    iConfig.getParameter<ParameterSet>("RejectEventParams");
  myConfig.rejectEvent_params = rejectEvent_params;
  
  // get acceptance cuts
  acceptance_cuts =
    iConfig.getParameter<ParameterSet>("AcceptanceCuts");
  myConfig.acceptance_cuts = acceptance_cuts;
    
  // get parameters for ObjectCleaner
  cleaner_params = iConfig.getParameter<ParameterSet>("CleaningParams");
  cleaner_params.addParameter<bool> ("useFastSim", useFastSimulation); 
  myConfig.cleaner_params = cleaner_params;
  
  // get parameters for Isolator  
  isolator_params = iConfig.getParameter<ParameterSet>("IsolationParams");
  myConfig.isolator_params = isolator_params;
  
  // get parameters for ObjectMatcher 
  objectmatch_params = iConfig.getParameter<ParameterSet>("ObjectMatchingParams");
  myConfig.objectmatch_params = objectmatch_params;
  
   // get parameters for MCProcessor 
  mcproc_params = iConfig.getParameter<ParameterSet>("MCProcParams");
  myConfig.mcproc_params = mcproc_params;
  
  // get parameters for UserAnalysis  
  useranalysis_params = iConfig.getParameter<ParameterSet>("UserAnalysisParams");
  myConfig.useranalysis_params = useranalysis_params;
  
  
  // get debug level
  DEBUGLVL = iConfig.getUntrackedParameter<int>("debuglvl", 0);  
  
  rej_NoTriggerData = rejectEvent_params.getParameter<bool>("rej_NoTriggerData") ;
  rej_NoL1fired = rejectEvent_params.getParameter<bool>("rej_NoL1fired") ;
  rej_NoHLTfired = rejectEvent_params.getParameter<bool>("rej_NoHLTfired") ;
  rej_NoHLTfired = rejectEvent_params.getParameter<bool>("rej_NoHLTfired") ;
  rej_BadHemis = rejectEvent_params.getParameter<bool>("rej_BadHemis") ;

}

void SusyAnalyzer::beginJob( const edm::EventSetup& )
{
  
  PrintTitle();
  PrintEvtRej();
  PrintAccCuts();
  PrintExtrapCuts();
  PrintMCCuts();
  PrintCleanerCuts();
  PrintIsolatorCuts();
  PrintObjectMatchingCuts();

  cout << endl;
  cout << "Debug Level = " << DEBUGLVL << endl;

   // initialize histogram output file
   edm::Service<TFileService> fs;
                                                                                                                                   
   // create histograms with stats
   hEventStats= fs->make<TH1I>("hEventStats","hEventStats",17,0,17); //17 bins
   hElecMuonStats= fs->make<TH1I>("hElecMuonStats","hElecMuonStats",10,0,10);//10 bins
   hPhotonJetStats= fs->make<TH1I>("hPhotonJetStats","hPhotonJetStats",10,0,10);//10 bins
   hMiscObjStats= fs->make<TH1I>("hMiscObjStats","hMiscObjStats",78,0,78); // 78 bins
   

   // instantiate user analysis class (incl. histogram booking)
   myUserAnalysis = new UserAnalysis(&myConfig);


   // initialize global counters 
   numTotL1BitsBeforeCuts.clear();
   for(int i=0; i<130; i++) {
     numTotL1BitsBeforeCuts.push_back(0);
   }

   numTotHltBitsBeforeCuts.clear();
   for(int i=0; i<130; i++) {
     numTotHltBitsBeforeCuts.push_back(0);
   }
   numTotEventsAfterCuts = 0;
   
   numTotEvt = 0;
   numTotMCReject = 0;
   numTotEvtExceptCaught = 0;
   numTotEvtNoTrigger = 0;
   numTotEvtNoReco = 0;
   numTotEvtNoTracks = 0;
   numTotEvtNoCalo = 0;
   numTotEvtEmpty = 0;
   numTotEvtNoPrimary = 0;
   numTotEvtBadHardJet = 0;
   numTotEvtCleanEmpty = 0;
   numTotEvtFinalEmpty = 0;
   numTotEvtBadNoisy = 0;
   numTotEvtBadMET = 0;
   numTotEvtBadHemi = 0;

   numTotMCElec = 0;
   numTotMCMuon = 0;
   numTotMCTau = 0;
   numTotMCPhot = 0;
   numTotMCJet = 0;
                                                                                                                                   
   numTotMCElecBigEta = 0;
   numTotMCMuonBigEta = 0;
   numTotMCTauBigEta = 0;
   numTotMCPhotBigEta = 0;
   numTotMCJetBigEta = 0;

   numTotNotPrimaryTrk = 0;
   numTotNotClean = 0;
   numTotDuplicate = 0;
   numTotElectrons = 0;
   numTotElecNotPrimaryTrk = 0;
   numTotElecNotCleanHOE = 0;
   numTotElecNotCleanShsh = 0;
   numTotElecNotCleanTmat = 0;
   numTotElecDupl = 0;
   numTotElecDuplBadHOE = 0;
   numTotElecDuplBadShsh = 0;
   numTotElecDuplBadTmat = 0;
   numTotElectronsNonIso = 0;  
   numTotElectronsNonIsoBadHOE = 0;  
   numTotElectronsNonIsoBadShsh = 0;  
   numTotElectronsNonIsoBadTmat = 0;  
   numTotElectronsfinal = 0;  
   numTotElectronsfinalBadHOE = 0;  
   numTotElectronsfinalBadShsh = 0;  
   numTotElectronsfinalBadTmat = 0;  
   numTotElectronsMatched = 0;
   numTotElectronsIsoMatched = 0;
   numTotElectronsMatchedBadHOE = 0;
   numTotElectronsMatchedBadShsh = 0;
   numTotElectronsMatchedBadTmat = 0;
   numTotMuons = 0;  
   numTotMuonNotPrimaryTrk = 0;
   numTotMuonNotClean = 0;
   numTotMuonDupl = 0;
   numTotMuonDuplBad = 0;
   numTotMuonsNonIso = 0;  
   numTotMuonsNonIsoBad = 0;  
   numTotMuonsfinal = 0;  
   numTotMuonsfinalBad = 0;  
   numTotMuonsMatched = 0;
   numTotMuonsIsoMatched = 0;
   numTotMuonsMatchedBad = 0;
   numTotTaus = 0;
   numTotTauNotPrimaryTrk = 0;
   numTotTauNotClean = 0;
   numTotTauDupl = 0;
   numTotTauDuplBad = 0;
   numTotTausNonIso = 0;
   numTotTausNonIsoBad = 0;
   numTotTausfinal = 0;
   numTotTausfinalBad = 0;
   numTotTausMatched = 0;
   numTotTausIsoMatched = 0;
   numTotTausMatchedBad = 0;
   numTotPhotons = 0;  
   numTotPhotNotPrimaryTrk = 0;
   numTotPhotNotCleanHOE = 0;
   numTotPhotNotCleanShsh = 0;
   numTotPhotDupl = 0;
   numTotPhotDuplBadHOE = 0;
   numTotPhotDuplBadShsh = 0;
   numTotPhotonsNonIso = 0;  
   numTotPhotonsNonIsoBadHOE = 0;  
   numTotPhotonsNonIsoBadShsh = 0;  
   numTotPhotonsfinal = 0;  
   numTotPhotonsfinalBadHOE = 0;  
   numTotPhotonsfinalBadShsh = 0;  
   numTotPhotonsMatched = 0;
   numTotPhotonsMatchedBadHOE = 0;
   numTotPhotonsMatchedBadShsh = 0;
   numTotJets = 0;  
   numTotJetNotPrimaryTrk = 0;
   numTotJetNotCleanFem = 0;
   numTotJetNotCleanFtk = 0;
   numTotJetDupl = 0;
   numTotJetDuplBadFem = 0;
   numTotJetDuplBadFtk = 0;
   numTotBJets = 0;  
   numTotJetsfinal = 0;  
   numTotJetsfinalBadFem = 0;  
   numTotJetsfinalBadFtk = 0;  
   numTotBJetsfinal = 0;  
   numTotJetsMatched = 0;
   numTotJetsMatchedBadFem = 0;
   numTotJetsMatchedBadFtk = 0;
   numTotUfos = 0;
   numTotUfosNotPrimaryTrk = 0;
   numTotUfosNotClean = 0;
   numTotUfosDupl = 0;
   numTotUfosDuplBad = 0;
   numTotUfosNonIso = 0;
   numTotUfosNonIsoBad = 0;
   numTotUfosfinal = 0;
   numTotUfosfinalBad = 0;
   numTotUfosMatched = 0;
   numTotUfosMatchedBad = 0;
   
   numTotEventsAfterCuts = 0;

   return ;
}

void SusyAnalyzer::endJob()
{

  // Final output of the run statistics
  
   PrintStatistics();
   cout << " in SusyAnalyzer::endJob" << endl;

   SetHistoWithStats(); // set event statistics histos (will be saved inside  the output root file)	
   
   // delete UserAnalysis, which prints its statistics
   myUserAnalysis->setNtotEvtProc(numTotEvt);
   delete myUserAnalysis; 
   


   
   
   return ;
}

SusyAnalyzer::~SusyAnalyzer()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)


}


//
// member functions
//

// ------------ method called to produce the data  ------------

void SusyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  try{
  // Variables and counters valid per event
  
//  if (iEvent.id().run() != 3415 || iEvent.id().event() != 10) {return;}
  
  numTotEvt++;
  if (DEBUGLVL >= 1){
     cout << endl;
     cout << "Event number: " << numTotEvt << endl;
  }  
  
  EventData = new MrEvent();
  EventData->setRun(iEvent.id().run());
  EventData->setEvent(iEvent.id().event());


  // ******************************************************** 
  // Input the MC data and store them as MrParticle
  // ******************************************************** 

  myInputMcData = new  InputMcData(&myConfig, iEvent );
  myInputMcData->SetDebug(DEBUGLVL);
  
  myInputMcData->InputMcDriver(EventData);
  
  // keep InputMcData alive this the end
  // (else its created MrParticles disappear)

   // handle MC and make MC printout
  
  myMCProcessor = new  MCProcessor(EventData, &myConfig);
  myMCProcessor->SetDebug(DEBUGLVL);

  bool acceptMC = true;
  acceptMC = myMCProcessor->MCDriver();
  
  if (!acceptMC){
    delete myMCProcessor;
    CleanMemory();
    if (DEBUGLVL >= 1){
      cout << " MC Event rejected *** " << endl;
    }
    numTotMCReject++;
    return;
  }
  
  int NumMCElecTrue = myMCProcessor->NumMCElecTrue();
  int NumMCMuonTrue = myMCProcessor->NumMCMuonTrue();
  int NumMCTauTrue = myMCProcessor->NumMCTauTrue();
  int NumMCPhotTrue = myMCProcessor->NumMCPhotTrue();
  int NumMCJetTrue = myMCProcessor->NumMCJetTrue();
                                                                                                                                        
  int NumMCElecBigEta = myMCProcessor->NumMCElecBigEta();
  int NumMCMuonBigEta = myMCProcessor->NumMCMuonBigEta();
  int NumMCTauBigEta = myMCProcessor->NumMCTauBigEta();
  int NumMCPhotBigEta = myMCProcessor->NumMCPhotBigEta();
  int NumMCJetBigEta = myMCProcessor->NumMCJetBigEta();

  delete myMCProcessor;


     
  // ******************************************************** 
  // Input the Trigger data and check whether the event is accepted
  // ******************************************************** 
  
  bool triggerExists = true;
  // Get the L1 Info
  Handle<L1GlobalTriggerReadoutRecord> L1GTRR;
  iEvent.getByLabel("gtDigis",L1GTRR);
  std::vector<int> l1bits;
  if (!L1GTRR.isValid()) {
    triggerExists = false;    
    EventData->setNoTriggerData();
    if (DEBUGLVL >= 1){
      cout << "L1GlobalTriggerReadoutRecord Not Valid!" << endl;
    }
  }
  else {  
   int nL1size = L1GTRR->decisionWord().size();
   if(numTotEvt==1) {numTotL1BitsBeforeCuts.resize(nL1size);}
   for (int i=0; i<nL1size; ++i) {
     l1bits.push_back(L1GTRR->decisionWord()[i]);
     if(L1GTRR->decisionWord()[i]) numTotL1BitsBeforeCuts[i]++;
   }
  }
  EventData->setL1Bits(l1bits);
  
  //Get the names of the L1 paths
  //for the moment the names are not included in L1GlobalTriggerReadoutRecord
  //we need to use L1GlobalTriggerObjectMapRecord
  if(numTotEvt==1) {
    edm::Handle<L1GlobalTriggerObjectMapRecord> gtObjectMapRecord;
    iEvent.getByLabel("hltL1GtObjectMap", gtObjectMapRecord);
    std::map<int,std::string> l1NameMap;
    const std::vector<L1GlobalTriggerObjectMap>& objMapVec =
      gtObjectMapRecord->gtObjectMap();
    for (std::vector<L1GlobalTriggerObjectMap>::const_iterator itMap = objMapVec.begin();
	 itMap != objMapVec.end(); ++itMap) {
      int algoBit = (*itMap).algoBitNumber();
      std::string algoNameStr = (*itMap).algoName();
      l1NameMap[algoBit] = algoNameStr;
    }
    l1Names.resize(L1GTRR->decisionWord().size());
    for (unsigned int i=0; i!=L1GTRR->decisionWord().size(); i++) {    
      l1Names[i]=l1NameMap[i];
    }
   EventData->setL1Names(l1Names);
 }

  // Get the HLT Info
  //vector<Handle<TriggerResults> > trhv;
  //try {iEvent.getManyByType(trhv);} catch (...) {;}
  Handle<TriggerResults> trh;  
  iEvent.getByLabel(m_hlTriggerResults, trh);
  //const unsigned int n(trhv.size());
  //if(n>1) cout << "More than one trigger table! Please check you are using the correct one" << endl;
  std::vector<int> hltbits;
  if (!trh.isValid()) {
    triggerExists = false;
    EventData->setNoTriggerData();
    if (DEBUGLVL >= 1){
      cout << "HLTriggerResult Not Valid!" << endl;
    }
  }
  else {  
   if(numTotEvt==1) {numTotHltBitsBeforeCuts.resize(trh->size());}
   for(unsigned int i=0; i< trh->size(); i++) {
     hltbits.push_back((*trh)[i].accept());
     if((*trh)[i].accept()) numTotHltBitsBeforeCuts[i]++;
   }
   if(numTotEvt==1) {
     edm::TriggerNames triggerNames_;  // TriggerNames class
     triggerNames_.init(*trh);
     hlNames=triggerNames_.triggerNames();
     EventData->setHltNames(hlNames);
   }
  }
  EventData->setHltBits(hltbits);

  // Check whether event is accepted by L1 and HLT triggers
  
  bool acctrigger = true;
  
  bool accL1  = myUserAnalysis->L1Driver(EventData);
  bool accHLT = myUserAnalysis->HLTDriver(EventData);
  acctrigger = (triggerExists && accL1 && accHLT);
 
  if (!acctrigger){
    numTotEvtNoTrigger++;
    if (!accL1){EventData->setNoL1fired();}
    if (!accHLT){EventData->setNoHLTfired();}
    if (DEBUGLVL >= 1){
     cout << " No trigger accept for this event " << endl;
    }
  }
//  if ( EventData->qualNoTriggerData() ){ cout << " No trigger data " << endl;}
//  if ( EventData->qualNoL1fired() ){ cout << " No L1 fired " << endl;}
//  if ( EventData->qualNoHLTfired() ){ cout << " No HLT fired " << endl;}
  // keep or reject event according to decision in cfg file
  if ( (rej_NoTriggerData && !triggerExists) 
     || (rej_NoL1fired && !accL1)
     || (rej_NoHLTfired && !accHLT) ){
    if (DEBUGLVL >= 1){
      cout << " No trigger, Event rejected " << endl;
    }
    CleanMemory ();
    return;
  }
  

//  cout << "Size of Recodata at beginning of filling " << RecoData.size() << endl;
     
  // ******************************************************** 
  // Input the Reconstructed data and store them as MrParticle objects
  // ******************************************************** 

  // get electron collection 
  Handle<GsfElectronCollection> electrons;  
  iEvent.getByLabel(m_electronSrc, electrons);

  for (unsigned int j = 0; j < electrons->size(); j++)
  {


 

   // set as energy the energy of the supercluster; and as momentum
   // the momentum of the track at closest approach to vertex, renormalized 
   // to the energy of the supercluster
//   float elenergy =((*electrons)[j].superCluster())->energy();
//   float elpx = (*electrons)[j].px() * elenergy / (*electrons)[j].p();
//   float elpy = (*electrons)[j].py() * elenergy / (*electrons)[j].p();
//   float elpz = (*electrons)[j].pz() * elenergy / (*electrons)[j].p();
//   MrElectron* recopart = new MrElectron(elpx, elpy, elpz, elenergy, &(*electrons)[j]);

  
// better : use weighted supercluster/gsftrack energy + egamma energy scale correction 
  
   MrElectron* recopart = new MrElectron((*electrons)[j].px(),(*electrons)[j].py(), (*electrons)[j].pz(), (*electrons)[j].energy(), &(*electrons)[j]);
 
   RecoData.push_back(recopart);

  
  }

  //get electron cluster shape Collection -> association map (electron,clustershape)  

  //barrel
  //edm::Handle<reco::BasicClusterShapeAssociationCollection> clusterShapeBarrel;
  //iEvent.getByLabel(m_clusterShapeBarrel,clusterShapeBarrel);
  //clusterShapeBarrelData = clusterShapeBarrel.product();
  
  //endcap
  //edm::Handle<reco::BasicClusterShapeAssociationCollection> clusterShapeEndcap;
  //iEvent.getByLabel(m_clusterShapeEndcap,clusterShapeEndcap);
  //clusterShapeEndcapData = clusterShapeEndcap.product();


  // get muons collection
  Handle<MuonCollection> muons; 
  iEvent.getByLabel(m_muonSrc, muons);

  for (unsigned int j = 0; j < muons->size(); j++)
  {
   if((*muons)[j].isGlobalMuon()) {
     MrMuon* recopart = new MrMuon((*muons)[j].px(),
            (*muons)[j].py(),(*muons)[j].pz(),(*muons)[j].energy(), &(*muons)[j] );
  
    RecoData.push_back(recopart);
   }
  }


   // get photons collection
   
   Handle<PhotonCollection> photons; 
   iEvent.getByLabel(m_photonSrc,"", photons);
   
   for (unsigned int j = 0; j < photons->size(); j++)
   {
   MrPhoton* recopart = new MrPhoton((*photons)[j].px(),
           (*photons)[j].py(),(*photons)[j].pz(),(*photons)[j].energy(), &(*photons)[j] );
  
   RecoData.push_back(recopart);
   
   }

   // get Taus collection
   
  // before CMSSW_1_6_0 
  // Handle<IsolatedTauTagInfoCollection> tauTagInfoHandle;
  // iEvent.getByLabel(m_tautag, tauTagInfoHandle);
  // const IsolatedTauTagInfoCollection * tauTagInfo = tauTagInfoHandle.product();
  
  // from CMSSW_1_6_0 till CMSSW_2_0_0 
  // Handle<JetTagCollection> tauTagInfoHandle;
  // iEvent.getByLabel(m_tautag, tauTagInfoHandle);
  // const JetTagCollection * tauTagInfo = tauTagInfoHandle.product();



   // get calo jet collection
   Handle<CaloJetCollection> jets;
   iEvent.getByLabel(m_jetsSrc, jets);


   // collect information for b-tagging
   Handle<JetTagCollection> jetsAndTracks;
   iEvent.getByLabel(m_bjettag,jetsAndTracks);

   const JetTag* jetTag = NULL;
    
   double EPSILON_BT = 0.00001;
   double btagdiscriminator = -100;
   double tautagdiscriminator = -100;

  for (unsigned int j = 0; j < jets->size(); j++)
  {
  /*   for (unsigned int i = 0; i < tauTagInfo->size(); i++) {

 
     if (
    //  &(*tauTagInfo)[i].jet() == &(*jets)[j] 
          fabs( ((*jets)[j].px() - (*tauTagInfo)[i].jet()->px())/  (*jets)[j].px()) < EPSILON_BT && 
	   fabs( ((*jets)[j].py() - (*tauTagInfo)[i].jet()->py())/  (*jets)[j].py()) < EPSILON_BT &&
	   fabs( ((*jets)[j].pz() - (*tauTagInfo)[i].jet()->pz())/  (*jets)[j].pz()) < EPSILON_BT &&
	   fabs( ((*jets)[j].energy() - (*tauTagInfo)[i].jet()->energy())/  (*jets)[j].energy()) < EPSILON_BT
     
     ) {
       tautagdiscriminator = (*tauTagInfo)[i].discriminator();
 
       break;
     }
    }
  */
 
    for (unsigned int i = 0; i < jetsAndTracks->size(); i++){
      if (

      //  &(*jetsAndTracks)[i].first == &(*jets)[j] )
          fabs( ((*jets)[j].px() - (*jetsAndTracks)[i].first->px())/  (*jets)[j].px()) < EPSILON_BT && 
	   fabs( ((*jets)[j].py() - (*jetsAndTracks)[i].first->py())/  (*jets)[j].py()) < EPSILON_BT &&
	   fabs( ((*jets)[j].pz() - (*jetsAndTracks)[i].first->pz())/  (*jets)[j].pz()) < EPSILON_BT &&
	   fabs( ((*jets)[j].energy() - (*jetsAndTracks)[i].first->energy())/  (*jets)[j].energy()) < EPSILON_BT )  {
   
	  btagdiscriminator =  (*jetsAndTracks)[i].second;
	  jetTag = &(*jetsAndTracks)[i];
	  break;
      
	}
    }
    MrJet* recopart = new MrJet((*jets)[j].px(),
           (*jets)[j].py(),(*jets)[j].pz(),(*jets)[j].energy(), &(*jets)[j],jetTag);

  
  
    recopart->setBtagDiscriminator(btagdiscriminator);
    recopart->setTauTagDiscriminator(tautagdiscriminator);
   
    RecoData.push_back(recopart);
 
  }



   // get track collection
   Handle<TrackCollection> tracks;
   iEvent.getByLabel(m_tracksSrc, tracks);
   TrackData = tracks.product();
   if (DEBUGLVL >= 3){
     cout << "Track collection: " << endl;
     for (int i=0; i< (int) TrackData->size(); i++){
       const Track* pTrack = &(*TrackData)[i];
       if (pTrack->pt() > 0.9) {
         cout << " Track index = " << i << ", p = " << pTrack->p()
              << ", pT = " << pTrack->pt() 
              << ", eta " << pTrack->eta() << ", phi " << pTrack->phi()
              << ", Charge " << pTrack->charge() << endl;
       }
     }
   }

   // get vertex collection
   Handle<VertexCollection> vertices;
   iEvent.getByLabel(m_vertexSrc, vertices);
   VertexData = vertices.product();
   if (DEBUGLVL >= 3){
     cout << "Vertex collection: " << endl;
     for (int i=0; i< (int) VertexData->size(); i++){
       const Vertex* pVertex = &(*VertexData)[i];
       cout << " Vertex index = " << i << ", x = " << pVertex->x()
            << ", y = " << pVertex->y()<< ", z = " << pVertex->z() << endl;
     }
   }


   // get calo towers collection
   Handle<CaloTowerCollection> calotowers; 
   iEvent.getByLabel(m_calotowers,calotowers);
   CaloTowerData = calotowers.product();
   if (DEBUGLVL >= 3){
     cout << "CaloTower collection: " << endl;
     for (int i=0; i< (int) CaloTowerData->size(); i++){
       const CaloTower* pCaloTower = &(*CaloTowerData)[i];
       if (pCaloTower->et() > 5.) {
         cout << " CaloTower index = " << i << ", ET = " << pCaloTower->et()
              << ", ET em = " << pCaloTower->emEnergy()
              << ", ET had = " << pCaloTower->hadEnergy() << endl;
       }
     }
   }
   
     
   // get jet corrections
   //const JetCorrector* jetCorrectorL2 = JetCorrector::getJetCorrector (m_jetsCorrectionL2RelativeService, iSetup);
   //const JetCorrector* jetCorrectorL3 = JetCorrector::getJetCorrector (m_jetsCorrectionL3AbsoluteService, iSetup);
   vector<const JetCorrector*> jetCorrectors;
   //jetCorrectors.push_back(jetCorrectorL2);
   //jetCorrectors.push_back(jetCorrectorL3);
   const JetCorrector* jetCorrectorChain = JetCorrector::getJetCorrector (m_jetsCorrectionChainService, iSetup);
   jetCorrectors.push_back(jetCorrectorChain);


   // Save the pointers to vertex, track and calotower collections in MrEvent
   
//   const TrackCollection * tc;
//   EventData->setTrackCollection(tc); 
   EventData->setTrackCollection(TrackData); 
   EventData->setVertexCollection(VertexData); 
   EventData->setCaloTowerCollection(CaloTowerData); 
   //EventData->setClusterShapeBarrelCollection(clusterShapeBarrelData);
   //EventData->setClusterShapeEndcapCollection(clusterShapeEndcapData);
   EventData->setJetCorrectors(&jetCorrectors);
  
   //for cluster shape in 2_2_X 
//   ESHandle<CaloGeometry> caloGeometry;
//   iSetup.get<CaloGeometryRecord>().get(caloGeometry);   
   EcalClusterLazyTools lazyTool( iEvent, iSetup, m_reducedBarrelRecHitCollection, m_reducedEndcapRecHitCollection);
   EventData->setLazyTools(&lazyTool);


   // make printout of candidates, etc.
   if (DEBUGLVL >= 1){
     cout << endl;
     cout << "Reco Candidates information: " << endl;
     cout << " number of electrons  "<< electrons->size() << endl;
     cout << " number of muons      "<< muons->size() << endl;
     //cout << " number of tau jets   " << tauTagInfo.size() << endl;
     cout << " number of photons    "<< photons->size() << endl;
     cout << " number of jets       "<< jets->size() << endl;
     cout << " number of tracks     "<< tracks->size() << endl;
     cout << " number of vertices   "<< vertices->size() << endl;
     cout << " number of calotowers "<< calotowers->size() << endl;
     //cout << " number of jetsgen    "<< jetsgen->size() << endl;
   }


  // ******************************************************** 
 
  // Save the RecoData in the Event data
 
   EventData->setRecoData(&RecoData);

  // ******************************************************** 

  // Save the CaloMet in de Event data
  
   Handle<CaloMETCollection> calomethandle;
   iEvent.getByLabel(m_calometSrc, calomethandle);
   const CaloMETCollection *calometcol = calomethandle.product();
   const CaloMET calomet = calometcol->front();
   math::XYZVector calometvector = math::XYZVector(calomet.px(),calomet.py(),0.);
   EventData->setMetCalo(calometvector);

  // ******************************************************** 
 
  // Handle the Reco event data and check their quality
   myRecoProcessor = new  RecoProcessor(EventData, &myConfig);
   myRecoProcessor->SetDebug(DEBUGLVL);

   bool acceptData = true;
   acceptData = myRecoProcessor->RecoDriver();
  
   // Update Event counters for final statistics
   numTotEvtNoReco += myRecoProcessor->NumEvtNoReco();
   numTotEvtNoTracks += myRecoProcessor->NumEvtNoTracks();
   numTotEvtNoCalo += myRecoProcessor->NumEvtNoCalo();
   numTotEvtEmpty += myRecoProcessor->NumEvtEmpty();
   numTotEvtNoPrimary += myRecoProcessor->NumEvtNoPrimary();
   numTotEvtBadHardJet += myRecoProcessor->NumEvtBadHardJet();
   numTotEvtCleanEmpty += myRecoProcessor->NumEvtCleanEmpty();
   numTotEvtFinalEmpty += myRecoProcessor->NumEvtFinalEmpty();
   numTotEvtBadNoisy += myRecoProcessor->NumEvtBadNoisy();
   numTotEvtBadMET += myRecoProcessor->NumEvtBadMET();
      
   // Abandon the event if it is bad
   if (!acceptData){
      delete myRecoProcessor;
      CleanMemory();
      if (DEBUGLVL >= 1){
       cout << " Event rejected *** " << endl;
      }
      return;
   }

  // end of event cleaning
  // keep the myRecoProcessor object, as it is still used below
   

  // ******************************************************** 
  // Compute efficiencies and fake rates 
  // Filip: to be removed for present release .....
   
   myEffProcessor = new EffProcessor(&myConfig);
   myEffProcessor->SetDebug(DEBUGLVL);
   
   myEffProcessor->EffDriver(EventData);
   

  // ******************************************************** 
  // Recontruct and match the hemispheres 
    
   myHemiAna = new ShapeAnalyzer(EventData);
   myHemiAna->SetDebug(DEBUGLVL);
   
   bool acceptHemi = true;
   acceptHemi = myHemiAna->AnalyzeHemi();
  
   if (!acceptHemi){
      numTotEvtBadHemi++;
      EventData->setBadHemis();
      if (DEBUGLVL >= 1){
       cout << " Bad hemispheres matching *** " << endl;
      }
      if (rej_BadHemis) {
        delete myHemiAna;
        CleanMemory();
        return;
      }
   }
   
   delete myHemiAna;

  // end of hemisphere handling
  
  
  // ******************************************************** 
  // Update run counters if event is accepted for end of job statistics
  
  numTotMCElec += NumMCElecTrue;
  numTotMCMuon += NumMCMuonTrue;
  numTotMCTau += NumMCTauTrue;
  numTotMCPhot += NumMCPhotTrue;
  numTotMCJet += NumMCJetTrue;
                                                                                                                                        
  numTotMCElecBigEta += NumMCElecBigEta;
  numTotMCMuonBigEta += NumMCMuonBigEta;
  numTotMCTauBigEta += NumMCTauBigEta;
  numTotMCPhotBigEta += NumMCPhotBigEta;
  numTotMCJetBigEta += NumMCJetBigEta;

  numTotNotPrimaryTrk += myRecoProcessor->NumNotPrimaryTrk();
  numTotNotClean += myRecoProcessor->NumNotClean();
  numTotDuplicate += myRecoProcessor->NumDuplicate();
  numTotElectrons += myRecoProcessor->NumElectrons();
  numTotElecNotPrimaryTrk += myRecoProcessor->NumElecNotPrimaryTrk();
  numTotElecNotCleanHOE += myRecoProcessor->NumElecNotCleanHOE();
  numTotElecNotCleanShsh += myRecoProcessor->NumElecNotCleanShsh();
  numTotElecNotCleanTmat += myRecoProcessor->NumElecNotCleanTmat();
  numTotElecDupl += myRecoProcessor->NumElecDupl();
  numTotElecDuplBadHOE += myRecoProcessor->NumElecDuplBadHOE();
  numTotElecDuplBadShsh += myRecoProcessor->NumElecDuplBadShsh();
  numTotElecDuplBadTmat += myRecoProcessor->NumElecDuplBadTmat();
  numTotElectronsNonIso += myRecoProcessor->NumElectronsNonIso();  
  numTotElectronsNonIsoBadHOE += myRecoProcessor->NumElectronsNonIsoBadHOE();  
  numTotElectronsNonIsoBadShsh += myRecoProcessor->NumElectronsNonIsoBadShsh();  
  numTotElectronsNonIsoBadTmat += myRecoProcessor->NumElectronsNonIsoBadTmat();  
  numTotElectronsfinal += myRecoProcessor->NumElectronsfinal();  
  numTotElectronsfinalBadHOE += myRecoProcessor->NumElectronsfinalBadHOE();  
  numTotElectronsfinalBadShsh += myRecoProcessor->NumElectronsfinalBadShsh();  
  numTotElectronsfinalBadTmat += myRecoProcessor->NumElectronsfinalBadTmat();  
  numTotElectronsMatched += myRecoProcessor->NumElectronsMatched();
  numTotElectronsIsoMatched += myRecoProcessor->NumElectronsIsoMatched();
  numTotElectronsMatchedBadHOE += myRecoProcessor->NumElectronsMatchedBadHOE();
  numTotElectronsMatchedBadShsh += myRecoProcessor->NumElectronsMatchedBadShsh();
  numTotElectronsMatchedBadTmat += myRecoProcessor->NumElectronsMatchedBadTmat();
  numTotMuons += myRecoProcessor->NumMuons();  
  numTotMuonNotPrimaryTrk += myRecoProcessor->NumMuonNotPrimaryTrk();
  numTotMuonNotClean += myRecoProcessor->NumMuonNotClean();
  numTotMuonDupl += myRecoProcessor->NumMuonDupl();
  numTotMuonDuplBad += myRecoProcessor->NumMuonDuplBad();
  numTotMuonsNonIso += myRecoProcessor->NumMuonsNonIso();  
  numTotMuonsNonIsoBad += myRecoProcessor->NumMuonsNonIsoBad();  
  numTotMuonsfinal += myRecoProcessor->NumMuonsfinal();  
  numTotMuonsfinalBad += myRecoProcessor->NumMuonsfinalBad();  
  numTotMuonsMatched += myRecoProcessor->NumMuonsMatched();
  numTotMuonsIsoMatched += myRecoProcessor->NumMuonsIsoMatched();
  numTotMuonsMatchedBad += myRecoProcessor->NumMuonsMatchedBad();
  numTotTaus += myRecoProcessor->NumTaus();  
  numTotTauNotPrimaryTrk += myRecoProcessor->NumTauNotPrimaryTrk();
  numTotTauNotClean += myRecoProcessor->NumTauNotClean();
  numTotTauDupl += myRecoProcessor->NumTauDupl();
  numTotTauDuplBad += myRecoProcessor->NumTauDuplBad();
  numTotTausNonIso += myRecoProcessor->NumTausNonIso();  
  numTotTausNonIsoBad += myRecoProcessor->NumTausNonIsoBad();  
  numTotTausfinal += myRecoProcessor->NumTausfinal();  
  numTotTausfinalBad += myRecoProcessor->NumTausfinalBad();  
  numTotTausMatched += myRecoProcessor->NumTausMatched();
  numTotTausIsoMatched += myRecoProcessor->NumTausIsoMatched();
  numTotTausMatchedBad += myRecoProcessor->NumTausMatchedBad();
  numTotPhotons += myRecoProcessor->NumPhotons();  
  numTotPhotNotPrimaryTrk += myRecoProcessor->NumPhotNotPrimaryTrk();
  numTotPhotNotCleanHOE += myRecoProcessor->NumPhotNotCleanHOE();
  numTotPhotNotCleanShsh += myRecoProcessor->NumPhotNotCleanShsh();
  numTotPhotDupl += myRecoProcessor->NumPhotDupl();
  numTotPhotDuplBadHOE += myRecoProcessor->NumPhotDuplBadHOE();
  numTotPhotDuplBadShsh += myRecoProcessor->NumPhotDuplBadShsh();
  numTotPhotonsNonIso += myRecoProcessor->NumPhotonsNonIso();  
  numTotPhotonsNonIsoBadHOE += myRecoProcessor->NumPhotonsNonIsoBadHOE();  
  numTotPhotonsNonIsoBadShsh += myRecoProcessor->NumPhotonsNonIsoBadShsh();  
  numTotPhotonsfinal += myRecoProcessor->NumPhotonsfinal();  
  numTotPhotonsfinalBadHOE += myRecoProcessor->NumPhotonsfinalBadHOE();  
  numTotPhotonsfinalBadShsh += myRecoProcessor->NumPhotonsfinalBadShsh();  
  numTotPhotonsMatched += myRecoProcessor->NumPhotonsMatched();
  numTotPhotonsMatchedBadHOE += myRecoProcessor->NumPhotonsMatchedBadHOE();
  numTotPhotonsMatchedBadShsh += myRecoProcessor->NumPhotonsMatchedBadShsh();
  numTotJets += myRecoProcessor->NumJets();  
  numTotJetNotPrimaryTrk += myRecoProcessor->NumJetNotPrimaryTrk();
  numTotJetNotCleanFem += myRecoProcessor->NumJetNotCleanFem();
  numTotJetNotCleanFtk += myRecoProcessor->NumJetNotCleanFtk();
  numTotJetDupl += myRecoProcessor->NumJetDupl();
  numTotJetDuplBadFem += myRecoProcessor->NumJetDuplBadFem();
  numTotJetDuplBadFtk += myRecoProcessor->NumJetDuplBadFtk();
  numTotBJets += myRecoProcessor->NumBJets();  
  numTotJetsfinal += myRecoProcessor->NumJetsfinal();  
  numTotJetsfinalBadFem += myRecoProcessor->NumJetsfinalBadFem();  
  numTotJetsfinalBadFtk += myRecoProcessor->NumJetsfinalBadFtk();  
  numTotBJetsfinal += myRecoProcessor->NumBJetsfinal();  
  numTotJetsMatched += myRecoProcessor->NumJetsMatched();
  numTotJetsMatchedBadFem += myRecoProcessor->NumJetsMatchedBadFem();
  numTotJetsMatchedBadFtk += myRecoProcessor->NumJetsMatchedBadFtk();
  numTotUfos += myRecoProcessor->NumUfos();
  numTotUfosNotPrimaryTrk += myRecoProcessor->NumUfosNotPrimaryTrk();
  numTotUfosNotClean += myRecoProcessor->NumUfosNotClean();
  numTotUfosDupl += myRecoProcessor->NumUfosDupl();
  numTotUfosDuplBad += myRecoProcessor->NumUfosDuplBad();
  numTotUfosNonIso += myRecoProcessor->NumUfosNonIso();
  numTotUfosNonIsoBad += myRecoProcessor->NumUfosNonIsoBad();
  numTotUfosfinal += myRecoProcessor->NumUfosfinal();
  numTotUfosfinalBad += myRecoProcessor->NumUfosfinalBad();
  numTotUfosMatched += myRecoProcessor->NumUfosMatched();
  numTotUfosMatchedBad += myRecoProcessor->NumUfosMatchedBad();
  
  numTotEventsAfterCuts++;
  
  delete myRecoProcessor;
   

  
  // ******************************************************** 
  // Now start the analysis
  // ******************************************************** 
  
  if (DEBUGLVL >= 1){
    cout << "Calling the User Analysis " << endl;
  }
  myUserAnalysis->doAnalysis(EventData);
  
  
  // ******************************************************** 
  // End of the event analysis
  // ******************************************************** 

  delete myInputMcData;


 
 
  // clean memory
  CleanMemory();

  } catch (std::exception & err) {   
   
    numTotEvtExceptCaught++;
    cout << "Exception during event number: " << iEvent.id() 
         << "\n" << err.what() << "\n";
    CleanMemory();
//    exit (1);
    return;
  }
  
  return;
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintTitle()
{ 
 // Prints a Title for SusyAnalysis

  cout << endl;
  cout << "*****************************" << endl;
  cout << "*** Start of SUSYBSM Analysis" << endl;
  cout << "*****************************" << endl;
  
  return;
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintEvtRej()
{ 
 // Prints a summary of event rejection decisions

 bool rej_NoTriggerData = rejectEvent_params.getParameter<bool>("rej_NoTriggerData") ;
 bool rej_NoL1fired = rejectEvent_params.getParameter<bool>("rej_NoL1fired") ;
 bool rej_NoHLTfired = rejectEvent_params.getParameter<bool>("rej_NoHLTfired") ;
 bool rej_MissingRecoData = rejectEvent_params.getParameter<bool>("rej_MissingRecoData") ;
 bool rej_MissingTrackData = rejectEvent_params.getParameter<bool>("rej_MissingTrackData") ;
 bool rej_MissingCaloTowers = rejectEvent_params.getParameter<bool>("rej_MissingCaloTowers") ;
 bool rej_Empty = rejectEvent_params.getParameter<bool>("rej_Empty") ;
 bool rej_NoPrimary = rejectEvent_params.getParameter<bool>("rej_NoPrimary") ;
 bool rej_BadHardJet = rejectEvent_params.getParameter<bool>("rej_BadHardJet") ;
 bool rej_CleanEmpty = rejectEvent_params.getParameter<bool>("rej_CleanEmpty") ;
 bool rej_FinalEmpty = rejectEvent_params.getParameter<bool>("rej_FinalEmpty") ;
 bool rej_BadNoisy = rejectEvent_params.getParameter<bool>("rej_BadNoisy") ;
 bool rej_BadMET = rejectEvent_params.getParameter<bool>("rej_BadMET") ;
 bool rej_BadHemis = rejectEvent_params.getParameter<bool>("rej_BadHemis") ;
  
  cout << endl;
  cout << "Event rejection decisions: " << endl;
  cout << "  Reject if no trigger data       = ";
  if (rej_NoTriggerData){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if L1 did not fire       = ";
  if (rej_NoL1fired){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if HLT did not fire      = ";
  if (rej_NoHLTfired){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if no Reco Data exist    = ";
  if (rej_MissingRecoData){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if no Track Data exist   = ";
  if (rej_MissingTrackData){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if no CaloTowers exist   = ";
  if (rej_MissingCaloTowers){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if empty after 1st acc   = ";
  if (rej_Empty){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if no Primary Vertex     = ";
  if (rej_NoPrimary){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if bad hard Jet found    = ";
  if (rej_BadHardJet){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if empty after cleaning  = ";
  if (rej_CleanEmpty){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if empty after final acc = ";
  if (rej_FinalEmpty){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if event bad or noisy    = ";
  if (rej_BadNoisy){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if event with bad MET    = ";
  if (rej_BadMET){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if event with bad hemi   = ";
  if (rej_BadHemis){cout << " true";} else {cout << " false";} cout << endl;
  
  return;
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintAccCuts()
{ 
 // Prints a summary of the acceptance cuts
 
  // load acceptance cuts
  float ana_elecEtaMax = acceptance_cuts.getParameter<double>("ana_elecEtaMax") ;
  float ana_elecPtMin1 = acceptance_cuts.getParameter<double>("ana_elecPtMin1") ;
  float ana_muonEtaMax = acceptance_cuts.getParameter<double>("ana_muonEtaMax") ;
  float ana_muonPtMin1 = acceptance_cuts.getParameter<double>("ana_muonPtMin1") ;
  float ana_tauEtaMax = acceptance_cuts.getParameter<double>("ana_tauEtaMax") ;
  float ana_tauPtMin1 = acceptance_cuts.getParameter<double>("ana_tauPtMin1") ;
  float ana_photonEtaMax = acceptance_cuts.getParameter<double>("ana_photonEtaMax") ;
  float ana_photonPtMin1 = acceptance_cuts.getParameter<double>("ana_photonPtMin1") ;
  float ana_jetEtaMax = acceptance_cuts.getParameter<double>("ana_jetEtaMax") ;
  float ana_jetPtMin1 = acceptance_cuts.getParameter<double>("ana_jetPtMin1") ;
  int ana_ufoSelMethod = acceptance_cuts.getParameter<int>("ana_ufoSelMethod") ;
  int ana_ufoTkHitsmin = acceptance_cuts.getParameter<int>("ana_ufoTkHitsmin") ;
  float ana_ufoCaloTowEFracmin = acceptance_cuts.getParameter<double>("ana_ufoCaloTowEFracmin") ;
  float ana_ufodRTrkFromJet = acceptance_cuts.getParameter<double>("ana_ufodRTrkFromJet") ;
  float ana_ufoEtaMax = acceptance_cuts.getParameter<double>("ana_ufoEtaMax") ;
  float ana_ufoPtMin1 = acceptance_cuts.getParameter<double>("ana_ufoPtMin1") ;
  float ana_ufoDRmin = acceptance_cuts.getParameter<double>("ana_ufoDRmin") ;
  bool ana_useCorrectedEnergy = acceptance_cuts.getParameter<bool>("ana_useCorrectedEnergy") ;
  float ana_elecPtMin2 = acceptance_cuts.getParameter<double>("ana_elecPtMin2") ;
  float ana_muonPtMin2 = acceptance_cuts.getParameter<double>("ana_muonPtMin2") ;
  float ana_tauPtMin2 = acceptance_cuts.getParameter<double>("ana_tauPtMin2") ;
  float ana_photonPtMin2 = acceptance_cuts.getParameter<double>("ana_photonPtMin2") ;
  float ana_jetPtMin2 = acceptance_cuts.getParameter<double>("ana_jetPtMin2") ;
  float ana_minBtagDiscriminator = acceptance_cuts.getParameter<double>("ana_minBtagDiscriminator");
  float ana_ufoPtMin2 = acceptance_cuts.getParameter<double>("ana_ufoPtMin2") ;

  cout << endl;
  cout << "Primary acceptance cuts for objects: " << endl;
  cout << "  Maximum Eta for electrons       = " << ana_elecEtaMax << endl;
  cout << "  Minimum Pt for electrons, first = " << ana_elecPtMin1 << endl;
  cout << "  Maximum Eta for muons           = " << ana_muonEtaMax << endl;
  cout << "  Minimum Pt for muons, first     = " << ana_muonPtMin1 << endl;
  cout << "  Maximum Eta for taus            = " << ana_tauEtaMax << endl;
  cout << "  Minimum Pt for taus, first      = " << ana_tauPtMin1 << endl;
  cout << "  Maximum Eta for photons         = " << ana_photonEtaMax << endl;
  cout << "  Minimum Pt for photons, first   = " << ana_photonPtMin1 << endl;
  cout << "  Maximum Eta for jets            = " << ana_jetEtaMax << endl;
  cout << "  Minimum Pt for jets, first      = " << ana_jetPtMin1 << endl;
  cout << "  Minimum b-tag discriminator     = " << ana_minBtagDiscriminator << endl;
  cout << "  Method for UFO selection        = " << ana_ufoSelMethod << endl;
  if (useAODOnly && ana_ufoSelMethod == 1){
    cout << " ==> cannot be = 1 if AOD only, please change to 2 or 3 *****" << endl;
    cout << "  the same applies to clean_methodTksInJetVx and clean_methodTksInJet *****" << endl;
    exit(1);
  }  
  cout << "  Minimum hits on a track         = " << ana_ufoTkHitsmin << endl;
  cout << "  Minimum CaloTow E Fraction      = " << ana_ufoCaloTowEFracmin << endl;
  cout << "  DeltaR for Track search of Jet  = " << ana_ufodRTrkFromJet << endl;
  cout << "  Maximum Eta for UFOs            = " << ana_ufoEtaMax << endl;
  cout << "  Minimum Pt for UFOs, first      = " << ana_ufoPtMin1 << endl;
  cout << "  Minimum DeltaR for UFOs in jets = " << ana_ufoDRmin << endl;
  cout << endl;
  cout << "Final acceptance cuts for objects: " << endl;
  cout << "  Using corrected energy          = ";
  if (ana_useCorrectedEnergy){cout << "true";}
  else {cout << "false";}
  cout << endl;
  cout << "  Minimum Pt for electrons, final = " << ana_elecPtMin2 << endl;
  cout << "  Minimum Pt for muons, final     = " << ana_muonPtMin2 << endl;
  cout << "  Minimum Pt for taus, final      = " << ana_tauPtMin2 << endl;
  cout << "  Minimum Pt for photons, final   = " << ana_photonPtMin2 << endl;
  cout << "  Minimum Pt for jets, final      = " << ana_jetPtMin2 << endl;
  cout << "  Minimum Pt for UFOs, final      = " << ana_ufoPtMin2 << endl;


    return;    
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintExtrapCuts()
{ 
 // Prints a summary of the parameters for extrapolation error adjustment (in cm)
 
  // load extrapolation error adjustment cuts
 float reco_elecD0ErrorThresh  = acceptance_cuts.getParameter<double>("reco_elecD0ErrorThresh") ;
 float reco_elecDzErrorThresh  = acceptance_cuts.getParameter<double>("reco_elecDzErrorThresh") ;
 float reco_muonD0ErrorThresh  = acceptance_cuts.getParameter<double>("reco_muonD0ErrorThresh") ;
 float reco_muonDzErrorThresh  = acceptance_cuts.getParameter<double>("reco_muonDzErrorThresh") ;
 float reco_jetD0ErrorThresh  = acceptance_cuts.getParameter<double>("reco_jetD0ErrorThresh") ;
 float reco_jetDzErrorThresh  = acceptance_cuts.getParameter<double>("reco_jetDzErrorThresh") ;
 float reco_ufoD0ErrorThresh  = acceptance_cuts.getParameter<double>("reco_ufoD0ErrorThresh") ;
 float reco_ufoDzErrorThresh  = acceptance_cuts.getParameter<double>("reco_ufoDzErrorThresh") ;

  cout << endl;
  cout << "Parameters for extrapolation error adjustment (in cm): " << endl;
  cout << "  reco_elecD0ErrorThresh          = " << reco_elecD0ErrorThresh << endl;
  cout << "  reco_elecDzErrorThresh          = " << reco_elecDzErrorThresh << endl;
  cout << "  reco_muonD0ErrorThresh          = " << reco_muonD0ErrorThresh << endl;
  cout << "  reco_muonDzErrorThresh          = " << reco_muonDzErrorThresh << endl;
  cout << "  reco_jetD0ErrorThresh           = " << reco_jetD0ErrorThresh << endl;
  cout << "  reco_jetDzErrorThresh           = " << reco_jetDzErrorThresh << endl;
  cout << "  reco_ufoD0ErrorThresh           = " << reco_ufoD0ErrorThresh << endl;
  cout << "  reco_ufoDzErrorThresh           = " << reco_ufoDzErrorThresh << endl;


    return;    
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintMCCuts()
{ 
 // Prints a summary of the parameters for MCProcessor
 
  // load the parameters for MCProcessor
  int   mc_numEvtPrnt = mcproc_params.getParameter<int>("mc_numEvtPrnt") ;
  float mc_PhotCalFac = mcproc_params.getParameter<double>("mc_PhotCalFac") ;
  float mc_JetCalFac = mcproc_params.getParameter<double>("mc_JetCalFac") ;
  float mc_JetDeltaRIC = mcproc_params.getParameter<double>("mc_JetDeltaRIC") ;

  cout << endl;
  cout << "Parameters for MCProcessor: " << endl;
  cout << "  mc_numEvtPrnt                   = " << mc_numEvtPrnt << endl;
  cout << "  mc_PhotCalFac                   = " << mc_PhotCalFac << endl;
  cout << "  mc_JetCalFac                    = " << mc_JetCalFac << endl;
  cout << "  mc_JetDeltaRIC                  = " << mc_JetDeltaRIC << endl;


    return;    
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintCleanerCuts()
{ 
 // Prints a summary of the parameters for ObjectCleaner
 
  // load the parameters for ObjectCleaner
  float clean_chisqVxmax = cleaner_params.getParameter<double>("clean_chisqVxmax") ;
  float clean_dRVxmax = cleaner_params.getParameter<double>("clean_dRVxmax") ;
  float clean_dzVxmax = cleaner_params.getParameter<double>("clean_dzVxmax") ;
  float clean_etaTkfromVxmax = cleaner_params.getParameter<double>("clean_etaTkfromVxmax") ;
  float clean_sumPtTkfromVxmin = cleaner_params.getParameter<double>("clean_sumPtTkfromVxmin") ;
  int   clean_methodTksInJetVx = cleaner_params.getParameter<int>("clean_methodTksInJetVx") ;
  int   clean_nJetVxTkHitsmin = cleaner_params.getParameter<int>("clean_nJetVxTkHitsmin") ;
  float clean_jetVxCaloTowEFracmin = cleaner_params.getParameter<double>("clean_jetVxCaloTowEFracmin") ;
  float clean_dRTrkFromJetVx = cleaner_params.getParameter<double>("clean_dRTrkFromJetVx") ;
  float clean_distVxmax = cleaner_params.getParameter<double>("clean_distVxmax") ;

  bool clean_UserDefinedElecID = cleaner_params.getParameter<bool> ("clean_UserDefinedElecID"); 
  float clean_ElecHoverEBarmax = cleaner_params.getParameter<double>("clean_ElecHoverEBarmax") ;
  float clean_ElecSigmaEtaEtaBarmax = cleaner_params.getParameter<double>("clean_ElecSigmaEtaEtaBarmax") ;
  float clean_ElecSigmaPhiPhiBarmax = cleaner_params.getParameter<double>("clean_ElecSigmaPhiPhiBarmax") ;
  float clean_ElecDeltaEtaInBarmax = cleaner_params.getParameter<double>("clean_ElecDeltaEtaInBarmax") ;
  float clean_ElecDeltaPhiInBarmax = cleaner_params.getParameter<double>("clean_ElecDeltaPhiInBarmax") ;
  float clean_ElecDeltaPhiOutBarmax = cleaner_params.getParameter<double>("clean_ElecDeltaPhiOutBarmax") ;
  float clean_ElecEoverPInBarmin = cleaner_params.getParameter<double>("clean_ElecEoverPInBarmin") ;
  float clean_ElecEoverPOutBarmin = cleaner_params.getParameter<double>("clean_ElecEoverPOutBarmin") ;
  float clean_ElecInvEMinusInvPBarmax = cleaner_params.getParameter<double>("clean_ElecInvEMinusInvPBarmax") ;
  float clean_ElecE9overE25Barmin = cleaner_params.getParameter<double>("clean_ElecE9overE25Barmin") ;
//  float clean_ElecBremFractionBarmax = cleaner_params.getParameter<double>("clean_ElecBremFractionBarmax") ;
  float clean_ElecHoverEEndmax = cleaner_params.getParameter<double>("clean_ElecHoverEEndmax") ;
  float clean_ElecSigmaEtaEtaEndmax = cleaner_params.getParameter<double>("clean_ElecSigmaEtaEtaEndmax") ;
  float clean_ElecSigmaPhiPhiEndmax = cleaner_params.getParameter<double>("clean_ElecSigmaPhiPhiEndmax") ;
  float clean_ElecDeltaEtaInEndmax = cleaner_params.getParameter<double>("clean_ElecDeltaEtaInEndmax") ;
  float clean_ElecDeltaPhiInEndmax = cleaner_params.getParameter<double>("clean_ElecDeltaPhiInEndmax") ;
  float clean_ElecDeltaPhiOutEndmax = cleaner_params.getParameter<double>("clean_ElecDeltaPhiOutEndmax") ;
  float clean_ElecEoverPInEndmin = cleaner_params.getParameter<double>("clean_ElecEoverPInEndmin") ;
  float clean_ElecEoverPOutEndmin = cleaner_params.getParameter<double>("clean_ElecEoverPOutEndmin") ;
  float clean_ElecInvEMinusInvPEndmax = cleaner_params.getParameter<double>("clean_ElecInvEMinusInvPEndmax") ;
  float clean_ElecE9overE25Endmin = cleaner_params.getParameter<double>("clean_ElecE9overE25Endmin") ;
//  float clean_ElecBremFractionEndmax = cleaner_params.getParameter<double>("clean_ElecBremFractionEndmax") ;
  std::string ecutquality = cleaner_params.getParameter<std::string>("electronQuality");

 
  float clean_dRSSelecmax = cleaner_params.getParameter<double>("clean_dRSSelecmax") ;
  float clean_MuonDPbyPmax = cleaner_params.getParameter<double>("clean_MuonDPbyPmax") ;
  float clean_MuonChi2max = cleaner_params.getParameter<double>("clean_MuonChi2max") ;
  float clean_MuonNHitsmin = cleaner_params.getParameter<double>("clean_MuonNHitsmin") ;
  float clean_dRMuonTowermax = cleaner_params.getParameter<double>("clean_dRMuonTowermax") ;
  float clean_dRSSmuonmax = cleaner_params.getParameter<double>("clean_dRSSmuonmax") ;
  float clean_dRPhotTowermax = cleaner_params.getParameter<double>("clean_dRPhotTowermax") ;
  float clean_PhotHoEmax = cleaner_params.getParameter<double>("clean_PhotHoEmax") ;
  float clean_dRPhotElemax = cleaner_params.getParameter<double>("clean_dRPhotElemax") ;
  float clean_dRPhotDupmax = cleaner_params.getParameter<double>("clean_dRPhotDupmax") ;
  float clean_deltaRElecJetmax =cleaner_params.getParameter<double>("clean_deltaRElecJetmax") ;
  float clean_elecbyJetEratio = cleaner_params.getParameter<double>("clean_elecbyJetEratio") ;
  int   clean_methodTksInJet = cleaner_params.getParameter<int>("clean_methodTksInJet") ;
  int   clean_nJetTkHitsmin = cleaner_params.getParameter<int>("clean_nJetTkHitsmin") ;
  float clean_jetCaloTowEFracmin = cleaner_params.getParameter<double>("clean_jetCaloTowEFracmin") ;
  float clean_dRTrkFromJet = cleaner_params.getParameter<double>("clean_dRTrkFromJet") ;
  float clean_FracChminJet = cleaner_params.getParameter<double>("clean_FracChminJet") ;
  float clean_FracEmmaxJet = cleaner_params.getParameter<double>("clean_FracEmmaxJet") ;
  float clean_rejEvtBadJetPtmin = cleaner_params.getParameter<double>("clean_rejEvtBadJetPtmin") ;
  float clean_dROSelecmax = cleaner_params.getParameter<double>("clean_dROSelecmax") ;
  float clean_MOSelecmax = cleaner_params.getParameter<double>("clean_MOSelecmax") ;
  float clean_FracChmin = cleaner_params.getParameter<double>("clean_FracChmin") ;
  float clean_FracEmmin = cleaner_params.getParameter<double>("clean_FracEmmin") ;
  float clean_METmin = cleaner_params.getParameter<double>("clean_METmin") ;
  float clean_dPhiJetMETmin = cleaner_params.getParameter<double>("clean_dPhiJetMETmin") ;
  float clean_dR12min = cleaner_params.getParameter<double>("clean_dR12min") ;
  float clean_dR21min = cleaner_params.getParameter<double>("clean_dR21min") ;

  cout << endl;
  cout << "Parameters for ObjectCleaner: " << endl;
  cout << "  clean_chisqVxmax                = " << clean_chisqVxmax << endl;
  cout << "  clean_dRVxmax                   = " << clean_dRVxmax << endl;
  cout << "  clean_dzVxmax                   = " << clean_dzVxmax << endl;
  cout << "  clean_etaTkfromVxmax            = " << clean_etaTkfromVxmax << endl;
  cout << "  clean_sumPtTkfromVxmin          = " << clean_sumPtTkfromVxmin << endl;
  cout << "  clean_methodTksInJetVx          = " << clean_methodTksInJetVx << endl;
  if (useAODOnly && clean_methodTksInJetVx == 1){
    cout << "  ==> cannot be = 1 if AOD only, please change to 2 or 3 *****" << endl;
    cout << "  the same applies to clean_methodTksInJet  *****" << endl;
    exit(1);
  }
  cout << "  clean_nJetVxTkHitsmin           = " << clean_nJetVxTkHitsmin << endl;
  cout << "  clean_jetVxCaloTowEFracmin      = " << clean_jetVxCaloTowEFracmin << endl;
  cout << "  clean_dRTrkFromJetVx            = " << clean_dRTrkFromJetVx << endl;
  cout << "  clean_distVxmax                 = " << clean_distVxmax << endl; 
  if(clean_UserDefinedElecID) {
  cout << "  UserDefinedElecID used with the following parameters: " << endl;   
  cout << "  clean_ElecHoverEBarmax          = " << clean_ElecHoverEBarmax << endl;
  cout << "  clean_ElecHoverEEndmax          = " << clean_ElecHoverEEndmax << endl;
  cout << "  clean_ElecSigmaEtaEtaBarmax     = " << clean_ElecSigmaEtaEtaBarmax << endl;
  cout << "  clean_ElecSigmaEtaEtaEndmax     = " << clean_ElecSigmaEtaEtaEndmax << endl;
  cout << "  clean_ElecSigmaPhiPhiBarmax     = " << clean_ElecSigmaPhiPhiBarmax << endl;
  cout << "  clean_ElecSigmaPhiPhiEndmax     = " << clean_ElecSigmaPhiPhiEndmax << endl;
  cout << "  clean_ElecDeltaEtaInBarmax      = " << clean_ElecDeltaEtaInBarmax << endl;
  cout << "  clean_ElecDeltaEtaInEndmax      = " << clean_ElecDeltaEtaInEndmax << endl;
  cout << "  clean_ElecDeltaPhiInBarmax      = " << clean_ElecDeltaPhiInBarmax << endl;
  cout << "  clean_ElecDeltaPhiInEndmax      = " <<  clean_ElecDeltaPhiInEndmax<< endl;
  cout << "  clean_ElecDeltaPhiOutBarmax     = " <<  clean_ElecDeltaPhiOutBarmax<< endl;
  cout << "  clean_ElecDeltaPhiOutEndmax     = " <<  clean_ElecDeltaPhiOutEndmax<< endl;
  cout << "  clean_ElecEoverPInBarmin        = " << clean_ElecEoverPInBarmin << endl;
  cout << "  clean_ElecEoverPInEndmin        = " << clean_ElecEoverPInEndmin << endl;
  cout << "  clean_ElecEoverPOutBarmin       = " << clean_ElecEoverPOutBarmin << endl;
  cout << "  clean_ElecEoverPOutEndmin       = " <<  clean_ElecEoverPOutEndmin<< endl;
  cout << "  clean_ElecInvEMinusInvPBarmax   = " << clean_ElecInvEMinusInvPBarmax << endl;
  cout << "  clean_ElecInvEMinusInvPEndmax   = " << clean_ElecInvEMinusInvPEndmax << endl;
  cout << "  clean_ElecE9overE25Barmin       = " <<  clean_ElecE9overE25Barmin<< endl;
  cout << "  clean_ElecE9overE25Endmin       = " << clean_ElecE9overE25Endmin << endl; 
  } else {
  cout << "  Egamma ElectronID tables used. Selection type: " << ecutquality << endl;
  } 
  cout << "  clean_dRSSelecmax               = " << clean_dRSSelecmax << endl;
  cout << "  clean_MuonDPbyPmax              = " << clean_MuonDPbyPmax << endl;
  cout << "  clean_MuonChi2max               = " << clean_MuonChi2max << endl;
  cout << "  clean_MuonNHitsmin              = " << clean_MuonNHitsmin << endl;
  cout << "  clean_dRMuonTowermax            = " << clean_dRMuonTowermax << endl;
  cout << "  clean_dRSSmuonmax               = " << clean_dRSSmuonmax << endl;
  cout << "  clean_dRPhotTowermax            = " << clean_dRPhotTowermax << endl;
  cout << "  clean_PhotHoEmax                = " << clean_PhotHoEmax << endl;
  cout << "  clean_dRPhotElemax              = " << clean_dRPhotElemax << endl;
  cout << "  clean_dRPhotDupmax              = " << clean_dRPhotDupmax << endl;
  cout << "  clean_deltaRElecJetmax          = " << clean_deltaRElecJetmax << endl;
  cout << "  clean_elecbyJetEratio           = " << clean_elecbyJetEratio << endl;
  cout << "  clean_methodTksInJet            = " << clean_methodTksInJet << endl;
  if (useAODOnly && clean_methodTksInJet == 1){
    cout << " ==> cannot be = 1 if AOD only, please change to 2 or 3 *****" << endl;
    exit(1);
  }
  cout << "  clean_nJetTkHitsmin             = " << clean_nJetTkHitsmin << endl;
  cout << "  clean_jetCaloTowEFracmin        = " << clean_jetCaloTowEFracmin << endl;
  cout << "  clean_dRTrkFromJet              = " << clean_dRTrkFromJet << endl;
  cout << "  clean_FracChminJet              = " << clean_FracChminJet << endl;
  cout << "  clean_FracEmmaxJet              = " << clean_FracEmmaxJet << endl;
  cout << "  clean_rejEvtBadJetPtmin         = " << clean_rejEvtBadJetPtmin << endl;
  cout << "  clean_dROSelecmax               = " << clean_dROSelecmax << endl;
  cout << "  clean_MOSelecmax                = " << clean_MOSelecmax << endl;
  cout << "  clean_FracChmin                 = " << clean_FracChmin << endl;
  cout << "  clean_FracEmmin                 = " << clean_FracEmmin << endl;
  cout << "  clean_METmin                    = " << clean_METmin << endl;
  cout << "  clean_dPhiJetMETmin             = " << clean_dPhiJetMETmin << endl;
  cout << "  clean_dR12min                   = " << clean_dR12min << endl;
  cout << "  clean_dR21min                   = " << clean_dR21min << endl;


    return;    
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintIsolatorCuts()
{ 
 // Prints a summary of the parameters for the Isolator
 
  // load the parameters for the Isolator
  int   iso_MethodElec = isolator_params.getParameter<int>("iso_MethodElec") ;
  float iso_jetbyElEmin = isolator_params.getParameter<double>("iso_jetbyElEmin") ;
  float iso_ptElwrtJetmin = isolator_params.getParameter<double>("iso_ptElwrtJetmin") ;
  float iso_ElCalDRin = isolator_params.getParameter<double>("iso_ElCalDRin") ;
  float iso_ElCalDRout = isolator_params.getParameter<double>("iso_ElCalDRout") ;
  float iso_ElCalSeed = isolator_params.getParameter<double>("iso_ElCalSeed") ;
  float iso_ElTkDRin = isolator_params.getParameter<double>("iso_ElTkDRin") ;
  float iso_ElTkDRout = isolator_params.getParameter<double>("iso_ElTkDRout") ;
  float iso_ElTkSeed = isolator_params.getParameter<double>("iso_ElTkSeed") ;
  float iso_ElCalWeight = isolator_params.getParameter<double>("iso_ElCalWeight") ;
  float iso_ElIsoValue = isolator_params.getParameter<double>("iso_ElIsoValue") ;
  int   iso_MethodMuon = isolator_params.getParameter<int>("iso_MethodMuon") ;
  float iso_jetbyMuEmin = isolator_params.getParameter<double>("iso_jetbyMuEmin") ;
  float iso_ptMuwrtJetmin = isolator_params.getParameter<double>("iso_ptMuwrtJetmin") ;
  float iso_MuCalDRin = isolator_params.getParameter<double>("iso_MuCalDRin") ;
  float iso_MuCalDRout = isolator_params.getParameter<double>("iso_MuCalDRout") ;
  float iso_MuCalSeed = isolator_params.getParameter<double>("iso_MuCalSeed") ;
  float iso_MuTkDRin = isolator_params.getParameter<double>("iso_MuTkDRin") ;
  float iso_MuTkDRout = isolator_params.getParameter<double>("iso_MuTkDRout") ;
  float iso_MuTkSeed = isolator_params.getParameter<double>("iso_MuTkSeed") ;
  float iso_MuCalWeight = isolator_params.getParameter<double>("iso_MuCalWeight") ;
  float iso_MuIsoValue = isolator_params.getParameter<double>("iso_MuIsoValue") ;
  int   iso_MethodTau = isolator_params.getParameter<int>("iso_MethodTau") ;
  float iso_jetbyTauEmin = isolator_params.getParameter<double>("iso_jetbyTauEmin") ;
  float iso_ptTauwrtJetmin = isolator_params.getParameter<double>("iso_ptTauwrtJetmin") ;
  float iso_TauCalDRin = isolator_params.getParameter<double>("iso_TauCalDRin") ;
  float iso_TauCalDRout = isolator_params.getParameter<double>("iso_TauCalDRout") ;
  float iso_TauCalSeed = isolator_params.getParameter<double>("iso_TauCalSeed") ;
  float iso_TauTkDRin = isolator_params.getParameter<double>("iso_TauTkDRin") ;
  float iso_TauTkDRout = isolator_params.getParameter<double>("iso_TauTkDRout") ;
  float iso_TauTkSeed = isolator_params.getParameter<double>("iso_TauTkSeed") ;
  float iso_TauCalWeight = isolator_params.getParameter<double>("iso_TauCalWeight") ;
  float iso_TauIsoValue = isolator_params.getParameter<double>("iso_TauIsoValue") ;
  int   iso_MethodPhot = isolator_params.getParameter<int>("iso_MethodPhot") ;
  float iso_jetbyPhotEmin = isolator_params.getParameter<double>("iso_jetbyPhotEmin") ;
  float iso_ptPhotwrtJetmin = isolator_params.getParameter<double>("iso_ptPhotwrtJetmin") ;
  float iso_PhCalDRin = isolator_params.getParameter<double>("iso_PhCalDRin") ;
  float iso_PhCalDRout = isolator_params.getParameter<double>("iso_PhCalDRout") ;
  float iso_PhCalSeed = isolator_params.getParameter<double>("iso_PhCalSeed") ;
  float iso_PhTkDRin = isolator_params.getParameter<double>("iso_PhTkDRin") ;
  float iso_PhTkDRout = isolator_params.getParameter<double>("iso_PhTkDRout") ;
  float iso_PhTkSeed = isolator_params.getParameter<double>("iso_PhTkSeed") ;
  float iso_PhCalWeight = isolator_params.getParameter<double>("iso_PhCalWeight") ;
  float iso_PhIsoValue = isolator_params.getParameter<double>("iso_PhIsoValue") ;
  int   iso_MethodUfo = isolator_params.getParameter<int>("iso_MethodUfo") ;
  float iso_jetbyUfoEmin = isolator_params.getParameter<double>("iso_jetbyUfoEmin") ;
  float iso_ptUfowrtJetmin = isolator_params.getParameter<double>("iso_ptUfowrtJetmin") ;
  float iso_UfoCalDRin = isolator_params.getParameter<double>("iso_UfoCalDRin") ;
  float iso_UfoCalDRout = isolator_params.getParameter<double>("iso_UfoCalDRout") ;
  float iso_UfoCalSeed = isolator_params.getParameter<double>("iso_UfoCalSeed") ;
  float iso_UfoTkDRin = isolator_params.getParameter<double>("iso_UfoTkDRin") ;
  float iso_UfoTkDRout = isolator_params.getParameter<double>("iso_UfoTkDRout") ;
  float iso_UfoTkSeed = isolator_params.getParameter<double>("iso_UfoTkSeed") ;
  float iso_UfoCalWeight = isolator_params.getParameter<double>("iso_UfoCalWeight") ;
  float iso_UfoIsoValue = isolator_params.getParameter<double>("iso_UfoIsoValue") ;
  float iso_DRJetMergemax = isolator_params.getParameter<double>("iso_DRJetMergemax") ;

  cout << endl;
  cout << "Parameters for Isolator: " << endl;
  cout << "  iso_MethodElec                  = " << iso_MethodElec << endl;
  cout << "  iso_jetbyElEmin                 = " << iso_jetbyElEmin << endl;
  cout << "  iso_ptElwrtJetmin               = " << iso_ptElwrtJetmin << endl;
  cout << "  iso_ElCalDRin                   = " << iso_ElCalDRin << endl;
  cout << "  iso_ElCalDRout                  = " << iso_ElCalDRout << endl;
  cout << "  iso_ElCalSeed                   = " << iso_ElCalSeed << endl;
  cout << "  iso_ElTkDRin                    = " << iso_ElTkDRin << endl;
  cout << "  iso_ElTkDRout                   = " << iso_ElTkDRout << endl;
  cout << "  iso_ElTkSeed                    = " << iso_ElTkSeed << endl;
  cout << "  iso_ElCalWeight                 = " << iso_ElCalWeight << endl;
  cout << "  iso_ElIsoValue                  = " << iso_ElIsoValue << endl;
  cout << "  iso_MethodMuon                  = " << iso_MethodMuon << endl;
  cout << "  iso_jetbyMuEmin                 = " << iso_jetbyMuEmin << endl;
  cout << "  iso_ptMuwrtJetmin               = " << iso_ptMuwrtJetmin << endl;
  cout << "  iso_MuCalDRin                   = " <<iso_MuCalDRin  << endl;
  cout << "  iso_MuCalDRout                  = " << iso_MuCalDRout << endl;
  cout << "  iso_MuCalSeed                   = " << iso_MuCalSeed << endl;
  cout << "  iso_MuTkDRin                    = " << iso_MuTkDRin << endl;
  cout << "  iso_MuTkDRout                   = " << iso_MuTkDRout << endl;
  cout << "  iso_MuTkSeed                    = " << iso_MuTkSeed << endl;
  cout << "  iso_MuCalWeight                 = " << iso_MuCalWeight << endl;
  cout << "  iso_MuIsoValue                  = " << iso_MuIsoValue << endl;
  cout << "  iso_MethodTau                   = " << iso_MethodTau << endl;
  cout << "  iso_jetbyTauEmin                = " << iso_jetbyTauEmin << endl;
  cout << "  iso_ptTauwrtJetmin              = " << iso_ptTauwrtJetmin << endl;
  cout << "  iso_TauCalDRin                  = " << iso_TauCalDRin << endl;
  cout << "  iso_TauCalDRout                 = " << iso_TauCalDRout << endl;
  cout << "  iso_TauCalSeed                  = " << iso_TauCalSeed << endl;
  cout << "  iso_TauTkDRin                   = " << iso_TauTkDRin << endl;
  cout << "  iso_TauTkDRout                  = " << iso_TauTkDRout << endl;
  cout << "  iso_TauTkSeed                   = " << iso_TauTkSeed << endl;
  cout << "  iso_TauCalWeight                = " << iso_TauCalWeight << endl;
  cout << "  iso_TauIsoValue                 = " << iso_TauIsoValue << endl;
  cout << "  iso_MethodPhot                  = " << iso_MethodPhot << endl;
  cout << "  iso_jetbyPhotEmin               = " << iso_jetbyPhotEmin << endl;
  cout << "  iso_ptPhotwrtJetmin             = " << iso_ptPhotwrtJetmin << endl;
  cout << "  iso_PhCalDRin                   = " << iso_PhCalDRin << endl;
  cout << "  iso_PhCalDRout                  = " << iso_PhCalDRout << endl;
  cout << "  iso_PhCalSeed                   = " << iso_PhCalSeed << endl;
  cout << "  iso_PhTkDRin                    = " << iso_PhTkDRin << endl;
  cout << "  iso_PhTkDRout                   = " << iso_PhTkDRout << endl;
  cout << "  iso_PhTkSeed                    = " << iso_PhTkSeed << endl;
  cout << "  iso_PhCalWeight                 = " << iso_PhCalWeight << endl;
  cout << "  iso_PhIsoValue                  = " << iso_PhIsoValue << endl;
  cout << "  iso_MethodUfo                   = " << iso_MethodUfo << endl;
  cout << "  iso_jetbyUfoEmin                 = " << iso_jetbyUfoEmin << endl;
  cout << "  iso_ptUfowrtJetmin               = " << iso_ptUfowrtJetmin << endl;
  cout << "  iso_UfoCalDRin                   = " <<iso_UfoCalDRin  << endl;
  cout << "  iso_UfoCalDRout                  = " << iso_UfoCalDRout << endl;
  cout << "  iso_UfoCalSeed                   = " << iso_UfoCalSeed << endl;
  cout << "  iso_UfoTkDRin                    = " << iso_UfoTkDRin << endl;
  cout << "  iso_UfoTkDRout                   = " << iso_UfoTkDRout << endl;
  cout << "  iso_UfoTkSeed                    = " << iso_UfoTkSeed << endl;
  cout << "  iso_UfoCalWeight                 = " << iso_UfoCalWeight << endl;
  cout << "  iso_UfoIsoValue                  = " << iso_UfoIsoValue << endl;
  cout << "  iso_DRJetMergemax                = " << iso_DRJetMergemax << endl;


    return;    
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintObjectMatchingCuts()
{ 
 // Prints a summary of the parameters for MatchObjects
 
  // load the parameters for MCProcessor
  float mo_elecDRmax = objectmatch_params.getParameter<double>("mo_elecDRmax") ;
  float mo_elecDPbyPmax = objectmatch_params.getParameter<double>("mo_elecDPbyPmax") ;
  float mo_muonDRmax = objectmatch_params.getParameter<double>("mo_muonDRmax") ;
  float mo_muonDPbyPmax = objectmatch_params.getParameter<double>("mo_muonDPbyPmax") ;
  float mo_photonDRmax = objectmatch_params.getParameter<double>("mo_photonDRmax") ;
  float mo_photonDPbyPmax = objectmatch_params.getParameter<double>("mo_photonDPbyPmax") ;
  float mo_jetDRmax = objectmatch_params.getParameter<double>("mo_jetDRmax") ;
  float mo_jetDPbyPmax = objectmatch_params.getParameter<double>("mo_jetDPbyPmax") ;
  float mo_ufoDRmax = objectmatch_params.getParameter<double>("mo_ufoDRmax") ;
  float mo_ufoDPbyPmax = objectmatch_params.getParameter<double>("mo_ufoDPbyPmax") ;
  float mo_celecDRmax = objectmatch_params.getParameter<double>("mo_celecDRmax") ;
  float mo_cmuonDRmax = objectmatch_params.getParameter<double>("mo_cmuonDRmax") ;
  float mo_cphotonDRmax = objectmatch_params.getParameter<double>("mo_cphotonDRmax") ;
  float mo_cjetDRmax = objectmatch_params.getParameter<double>("mo_cjetDRmax") ;
  float mo_cufoDRmax = objectmatch_params.getParameter<double>("mo_cufoDRmax") ;

  cout << endl;
  cout << "Parameters for : MatchObjects" << endl;
  cout << "  mo_elecDRmax                    = " << mo_elecDRmax << endl;
  cout << "  mo_elecDPbyPmax                 = " << mo_elecDPbyPmax << endl;
  cout << "  mo_muonDRmax                    = " << mo_muonDRmax << endl;
  cout << "  mo_muonDPbyPmax                 = " << mo_muonDPbyPmax << endl;
  cout << "  mo_photonDRmax                  = " << mo_photonDRmax << endl;
  cout << "  mo_photonDPbyPmax               = " << mo_photonDPbyPmax << endl;
  cout << "  mo_jetDRmax                     = " << mo_jetDRmax << endl;
  cout << "  mo_jetDPbyPmax                  = " << mo_jetDPbyPmax << endl;
  cout << "  mo_ufoDRmax                     = " << mo_ufoDRmax << endl;
  cout << "  mo_ufoDPbyPmax                  = " << mo_ufoDPbyPmax << endl;
  cout << "  mo_celecDRmax                   = " << mo_celecDRmax << endl;
  cout << "  mo_cmuonDRmax                   = " << mo_cmuonDRmax << endl;
  cout << "  mo_cphotonDRmax                 = " << mo_cphotonDRmax << endl;
  cout << "  mo_cjetDRmax                    = " << mo_cjetDRmax << endl;
  cout << "  mo_cufoDRmax                   = " << mo_cufoDRmax << endl;


    return;    
}

//------------------------------------------------------------------------------

void SusyAnalyzer::PrintStatistics(void)
{ 
  // Final output of the run statistics
  
 bool rej_NoTriggerData = rejectEvent_params.getParameter<bool>("rej_NoTriggerData") ;
// bool rej_NoL1fired = rejectEvent_params.getParameter<bool>("rej_NoL1fired") ;
// bool rej_NoHLTfired = rejectEvent_params.getParameter<bool>("rej_NoHLTfired") ;
 bool rej_MissingRecoData = rejectEvent_params.getParameter<bool>("rej_MissingRecoData") ;
 bool rej_MissingTrackData = rejectEvent_params.getParameter<bool>("rej_MissingTrackData") ;
 bool rej_MissingCaloTowers = rejectEvent_params.getParameter<bool>("rej_MissingCaloTowers") ;
 bool rej_Empty = rejectEvent_params.getParameter<bool>("rej_Empty") ;
 bool rej_NoPrimary = rejectEvent_params.getParameter<bool>("rej_NoPrimary") ;
 bool rej_BadHardJet = rejectEvent_params.getParameter<bool>("rej_BadHardJet") ;
 bool rej_CleanEmpty = rejectEvent_params.getParameter<bool>("rej_CleanEmpty") ;
 bool rej_FinalEmpty = rejectEvent_params.getParameter<bool>("rej_FinalEmpty") ;
 bool rej_BadNoisy = rejectEvent_params.getParameter<bool>("rej_BadNoisy") ;
 bool rej_BadMET = rejectEvent_params.getParameter<bool>("rej_BadMET") ;
 bool rej_BadHemis = rejectEvent_params.getParameter<bool>("rej_BadHemis") ;
 
 cout << endl;
 cout << "Trigger Statistics : " << endl;
 cout << "   ***L1          : " << endl;
 cout << "        Before Any Cut: " << endl;
 for (int i=0; i<(int)numTotL1BitsBeforeCuts.size(); i++) {
   cout << "      Bit n. " << right << setw(4) << i << "   " << left << setw(29) << l1Names[i] <<"\t Fired " << right << setw(7) << numTotL1BitsBeforeCuts[i] << 
    " \t times \t (" << 100.*(float)numTotL1BitsBeforeCuts[i]/(float)numTotEvt << " %) " << endl;
 }
 cout << "   ***HLT          : " << endl;
 cout << "        Before Any Cut: " << endl;
 for (int i=0; i<(int)numTotHltBitsBeforeCuts.size(); i++) {
    cout << "      Bit n. " << right << setw(4) << i << "   " << left << setw(29) << hlNames[i] <<"\t Fired " << right << setw(7) << numTotHltBitsBeforeCuts[i] << 
    " \t times \t (" << 100.*(float)numTotHltBitsBeforeCuts[i]/(float)numTotEvt << " %) " << endl;
 }
 
 cout << endl;
 cout << "Total number of events processed = " << numTotEvt << endl;
 numTotEvtReject = numTotEvtExceptCaught;
 numTotEvtReject += numTotMCReject;
 if (rej_NoTriggerData){numTotEvtReject += numTotEvtNoTrigger;}
 if (rej_MissingRecoData){numTotEvtReject += numTotEvtNoReco;}
 if (rej_MissingTrackData){numTotEvtReject += numTotEvtNoTracks;}
 if (rej_MissingCaloTowers){numTotEvtReject += numTotEvtNoCalo;}
 if (rej_Empty){numTotEvtReject += numTotEvtEmpty;}
 if (rej_NoPrimary){numTotEvtReject += numTotEvtNoPrimary;}
 if (rej_BadHardJet){numTotEvtReject += numTotEvtBadHardJet;}
 if (rej_CleanEmpty){numTotEvtReject += numTotEvtCleanEmpty;}
 if (rej_FinalEmpty){numTotEvtReject += numTotEvtFinalEmpty;}
 if (rej_BadNoisy){numTotEvtReject += numTotEvtBadNoisy;}
 if (rej_BadMET){numTotEvtReject += numTotEvtBadMET;}
 if (rej_BadHemis){numTotEvtReject += numTotEvtBadHemi;}
 cout << "   events accepted                  = " 
      << numTotEvt-numTotEvtReject << endl;
 cout << "   events rejected (total)          = " << numTotEvtReject
      << "  = " << 100.*(float)numTotEvtReject / (float)numTotEvt << " %" << endl;
 cout << "    rejected as no MC data          = " << numTotMCReject 
      << "  = " << 100.*(float)numTotMCReject / (float)numTotEvt  << "  % *****" << endl;
 cout << "    rejected as exception           = " << numTotEvtExceptCaught 
      << "  = " << 100.*(float)numTotEvtExceptCaught / (float)numTotEvt  << "  % *****" << endl;
 cout << "    without trigger                 = " << numTotEvtNoTrigger
      << "  = " << 100.*(float)numTotEvtNoTrigger / (float)numTotEvt << " %";
 if (rej_NoTriggerData){cout << " rejected ";}
 cout << endl;
 cout << "    without Reco Data               = " << numTotEvtNoReco
      << "  = " << 100.*(float)numTotEvtNoReco / (float)numTotEvt << " %";
 if (rej_MissingRecoData){cout << " rejected ";}
 cout << endl;
 cout << "    without Track Data              = " << numTotEvtNoTracks
      << "  = " << 100.*(float)numTotEvtNoTracks / (float)numTotEvt << " %";
 if (rej_MissingTrackData){cout << " rejected ";}
 cout << endl;
 cout << "    without Calo Tower Data         = " << numTotEvtNoCalo
      << "  = " << 100.*(float)numTotEvtNoCalo / (float)numTotEvt << " %";
 if (rej_MissingCaloTowers){cout << " rejected ";}
 cout << endl;
 cout << "    empty after primary acceptance  = " << numTotEvtEmpty
      << "  = " << 100.*(float)numTotEvtEmpty / (float)numTotEvt << " %";
 if (rej_Empty){cout << " rejected ";}
 cout << endl;
 cout << "    without good primary Vx         = " << numTotEvtNoPrimary
      << "  = " << 100.*(float)numTotEvtNoPrimary / (float)numTotEvt << " %";
 if (rej_NoPrimary){cout << " rejected ";}
 cout << endl;
 cout << "    with  rejected bad hard jet     = " << numTotEvtBadHardJet
      << "  = " << 100.*(float)numTotEvtBadHardJet / (float)numTotEvt << " %";
 if (rej_BadHardJet){cout << " rejected ";}
 cout << endl;
 cout << "    empty after cleaning            = " << numTotEvtCleanEmpty
      << "  = " << 100.*(float)numTotEvtCleanEmpty / (float)numTotEvt << " %";
 if (rej_CleanEmpty){cout << " rejected ";}
 cout << endl;
 cout << "    empty after final acceptance    = " << numTotEvtFinalEmpty
      << "  = " << 100.*(float)numTotEvtFinalEmpty / (float)numTotEvt << " %";
 if (rej_FinalEmpty){cout << " rejected ";}
 cout << endl;
 cout << "    event is bad or noisy           = " << numTotEvtBadNoisy
      << "  = " << 100.*(float)numTotEvtBadNoisy / (float)numTotEvt << " %";
 if (rej_BadNoisy){cout << " rejected ";}
 cout << endl;
 cout << "    event has bad MET               = " << numTotEvtBadMET
      << "  = " << 100.*(float)numTotEvtBadMET / (float)numTotEvt << " %";
 if (rej_BadMET){cout << " rejected ";}
 cout << endl;
 cout << "    bad hemispheres matching        = " << numTotEvtBadHemi
      << "  = " << 100.*(float)numTotEvtBadHemi / (float)numTotEvt << " %";
 if (rej_BadHemis){cout << " rejected ";}
 cout << endl;

 cout << endl;
 cout << "Statistics for the accepted events:" << endl;
 cout << "Objects within first acceptance cuts:" << endl;
 int numTotReject = numTotNotPrimaryTrk+numTotDuplicate;
 cout << "Rejected: " << numTotReject << endl;
 cout << "  Total number incompatible with primary vertex = " 
      << numTotNotPrimaryTrk << " rejected" << endl;
 cout << "    Electrons = " << numTotElecNotPrimaryTrk << endl;
 cout << "    Muons     = " << numTotMuonNotPrimaryTrk << endl;
 cout << "    Taus      = " << numTotTauNotPrimaryTrk << endl;
 cout << "    Photons   = " << numTotPhotNotPrimaryTrk << endl;
 cout << "    Jets      = " << numTotJetNotPrimaryTrk << endl;
 cout << "    UFOs      = " << numTotUfosNotPrimaryTrk << endl;
 cout << "  Total number of not-clean objects             = " 
      << numTotNotClean << " kept" << endl;
 int numTotElecNotClean = numTotElecNotCleanHOE + numTotElecNotCleanShsh + numTotElecNotCleanTmat;
 cout << "    Electrons = " << numTotElecNotClean << endl;
 cout << "    Muons     = " << numTotMuonNotClean << endl;
 cout << "    Taus      = " << numTotTauNotClean << endl;
 int numTotPhotNotClean = numTotPhotNotCleanHOE + numTotPhotNotCleanShsh;
 cout << "    Photons   = " << numTotPhotNotClean << endl;
 int numTotJetNotClean = numTotJetNotCleanFem + numTotJetNotCleanFtk;
 cout << "    Jets      = " << numTotJetNotClean << endl;
 cout << "    UFOs      = " << numTotUfosNotClean << endl;
 cout << "  Total number of clean duplicate objects       = " 
      << numTotDuplicate << " rejected" << endl;
 cout << "    Electrons = " << numTotElecDupl << endl;
 cout << "    Muons     = " << numTotMuonDupl << endl;
 cout << "    Taus      = " << numTotTauDupl << endl;
 cout << "    Photons   = " << numTotPhotDupl << endl;
 cout << "    Jets      = " << numTotJetDupl << endl;
 cout << "    UFOs      = " << numTotUfosDupl << endl;

 cout << endl;
 int numTotAccept = numTotElectrons+numTotMuons+numTotTaus
                   +numTotPhotons+numTotJets+numTotUfos;
 cout << "Accepted good objects: " << numTotAccept << endl;
 cout << "Total number of electrons = " << numTotElectrons 
      << " per event = " << (float)numTotElectrons / (float)numTotEvt << endl;
 int numTotElectronsIso = numTotElectrons - numTotElectronsNonIso;
 cout << "   Isolated               = " << numTotElectronsIso << endl;
 cout << "   Non isolated           = " << numTotElectronsNonIso << endl;
 cout << "Total number of muons     = " << numTotMuons 
      << " per event = " << (float)numTotMuons / (float)numTotEvt << endl;
 int numTotMuonsIso = numTotMuons - numTotMuonsNonIso;
 cout << "   Isolated               = " << numTotMuonsIso << endl;
 cout << "   Non isolated           = " << numTotMuonsNonIso << endl;
 cout << "Total number of taus      = " << numTotTaus 
      << " per event = " << (float)numTotTaus / (float)numTotEvt << endl;
 int numTotTausIso = numTotTaus - numTotTausNonIso;
 cout << "   Isolated               = " << numTotTausIso << endl;
 cout << "   Non isolated           = " << numTotTausNonIso << endl;
 cout << "Total number of photons   = " << numTotPhotons 
      << " per event = " << (float)numTotPhotons / (float)numTotEvt << endl;
 int numTotPhotonsIso = numTotPhotons - numTotPhotonsNonIso;
 cout << "   Isolated               = " << numTotPhotonsIso << endl;
 cout << "   Non isolated           = " << numTotPhotonsNonIso << endl;
 cout << "Total number of jets      = " <<  numTotJets
      << " per event = " << (float)numTotJets / (float)numTotEvt << endl;
 cout << "   Total number of b-jets = " <<  numTotBJets
      << " per event = " << (float)numTotBJets / (float)numTotEvt << endl;
 cout << "Total number of UFOs      = " << numTotUfos 
      << " per event = " << (float)numTotUfos / (float)numTotEvt << endl;
 int numTotUfosIso = numTotUfos - numTotUfosNonIso;
 cout << "   Isolated               = " << numTotUfosIso << endl;
 cout << "   Non isolated           = " << numTotUfosNonIso << endl;

 cout << endl;
 cout << "Accepted objects within final acceptance cuts:" << endl;
 cout << "Total number of electrons = " << numTotElectronsfinal 
      << " per event = " << (float)numTotElectronsfinal / (float)numTotEvt << endl;
 cout << "Total number of muons     = " << numTotMuonsfinal 
      << " per event = " << (float)numTotMuonsfinal / (float)numTotEvt << endl;
 cout << "Total number of taus      = " << numTotTausfinal 
      << " per event = " << (float)numTotTausfinal / (float)numTotEvt << endl;
 cout << "Total number of photons   = " << numTotPhotonsfinal 
      << " per event = " << (float)numTotPhotonsfinal / (float)numTotEvt << endl;
 cout << "Total number of jets      = " <<  numTotJetsfinal
      << " per event = " << (float)numTotJetsfinal / (float)numTotEvt << endl;
 cout << " Total number of b-jets   = " <<  numTotBJetsfinal
      << " per event = " << (float)numTotBJetsfinal / (float)numTotEvt << endl;
 cout << "Total number of UFOs      = " <<  numTotUfosfinal
      << " per event = " << (float)numTotUfosfinal / (float)numTotEvt << endl;

 cout << endl;
 cout << "Objects matched to MC truth:" << endl;
 cout << "Total number of matched electrons = " << numTotElectronsMatched; 
 if (numTotElectronsfinal > 0){
  cout << "  = " << 100.*(float)numTotElectronsMatched / (float)numTotElectronsfinal; 
  cout << " % of final";
 }
 cout << endl;
 cout << "Total number of matched muons     = " << numTotMuonsMatched;
 if (numTotMuonsfinal > 0){
  cout << "  = " << 100.*(float)numTotMuonsMatched / (float)numTotMuonsfinal; 
  cout << " % of final";
 }
 cout << endl;
 cout << "Total number of matched taus      = " << numTotTausMatched;
 if (numTotTausfinal > 0){
  cout << "  = " << 100.*(float)numTotTausMatched / (float)numTotTausfinal; 
  cout << " % of final";
 }
 cout << endl;
 cout << "Total number of matched photons   = " << numTotPhotonsMatched;
 if (numTotPhotonsfinal > 0){
  cout << "  = " << 100.*(float)numTotPhotonsMatched / (float)numTotPhotonsfinal; 
  cout << " % of final";
 }
 cout << endl;
 cout << "Total number of matched jets      = " << numTotJetsMatched;
 if (numTotJetsfinal > 0){
  cout << "  = " << 100.*(float)numTotJetsMatched / (float)numTotJetsfinal; 
  cout << " % of final";
 }
 cout << endl;
 cout << "Total number of matched UFOs      = " << numTotUfosMatched;
 if (numTotUfosfinal > 0){
  cout << "  = " << 100.*(float)numTotUfosMatched / (float)numTotUfosfinal; 
  cout << " % of final";
 }
 cout << endl;
 
 cout << endl;
 cout << "Efficiency for MC truth Objects within final acceptance"
      << " (=Iso matched/Iso MC):" << endl;
 cout << "Total number MC electrons = " << numTotMCElec; 
 if (numTotMCElec > 0){
  cout << "  = " << 100.*(float)numTotElectronsIsoMatched / (float)numTotMCElec; 
  cout << " % efficiency";
 }
 cout << endl;
 cout << "Total number MC muons     = " << numTotMCMuon; 
 if (numTotMCMuon > 0){
  cout << "  = " << 100.*(float)numTotMuonsIsoMatched / (float)numTotMCMuon; 
  cout << " % efficiency";
 }
 cout << endl;
 cout << "Total number MC taus      = " << numTotMCTau; 
 if (numTotMCTau > 0){
  cout << "  = " << 100.*(float)numTotTausIsoMatched / (float)numTotMCTau; 
  cout << " % efficiency";
 }
 cout << endl;
 cout << "Total number MC photons   = " << numTotMCPhot; 
 if (numTotMCPhot > 0){
  cout << "  = " << 100.*(float)numTotPhotonsMatched / (float)numTotMCPhot; 
  cout << " % efficiency";
 }
 cout << endl;
 cout << "Total number MC jets      = " << numTotMCJet; 
 if (numTotMCJet > 0){
  cout << "  = " << 100.*(float)numTotJetsMatched / (float)numTotMCJet; 
  cout << " % efficiency (very crude)";
 }
 cout << endl;
 
 cout << endl;
 cout << "Accepted not clean objects: " << numTotNotClean << endl;
 cout << "    Electrons = " << numTotElecNotClean << endl;
 cout << "      bad H/E                = " << numTotElecNotCleanHOE << endl;
 cout << "        duplicate            = " << numTotElecDuplBadHOE << endl;
 cout << "        non isolated         = " << numTotElectronsNonIsoBadHOE << endl;
 cout << "        isolated             = " << numTotElecNotCleanHOE
                                          -numTotElecDuplBadHOE-numTotElectronsNonIsoBadHOE << endl;
 cout << "      bad shower shape       = " << numTotElecNotCleanShsh << endl;
 cout << "        duplicate            = " << numTotElecDuplBadShsh << endl;
 cout << "        non isolated         = " << numTotElectronsNonIsoBadShsh << endl;
 cout << "        isolated             = " << numTotElecNotCleanShsh
                                          -numTotElecDuplBadShsh-numTotElectronsNonIsoBadShsh << endl;
 cout << "      bad track matching     = " << numTotElecNotCleanTmat << endl;
 cout << "        duplicate            = " << numTotElecDuplBadTmat << endl;
 cout << "        non isolated         = " << numTotElectronsNonIsoBadTmat << endl;
 cout << "        isolated             = " << numTotElecNotCleanTmat
                                          -numTotElecDuplBadTmat-numTotElectronsNonIsoBadTmat << endl;
 cout << "    Muons     = " << numTotMuonNotClean << endl;
 cout << "        duplicate            = " << numTotMuonDuplBad << endl;
 cout << "        non isolated         = " << numTotMuonsNonIsoBad << endl;
 cout << "        isolated             = " << numTotMuonNotClean
                                          -numTotMuonDuplBad-numTotMuonsNonIsoBad << endl;
 cout << "    Taus      = " << numTotTauNotClean << endl;
 cout << "        duplicate            = " << numTotTauDuplBad << endl;
 cout << "        non isolated         = " << numTotTausNonIsoBad << endl;
 cout << "        isolated             = " << numTotTauNotClean
                                          -numTotTauDuplBad-numTotTausNonIsoBad << endl;
 cout << "    Photons   = " << numTotPhotNotClean << endl;
 cout << "      bad H/E                = " << numTotPhotNotCleanHOE << endl;
 cout << "        duplicate            = " << numTotPhotDuplBadHOE << endl;
 cout << "        non isolated         = " << numTotPhotonsNonIsoBadHOE << endl;
 cout << "        isolated             = " << numTotPhotNotCleanHOE
                                          -numTotPhotDuplBadHOE-numTotPhotonsNonIsoBadHOE << endl;
 cout << "      bad shower shape       = " << numTotPhotNotCleanShsh << endl;
 cout << "        duplicate            = " << numTotPhotDuplBadShsh << endl;
 cout << "        non isolated         = " << numTotPhotonsNonIsoBadShsh << endl;
 cout << "        isolated             = " << numTotPhotNotCleanShsh
                                          -numTotPhotDuplBadShsh-numTotPhotonsNonIsoBadShsh << endl;
 cout << "    Jets      = " << numTotJetNotClean << endl;
 cout << "      bad EM ET fraction     = " << numTotJetNotCleanFem << endl;
 cout << "        duplicate            = " << numTotJetDuplBadFem << endl;
 cout << "      bad track PT fraction  = " << numTotJetNotCleanFtk << endl;
 cout << "        duplicate            = " << numTotJetDuplBadFtk << endl;
 cout << "    UFOs      = " << numTotUfosNotClean << endl;
 cout << "        duplicate            = " << numTotUfosDuplBad << endl;
 cout << "        non isolated         = " << numTotUfosNonIsoBad << endl;
 cout << "        isolated             = " << numTotUfosNotClean
                                          -numTotUfosDuplBad-numTotUfosNonIsoBad << endl;

 cout << endl;
 cout << "Not clean objects within final acceptance cuts:" << endl;
 cout << "electrons with bad H/E            = " <<  numTotElectronsfinalBadHOE
      << " per event = " << (float)numTotElectronsfinalBadHOE / (float)numTotEvt << endl;
 cout << "  matched                         = " <<  numTotElectronsMatchedBadHOE
      << " per event = " << (float)numTotElectronsMatchedBadHOE / (float)numTotEvt << endl;
 cout << "electrons with bad shower shape   = " <<  numTotElectronsfinalBadShsh
      << " per event = " << (float)numTotElectronsfinalBadShsh / (float)numTotEvt << endl;
 cout << "  matched                         = " <<  numTotElectronsMatchedBadShsh
      << " per event = " << (float)numTotElectronsMatchedBadShsh / (float)numTotEvt << endl;
 cout << "electrons with bad track matching = " <<  numTotElectronsfinalBadTmat
      << " per event = " << (float)numTotElectronsfinalBadTmat / (float)numTotEvt << endl;
 cout << "  matched                         = " <<  numTotElectronsMatchedBadTmat
      << " per event = " << (float)numTotElectronsMatchedBadTmat / (float)numTotEvt << endl;
 cout << "muons not clean                   = " << numTotMuonsfinalBad
      << " per event = " << (float)numTotMuonsfinalBad / (float)numTotEvt << endl;
 cout << "  matched                         = " <<  numTotMuonsMatchedBad
      << " per event = " << (float)numTotMuonsMatchedBad / (float)numTotEvt << endl;
 cout << "taus not clean                    = " << numTotTausfinalBad 
      << " per event = " << (float)numTotTausfinalBad / (float)numTotEvt << endl;
 cout << "  matched                         = " <<  numTotTausMatchedBad
      << " per event = " << (float)numTotTausMatchedBad / (float)numTotEvt << endl;
 cout << "photons with bad H/E              = " << numTotPhotonsfinalBadHOE 
      << " per event = " << (float)numTotPhotonsfinalBadHOE / (float)numTotEvt << endl;
 cout << "  matched                         = " <<  numTotPhotonsMatchedBadHOE
      << " per event = " << (float)numTotPhotonsMatchedBadHOE / (float)numTotEvt << endl;
 cout << "photons with bad shower shape     = " << numTotPhotonsfinalBadShsh 
      << " per event = " << (float)numTotPhotonsfinalBadShsh / (float)numTotEvt << endl;
 cout << "  matched                         = " <<  numTotPhotonsMatchedBadShsh
      << " per event = " << (float)numTotPhotonsMatchedBadShsh / (float)numTotEvt << endl;
 cout << "jets with bad EM ET fraction      = " <<  numTotJetsfinalBadFem
      << " per event = " << (float)numTotJetsfinalBadFem / (float)numTotEvt << endl;
 cout << "  matched                         = " <<  numTotJetsMatchedBadFem
      << " per event = " << (float)numTotJetsMatchedBadFem / (float)numTotEvt << endl;
 cout << "jets with bad track PT fraction   = " <<  numTotJetsfinalBadFtk
      << " per event = " << (float)numTotJetsfinalBadFtk / (float)numTotEvt << endl;
 cout << "  matched                         = " <<  numTotJetsMatchedBadFtk
      << " per event = " << (float)numTotJetsMatchedBadFtk / (float)numTotEvt << endl;
 cout << "UFOs not clean                    = " << numTotUfosfinalBad
      << " per event = " << (float)numTotUfosfinalBad / (float)numTotEvt << endl;
 cout << "  matched                         = " <<  numTotUfosMatchedBad
      << " per event = " << (float)numTotUfosMatchedBad / (float)numTotEvt << endl;
 
 cout << endl;
 cout << "Analysis Statistics : " << endl;
 cout << " Total Number of Events selected after SusyAnalysis cuts : " << numTotEventsAfterCuts << " (" << 100.*(float)numTotEventsAfterCuts / (float)numTotEvt << " %) " << endl;
 cout << endl;
 
 cout << "***  " << endl;
 cout << "*** End of Job " << endl;
 
 cout << "         " << endl;
 

 
 return;    
}


//------------------------------------------------------------------------------

void SusyAnalyzer::SetHistoWithStats()
{
   hEventStats->SetFillColor(kRed);hEventStats->SetDrawOption("hbar");
   hEventStats->GetXaxis()->SetBinLabel(1,"TotEvts");hEventStats->SetBinContent(1,numTotEvt);
   hEventStats->GetXaxis()->SetBinLabel(2,"EventsAfterCuts");hEventStats->SetBinContent(2,numTotEventsAfterCuts);
   hEventStats->GetXaxis()->SetBinLabel(3,"EvtReject");hEventStats->SetBinContent(3,numTotEvtReject);
   hEventStats->GetXaxis()->SetBinLabel(4,"MCReject");hEventStats->SetBinContent(4,numTotMCReject);
   hEventStats->GetXaxis()->SetBinLabel(5,"EvtExceptCaught");hEventStats->SetBinContent(5,numTotEvtExceptCaught);
   hEventStats->GetXaxis()->SetBinLabel(6,"EvtNoTrigger");hEventStats->SetBinContent(6,numTotEvtNoTrigger);
   hEventStats->GetXaxis()->SetBinLabel(7,"EvtNoReco");hEventStats->SetBinContent(7,numTotEvtNoReco);
   hEventStats->GetXaxis()->SetBinLabel(8,"EvtNoTracks");hEventStats->SetBinContent(8,numTotEvtNoTracks);
   hEventStats->GetXaxis()->SetBinLabel(9,"EvtNoCalo");hEventStats->SetBinContent(9,numTotEvtNoCalo);
   hEventStats->GetXaxis()->SetBinLabel(10,"EvtEmpty");hEventStats->SetBinContent(10,numTotEvtEmpty);
   hEventStats->GetXaxis()->SetBinLabel(11,"EvtNoPrimary");hEventStats->SetBinContent(11,numTotEvtNoPrimary);
   hEventStats->GetXaxis()->SetBinLabel(12,"EvtBadHardJet");hEventStats->SetBinContent(12,numTotEvtBadHardJet);
   hEventStats->GetXaxis()->SetBinLabel(13,"EvtCleanEmpty");hEventStats->SetBinContent(13,numTotEvtCleanEmpty);
   hEventStats->GetXaxis()->SetBinLabel(14,"EvtFinalEmpty");hEventStats->SetBinContent(14,numTotEvtFinalEmpty);
   hEventStats->GetXaxis()->SetBinLabel(15,"EvtBadNoisy");hEventStats->SetBinContent(15,numTotEvtBadNoisy);
   hEventStats->GetXaxis()->SetBinLabel(16,"EvtBadMET");hEventStats->SetBinContent(16,numTotEvtBadMET);
   hEventStats->GetXaxis()->SetBinLabel(17,"EvtBadHemi");hEventStats->SetBinContent(17,numTotEvtBadHemi);
                                                                                                                                        
   hElecMuonStats->SetFillColor(kBlue);hElecMuonStats->SetDrawOption("hbar");
   hElecMuonStats->GetXaxis()->SetBinLabel(1,"Electrons");hElecMuonStats->SetBinContent(1,numTotElectrons);
   hElecMuonStats->GetXaxis()->SetBinLabel(2,"Electronsfinal");hElecMuonStats->SetBinContent(2,numTotElectronsfinal);
   hElecMuonStats->GetXaxis()->SetBinLabel(3,"ElectronsMatched");hElecMuonStats->SetBinContent(3,numTotElectronsMatched);
   hElecMuonStats->GetXaxis()->SetBinLabel(4,"MCElec");hElecMuonStats->SetBinContent(4,numTotMCElec);
   hElecMuonStats->GetXaxis()->SetBinLabel(5,"MCElecBigEta");hElecMuonStats->SetBinContent(5,numTotMCElecBigEta);
   hElecMuonStats->GetXaxis()->SetBinLabel(6,"Muons");hElecMuonStats->SetBinContent(6,numTotMuons);
   hElecMuonStats->GetXaxis()->SetBinLabel(7,"Muonsfinal");hElecMuonStats->SetBinContent(7,numTotMuonsfinal);
   hElecMuonStats->GetXaxis()->SetBinLabel(8,"MuonsMatched");hElecMuonStats->SetBinContent(8,numTotMuonsMatched);
   hElecMuonStats->GetXaxis()->SetBinLabel(9,"MCMuon");hElecMuonStats->SetBinContent(9,numTotMCMuon);
   hElecMuonStats->GetXaxis()->SetBinLabel(10,"MCMuonBigEta");hElecMuonStats->SetBinContent(10,numTotMCMuonBigEta);
                                                                                                                                        
                                                                                                                                        
   hPhotonJetStats->SetFillColor(kBlack);hPhotonJetStats->SetDrawOption("hbar");
   hPhotonJetStats->GetXaxis()->SetBinLabel(1,"Photons");hPhotonJetStats->SetBinContent(1,numTotPhotons);
   hPhotonJetStats->GetXaxis()->SetBinLabel(2,"Photonsfinal");hPhotonJetStats->SetBinContent(2,numTotPhotonsfinal);
   hPhotonJetStats->GetXaxis()->SetBinLabel(3,"PhotonsMatched");hPhotonJetStats->SetBinContent(3,numTotPhotonsMatched);
   hPhotonJetStats->GetXaxis()->SetBinLabel(4,"MCPhot");hPhotonJetStats->SetBinContent(4,numTotMCPhot);
   hPhotonJetStats->GetXaxis()->SetBinLabel(5,"MCPhotBigEta");hPhotonJetStats->SetBinContent(5,numTotMCPhotBigEta);
   hPhotonJetStats->GetXaxis()->SetBinLabel(6,"Jets");hPhotonJetStats->SetBinContent(6,numTotJets);
   hPhotonJetStats->GetXaxis()->SetBinLabel(7,"Jetsfinal");hPhotonJetStats->SetBinContent(7,numTotJetsfinal);
   hPhotonJetStats->GetXaxis()->SetBinLabel(8,"JetsMatched");hPhotonJetStats->SetBinContent(8,numTotJetsMatched);
   hPhotonJetStats->GetXaxis()->SetBinLabel(9,"MCJet");hPhotonJetStats->SetBinContent(9,numTotMCJet);
   hPhotonJetStats->GetXaxis()->SetBinLabel(10,"MCJetBigEta");hPhotonJetStats->SetBinContent(10,numTotMCJetBigEta);
                                                                                                                                        
                                                                                                                                        
                                                                                                                                        
   hMiscObjStats->SetFillColor(kGreen);hMiscObjStats->SetDrawOption("hbar");
   hMiscObjStats->GetXaxis()->SetBinLabel(1,"NotPrimaryTrk");hMiscObjStats->SetBinContent(1,numTotNotPrimaryTrk);
   hMiscObjStats->GetXaxis()->SetBinLabel(2,"NotClean");hMiscObjStats->SetBinContent(2,numTotNotClean);
   hMiscObjStats->GetXaxis()->SetBinLabel(3,"Duplicate");hMiscObjStats->SetBinContent(3,numTotDuplicate);
   hMiscObjStats->GetXaxis()->SetBinLabel(4,"ElecNotPrimaryTrk");hMiscObjStats->SetBinContent(4,numTotElecNotPrimaryTrk);
   hMiscObjStats->GetXaxis()->SetBinLabel(5,"ElecNotCleanHOE");hMiscObjStats->SetBinContent(5,numTotElecNotCleanHOE);
   hMiscObjStats->GetXaxis()->SetBinLabel(6,"ElecNotCleanShsh");hMiscObjStats->SetBinContent(6,numTotElecNotCleanShsh);
   hMiscObjStats->GetXaxis()->SetBinLabel(7,"ElecNotCleanTmat");hMiscObjStats->SetBinContent(7,numTotElecNotCleanTmat);
   hMiscObjStats->GetXaxis()->SetBinLabel(8,"ElecDupl");hMiscObjStats->SetBinContent(8,numTotElecDupl);
   hMiscObjStats->GetXaxis()->SetBinLabel(9,"ElecDuplBadHOE");hMiscObjStats->SetBinContent(9,numTotElecDuplBadHOE);
   hMiscObjStats->GetXaxis()->SetBinLabel(10,"ElecDuplBadShsh");hMiscObjStats->SetBinContent(10,numTotElecDuplBadShsh);
   hMiscObjStats->GetXaxis()->SetBinLabel(11,"ElecDuplBadTmat");hMiscObjStats->SetBinContent(11,numTotElecDuplBadTmat);
   hMiscObjStats->GetXaxis()->SetBinLabel(12,"ElectronsNonIso");hMiscObjStats->SetBinContent(12,numTotElectronsNonIso);
   hMiscObjStats->GetXaxis()->SetBinLabel(13,"ElectronsNonIsoBadHOE");hMiscObjStats->SetBinContent(13,numTotElectronsNonIsoBadHOE);
   hMiscObjStats->GetXaxis()->SetBinLabel(14,"ElectronsNonIsoBadShsh");hMiscObjStats->SetBinContent(14,numTotElectronsNonIsoBadShsh);
   hMiscObjStats->GetXaxis()->SetBinLabel(15,"ElectronsNonIsoBadTmat");hMiscObjStats->SetBinContent(15,numTotElectronsNonIsoBadTmat);
   hMiscObjStats->GetXaxis()->SetBinLabel(16,"ElectronsfinalBadHOE");hMiscObjStats->SetBinContent(16,numTotElectronsfinalBadHOE);
   hMiscObjStats->GetXaxis()->SetBinLabel(17,"ElectronsfinalBadShsh");hMiscObjStats->SetBinContent(17,numTotElectronsfinalBadShsh);
   hMiscObjStats->GetXaxis()->SetBinLabel(18,"ElectronsfinalBadTmat");hMiscObjStats->SetBinContent(18,numTotElectronsfinalBadTmat);
   hMiscObjStats->GetXaxis()->SetBinLabel(19,"ElectronsMatchedBadHOE");hMiscObjStats->SetBinContent(19,numTotElectronsMatchedBadHOE);
   hMiscObjStats->GetXaxis()->SetBinLabel(20,"ElectronsMatchedBadShsh");hMiscObjStats->SetBinContent(20,numTotElectronsMatchedBadShsh);
   hMiscObjStats->GetXaxis()->SetBinLabel(21,"ElectronsMatchedBadTmat");hMiscObjStats->SetBinContent(21,numTotElectronsMatchedBadTmat);
   hMiscObjStats->GetXaxis()->SetBinLabel(22,"MuonNotPrimaryTrk");hMiscObjStats->SetBinContent(22,numTotMuonNotPrimaryTrk);
   hMiscObjStats->GetXaxis()->SetBinLabel(23,"MuonNotClean");hMiscObjStats->SetBinContent(23,numTotMuonNotClean);
   hMiscObjStats->GetXaxis()->SetBinLabel(24,"MuonDupl");hMiscObjStats->SetBinContent(24,numTotMuonDupl);
   hMiscObjStats->GetXaxis()->SetBinLabel(25,"MuonDuplBad");hMiscObjStats->SetBinContent(25,numTotMuonDuplBad);
   hMiscObjStats->GetXaxis()->SetBinLabel(26,"MuonsNonIso");hMiscObjStats->SetBinContent(26,numTotMuonsNonIso);
   hMiscObjStats->GetXaxis()->SetBinLabel(27,"MuonsNonIsoBad");hMiscObjStats->SetBinContent(27,numTotMuonsNonIsoBad);
   hMiscObjStats->GetXaxis()->SetBinLabel(28,"MuonsfinalBad");hMiscObjStats->SetBinContent(28,numTotMuonsfinalBad);
   hMiscObjStats->GetXaxis()->SetBinLabel(29,"MuonsMatchedBad");hMiscObjStats->SetBinContent(29,numTotMuonsMatchedBad);
   hMiscObjStats->GetXaxis()->SetBinLabel(30,"Taus");hMiscObjStats->SetBinContent(30,numTotTaus);
   hMiscObjStats->GetXaxis()->SetBinLabel(31,"Tausfinal");hMiscObjStats->SetBinContent(31,numTotTausfinal);
   hMiscObjStats->GetXaxis()->SetBinLabel(32,"TausMatched");hMiscObjStats->SetBinContent(32,numTotTausMatched);
   hMiscObjStats->GetXaxis()->SetBinLabel(33,"MCTau");hMiscObjStats->SetBinContent(33,numTotMCTau);
   hMiscObjStats->GetXaxis()->SetBinLabel(34,"MCTauBigEta");hMiscObjStats->SetBinContent(34,numTotMCTauBigEta);
   hMiscObjStats->GetXaxis()->SetBinLabel(35,"TauNotPrimaryTrk");hMiscObjStats->SetBinContent(35,numTotTauNotPrimaryTrk);
   hMiscObjStats->GetXaxis()->SetBinLabel(36,"TauNotClean");hMiscObjStats->SetBinContent(36,numTotTauNotClean);
   hMiscObjStats->GetXaxis()->SetBinLabel(37,"TauDupl");hMiscObjStats->SetBinContent(37,numTotTauDupl);
   hMiscObjStats->GetXaxis()->SetBinLabel(38,"TauDuplBad");hMiscObjStats->SetBinContent(38,numTotTauDuplBad);
   hMiscObjStats->GetXaxis()->SetBinLabel(39,"TausNonIso");hMiscObjStats->SetBinContent(39,numTotTausNonIso);
   hMiscObjStats->GetXaxis()->SetBinLabel(40,"TausNonIsoBad");hMiscObjStats->SetBinContent(40,numTotTausNonIsoBad);
   hMiscObjStats->GetXaxis()->SetBinLabel(41,"TausfinalBad");hMiscObjStats->SetBinContent(41,numTotTausfinalBad);
   hMiscObjStats->GetXaxis()->SetBinLabel(42,"TausMatchedBad");hMiscObjStats->SetBinContent(42,numTotTausMatchedBad);
   hMiscObjStats->GetXaxis()->SetBinLabel(43,"PhotNotPrimaryTrk");hMiscObjStats->SetBinContent(43,numTotPhotNotPrimaryTrk);
   hMiscObjStats->GetXaxis()->SetBinLabel(44,"PhotNotCleanHOE");hMiscObjStats->SetBinContent(44,numTotPhotNotCleanHOE);
   hMiscObjStats->GetXaxis()->SetBinLabel(45,"PhotNotCleanShsh");hMiscObjStats->SetBinContent(45,numTotPhotNotCleanShsh);
   hMiscObjStats->GetXaxis()->SetBinLabel(46,"PhotDupl");hMiscObjStats->SetBinContent(46,numTotPhotDupl);
   hMiscObjStats->GetXaxis()->SetBinLabel(47,"PhotDuplBadHOE");hMiscObjStats->SetBinContent(47,numTotPhotDuplBadHOE);
   hMiscObjStats->GetXaxis()->SetBinLabel(48,"PhotDuplBadShsh");hMiscObjStats->SetBinContent(48,numTotPhotDuplBadShsh);
   hMiscObjStats->GetXaxis()->SetBinLabel(49,"PhotonsNonIso");hMiscObjStats->SetBinContent(49,numTotPhotonsNonIso);
   hMiscObjStats->GetXaxis()->SetBinLabel(50,"PhotonsNonIsoBadHOE");hMiscObjStats->SetBinContent(50,numTotPhotonsNonIsoBadHOE);
   hMiscObjStats->GetXaxis()->SetBinLabel(51,"PhotonsNonIsoBadShsh");hMiscObjStats->SetBinContent(51,numTotPhotonsNonIsoBadShsh);
   hMiscObjStats->GetXaxis()->SetBinLabel(52,"PhotonsfinalBadHOE");hMiscObjStats->SetBinContent(52,numTotPhotonsfinalBadHOE);
   hMiscObjStats->GetXaxis()->SetBinLabel(53,"PhotonsfinalBadShsh");hMiscObjStats->SetBinContent(53,numTotPhotonsfinalBadShsh);
   hMiscObjStats->GetXaxis()->SetBinLabel(54,"PhotonsMatchedBadHOE");hMiscObjStats->SetBinContent(54,numTotPhotonsMatchedBadHOE);
   hMiscObjStats->GetXaxis()->SetBinLabel(55,"PhotonsMatchedBadShsh");hMiscObjStats->SetBinContent(55,numTotPhotonsMatchedBadShsh);
   hMiscObjStats->GetXaxis()->SetBinLabel(56,"JetNotPrimaryTrk");hMiscObjStats->SetBinContent(56,numTotJetNotPrimaryTrk);
   hMiscObjStats->GetXaxis()->SetBinLabel(57,"JetNotCleanFem");hMiscObjStats->SetBinContent(57,numTotJetNotCleanFem);
   hMiscObjStats->GetXaxis()->SetBinLabel(58,"JetNotCleanFtk");hMiscObjStats->SetBinContent(58,numTotJetNotCleanFtk);
   hMiscObjStats->GetXaxis()->SetBinLabel(59,"JetDupl");hMiscObjStats->SetBinContent(59,numTotJetDupl);
   hMiscObjStats->GetXaxis()->SetBinLabel(60,"JetDuplBadFem");hMiscObjStats->SetBinContent(60,numTotJetDuplBadFem);
   hMiscObjStats->GetXaxis()->SetBinLabel(61,"JetDuplBadFtk");hMiscObjStats->SetBinContent(61,numTotJetDuplBadFtk);
   hMiscObjStats->GetXaxis()->SetBinLabel(62,"BJets");hMiscObjStats->SetBinContent(62,numTotBJets);
   hMiscObjStats->GetXaxis()->SetBinLabel(63,"JetsfinalBadFem");hMiscObjStats->SetBinContent(63,numTotJetsfinalBadFem);
   hMiscObjStats->GetXaxis()->SetBinLabel(64,"JetsfinalBadFtk");hMiscObjStats->SetBinContent(64,numTotJetsfinalBadFtk);
   hMiscObjStats->GetXaxis()->SetBinLabel(65,"BJetsfinal");hMiscObjStats->SetBinContent(65,numTotBJetsfinal);
   hMiscObjStats->GetXaxis()->SetBinLabel(66,"JetsMatchedBadFem");hMiscObjStats->SetBinContent(66,numTotJetsMatchedBadFem);
   hMiscObjStats->GetXaxis()->SetBinLabel(67,"JetsMatchedBadFtk");hMiscObjStats->SetBinContent(67,numTotJetsMatchedBadFtk);
   hMiscObjStats->GetXaxis()->SetBinLabel(68,"Ufos");hMiscObjStats->SetBinContent(68,numTotUfos);
   hMiscObjStats->GetXaxis()->SetBinLabel(69,"Ufosfinal");hMiscObjStats->SetBinContent(69,numTotUfosfinal);
   hMiscObjStats->GetXaxis()->SetBinLabel(70,"UfosMatched");hMiscObjStats->SetBinContent(70,numTotUfosMatched);
   hMiscObjStats->GetXaxis()->SetBinLabel(71,"UfosNotPrimaryTrk");hMiscObjStats->SetBinContent(71,numTotUfosNotPrimaryTrk);
   hMiscObjStats->GetXaxis()->SetBinLabel(72,"UfosNotClean");hMiscObjStats->SetBinContent(72,numTotUfosNotClean);
   hMiscObjStats->GetXaxis()->SetBinLabel(73,"UfosDupl");hMiscObjStats->SetBinContent(73,numTotUfosDupl);
   hMiscObjStats->GetXaxis()->SetBinLabel(74,"UfosDuplBad");hMiscObjStats->SetBinContent(74,numTotUfosDuplBad);
   hMiscObjStats->GetXaxis()->SetBinLabel(75,"UfosNonIso");hMiscObjStats->SetBinContent(75,numTotUfosNonIso);
   hMiscObjStats->GetXaxis()->SetBinLabel(76,"UfosNonIsoBad");hMiscObjStats->SetBinContent(76,numTotUfosNonIsoBad);
   hMiscObjStats->GetXaxis()->SetBinLabel(77,"UfosfinalBad");hMiscObjStats->SetBinContent(77,numTotUfosfinalBad);
   hMiscObjStats->GetXaxis()->SetBinLabel(78,"UfosMatchedBad");hMiscObjStats->SetBinContent(78,numTotUfosMatchedBad);
                                                                                                                                        
                                                                                                                                        
}


//------------------------------------------------------------------------------

bool SusyAnalyzer::AcceptTrigger()
{
  // Verifies that the event triggered
  // Still to be done ****

 return true;

}

//------------------------------------------------------------------------------

void SusyAnalyzer::CleanMemory()
{
// Clean the memory at end of the event

 if (DEBUGLVL >= 1){
   cout << "Cleaning memory " << endl;
 }

 if (!MCData.empty() ){
  for (unsigned int i = 0; i < MCData.size(); i++){  
    delete MCData[i];
  }
  MCData.clear();
 } 
 if (!GenData.empty() ){
  for (unsigned int i = 0; i < GenData.size(); i++){  
    delete GenData[i];
  }
  GenData.clear();
 }
 if (!RecoData.empty() ){
  for (unsigned int i = 0; i < RecoData.size(); i++){
    delete RecoData[i];
  }  
  RecoData.clear();
 }
 
 delete EventData;


 return;

}

