#ifndef JetMETCorrections_Type1MET_JetCorrExtractorT_h
#define JetMETCorrections_Type1MET_JetCorrExtractorT_h

/** \class JetCorrExtractorT
 *
 * Retrieve jet energy correction factor for
 *  o reco::CaloJets
 *  o reco::PFJets
 *  o pat::Jets (of either PF-type or Calo-type)
 *
 * NOTE: this "general" implementation is to be used for reco::CaloJets and reco::PFJets, **not** for pat::Jets
 *
 * \author Christian Veelken, LLR
 *
 * \version $Revision: 1.2 $
 *
 * $Id: JetCorrExtractorT.h,v 1.2 2011/09/30 08:11:33 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Candidate/interface/Candidate.h"

namespace
{
  template <typename T>
  double getCorrection(const T& rawJet, const std::string& jetCorrLabel, 
		       const edm::Event& evt, const edm::EventSetup& es, 
		       const edm::RefToBase<reco::Jet>& rawJetRef)
  {
    const JetCorrector* jetCorrector = JetCorrector::getJetCorrector(jetCorrLabel, es);
    if ( !jetCorrector )  
      throw cms::Exception("JetCorrExtractor")
	<< "Failed to access Jet corrections for = " << jetCorrLabel << " !!\n";
    //return jetCorrector->correction(rawJet, rawJetRef, evt, es);
    return jetCorrector->correction(rawJet, evt, es);
  }

  double sign(double x)
  {
    if      ( x > 0. ) return +1.;
    else if ( x < 0. ) return -1.;
    else               return  0.;
  }
}

template <typename T>
class JetCorrExtractorT
{
 public:

  reco::Candidate::LorentzVector operator()(const T& rawJet, const std::string& jetCorrLabel, 
					    const edm::Event* evt = 0, const edm::EventSetup* es = 0, 
					    const edm::RefToBase<reco::Jet>* rawJetRef = 0, 
					    double jetCorrEtaMax = 9.9, 
					    const reco::Candidate::LorentzVector* rawJetP4_specified = 0)
  {
    // "general" implementation requires access to edm::Event and edm::EventSetup,
    // only specialization for pat::Jets doesn't
    assert(evt && es && rawJetRef);

    // allow to specify four-vector to be used as "raw" (uncorrected) jet momentum,
    // call 'rawJet.p4()' in case four-vector not specified explicitely
    reco::Candidate::LorentzVector rawJetP4 = ( rawJetP4_specified ) ?
      (*rawJetP4_specified) : rawJet.p4();

    // restrict computation of JEC factors to region |eta| < 4.7,
    // to work around problem with CMSSW_4_2_x JEC factors at high eta,
    // reported in
    //  https://hypernews.cern.ch/HyperNews/CMS/get/jes/270.html
    //  https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/1259/1.html
    double jetCorrFactor = 1.;
    if ( fabs(rawJetP4.eta()) < jetCorrEtaMax ) {      
      jetCorrFactor = getCorrection(rawJet, jetCorrLabel, *evt, *es, *rawJetRef);
    } else {
      reco::Candidate::PolarLorentzVector modJetPolarP4(rawJetP4);
      modJetPolarP4.SetEta(sign(rawJetP4.eta())*jetCorrEtaMax);
      
      reco::Candidate::LorentzVector modJetP4(modJetPolarP4);
      
      T modJet(rawJet);
      modJet.setP4(modJetP4);
      
      jetCorrFactor = getCorrection(modJet, jetCorrLabel, *evt, *es, *rawJetRef);
    }

    reco::Candidate::LorentzVector corrJetP4 = rawJetP4;
    corrJetP4 *= jetCorrFactor;

    return corrJetP4;
  }
};

#endif
