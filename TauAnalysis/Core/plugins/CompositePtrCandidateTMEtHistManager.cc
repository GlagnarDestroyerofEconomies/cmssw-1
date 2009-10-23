#include "TauAnalysis/Core/plugins/CompositePtrCandidateTMEtHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEtFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

const double epsilon = 0.01;

template<typename T>
bool matchesGenCandidate(const CompositePtrCandidateTMEt<T>& compositePtrCandidate)
{
  bool isGenMatched = false;
// not implemented yet...
  return isGenMatched;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

template<typename T>
CompositePtrCandidateTMEtHistManager<T>::CompositePtrCandidateTMEtHistManager(const edm::ParameterSet& cfg)
  : dqmError_(0)
{
  //std::cout << "<CompositePtrCandidateTMEtHistManager::CompositePtrCandidateTMEtHistManager>:" << std::endl;

  tauNuCandidateSrc_ = cfg.getParameter<edm::InputTag>("tauNuCandidateSource");
  //std::cout << " tauNuCandidateSrc = " << tauNuCandidateSrc_ << std::endl;

  tauJetWeightExtractors_ = getTauJetWeightExtractors<T>(cfg, "tauJetWeightSource");

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;

  requireGenMatch_ = cfg.getParameter<bool>("requireGenMatch");
  //std::cout << " requireGenMatch = " << requireGenMatch_ << std::endl;

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "tauNuCandidates");
}

template<typename T>
CompositePtrCandidateTMEtHistManager<T>::~CompositePtrCandidateTMEtHistManager()
{
  for ( typename std::vector<FakeRateJetWeightExtractor<T>*>::iterator it = tauJetWeightExtractors_.begin();
	it != tauJetWeightExtractors_.end(); ++it ) {
    delete (*it);
  }
}

template<typename T>
void CompositePtrCandidateTMEtHistManager<T>::bookHistograms()
{
  //std::cout << "<CompositePtrCandidateTMEtHistManager::bookHistograms>:" << std::endl;

  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("bookHistograms") << " Failed to access dqmStore --> histograms will NOT be booked !!";
    dqmError_ = 1;
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
  dqmStore.setCurrentFolder(dqmDirectory_store_);
  
  hNuTauCandidatePt_ = dqmStore.book1D("NuTauCandidatePt", "Composite P_{T}", 75, 0., 150.);
  hNuTauCandidatePhi_ = dqmStore.book1D("NuTauCandidatePhi", "Composite #phi", 36, -TMath::Pi(), +TMath::Pi());

  hNuTauCandidateDPhi_ = dqmStore.book1D("NuTauCandidateDPhi", "#Delta#phi_{T,MET}", 36, -epsilon, TMath::Pi() + epsilon);
  hNuTauCandidateMt_ = dqmStore.book1D("NuTauCandidateMt", "Mass_{T,MET}", 40, 0., 200.);
  
  hMEtVsTauPt_ = dqmStore.book2D("MEtVsTauPt", "P_{T}^{T} - MET", 20, 0., 100., 20, 0., 100.);
}

template<typename T>
double CompositePtrCandidateTMEtHistManager<T>::getCandidateWeight(const CompositePtrCandidateTMEt<T>& tauNuCandidate)
{
  return getTauJetWeight<T>(*tauNuCandidate.visDecayProducts(), tauJetWeightExtractors_);
}

template<typename T>
void CompositePtrCandidateTMEtHistManager<T>::fillHistograms(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<CompositePtrCandidateTMEtHistManager::fillHistograms>:" << std::endl; 

  if ( dqmError_ ) {
    edm::LogError ("fillHistograms") << " Failed to access dqmStore --> histograms will NOT be filled !!";
    return;
  }

  typedef std::vector<CompositePtrCandidateTMEt<T> > CompositePtrCandidateCollection;
  edm::Handle<CompositePtrCandidateCollection> tauNuCandidates;
  evt.getByLabel(tauNuCandidateSrc_, tauNuCandidates);

  //std::cout << " tauNuCandidates.size = " << tauNuCandidates->size() << std::endl;

  double tauJetWeightSum = 0.;
  for ( typename CompositePtrCandidateCollection::const_iterator tauNuCandidate = tauNuCandidates->begin();
	tauNuCandidate != tauNuCandidates->end(); ++tauNuCandidate ) {
    if ( requireGenMatch_ && !matchesGenCandidate(*tauNuCandidate) ) continue;

    tauJetWeightSum += getCandidateWeight(*tauNuCandidate);
  }

  for ( typename CompositePtrCandidateCollection::const_iterator tauNuCandidate = tauNuCandidates->begin();
	tauNuCandidate != tauNuCandidates->end(); ++tauNuCandidate ) {
    
    //bool isGenMatched = matchesGenCandidate(*tauNuCandidate);
    //std::cout << " isGenMatched = " << isGenMatched << std::endl;

    if ( requireGenMatch_ && !matchesGenCandidate(*tauNuCandidate) ) continue;

    double tauJetWeight = getCandidateWeight(*tauNuCandidate);
    double weight = getWeight(evtWeight, tauJetWeight, tauJetWeightSum);
    
    hNuTauCandidatePt_->Fill(tauNuCandidate->pt(), weight);
    hNuTauCandidatePhi_->Fill(tauNuCandidate->phi(), weight);
    
    hNuTauCandidateDPhi_->Fill(tauNuCandidate->dPhi(), weight);
    hNuTauCandidateMt_->Fill(tauNuCandidate->mt(), weight);
    
    hMEtVsTauPt_->Fill(tauNuCandidate->visDecayProducts()->pt(), tauNuCandidate->met()->pt(), weight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/Tau.h"

typedef CompositePtrCandidateTMEtHistManager<pat::Tau> PATTauNuHistManager;

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATTauNuHistManager, "PATTauNuHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATTauNuHistManager, "PATTauNuHistManager");
  
#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<PATTauNuHistManager> PATTauNuAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(PATTauNuAnalyzer);


