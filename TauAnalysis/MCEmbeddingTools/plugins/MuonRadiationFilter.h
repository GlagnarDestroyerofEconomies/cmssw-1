#ifndef TauAnalysis_MCEmbeddingTools_MuonRadiationFilter_h
#define TauAnalysis_MCEmbeddingTools_MuonRadiationFilter_h

/** \class MuonRadiationFilter
 *
 * Veto events in which a muon from Z --> mu+ mu- decay radiates a photon:
 *  muon -> muon + photon
 * 
 * \author Christian Veelken, LLR
 *        (based on python code developed by Mike Bachtis)
 *
 * \version $Revision: 1.8 $
 *
 * $Id: MuonRadiationFilter.h,v 1.8 2012/02/13 17:33:04 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

class MuonRadiationFilter : public edm::EDFilter 
{
 public:
  explicit MuonRadiationFilter(const edm::ParameterSet&);
  ~MuonRadiationFilter() {}

 private:
  bool filter(edm::Event&, const edm::EventSetup&);

  void compPFIso(const reco::Candidate::LorentzVector&, const reco::PFCandidateCollection&, double&, double&, double&);

  edm::InputTag srcSelectedMuons_;
  edm::InputTag srcPFCandsNoPU_;
  edm::InputTag srcPFCandsPU_;

  double minPtLow_;
  double dRlowPt_;

  double minPtHigh_;
  double dRhighPt_;

  double dRvetoCone_;
  double dRisoCone_;
  double maxRelIso_;

  double maxMass_;
};

#endif
