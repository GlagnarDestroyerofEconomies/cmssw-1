#ifndef TauAnalysis_CandidateTools_CompositePtrCandidateT1T2MEtProducer_h
#define TauAnalysis_CandidateTools_CompositePtrCandidateT1T2MEtProducer_h

/** \class CompositePtrCandidateT1T2MEtProducer
 *
 * Produce combinations of leptonic and hadronic decay products 
 * of a pair of tau leptons plus missing transverse momentum 
 * (representing the undetected momentum carried away by the neutrinos 
 *  produced in the two tau decays) 
 * 
 * \authors Colin Bernet,
 *          Michal Bluj,
 *          Christian Veelken
 *
 * \version $Revision: 1.7 $
 *
 * $Id: CompositePtrCandidateT1T2MEtProducer.h,v 1.7 2010/03/16 23:57:30 friis Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/View.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/CandidateTools/interface/FetchCollection.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"

#include "TauAnalysis/CandidateTools/interface/CompositePtrCandidateT1T2MEtAlgorithm.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include <string>

template<typename T1, typename T2>
class CompositePtrCandidateT1T2MEtProducer : public edm::EDProducer 
{
  typedef edm::Ptr<T1> T1Ptr;
  typedef edm::Ptr<T2> T2Ptr;

  typedef std::vector<CompositePtrCandidateT1T2MEt<T1,T2> > CompositePtrCandidateCollection;
  
 public:

  explicit CompositePtrCandidateT1T2MEtProducer(const edm::ParameterSet& cfg)
    : algorithm_(cfg), 
      doSVreco_(false), 
      cfgError_(0)
  {
    useLeadingTausOnly_ = cfg.getParameter<bool>("useLeadingTausOnly");
    srcLeg1_ = cfg.getParameter<edm::InputTag>("srcLeg1");
    srcLeg2_ = cfg.getParameter<edm::InputTag>("srcLeg2");
    dRmin12_ = cfg.getParameter<double>("dRmin12");
    srcMET_ = ( cfg.exists("srcMET") ) ? cfg.getParameter<edm::InputTag>("srcMET") : edm::InputTag();
    srcGenParticles_ = ( cfg.exists("srcGenParticles") ) ? cfg.getParameter<edm::InputTag>("srcGenParticles") : edm::InputTag();
    srcPV_ = ( cfg.exists("srcPrimaryVertex") ) ? cfg.getParameter<edm::InputTag>("srcPrimaryVertex") : edm::InputTag();
    srcBeamSpot_ = ( cfg.exists("srcBeamSpot") ) ? cfg.getParameter<edm::InputTag>("srcBeamSpot") : edm::InputTag();
    recoMode_ = cfg.getParameter<std::string>("recoMode");
    verbosity_ = cfg.getUntrackedParameter<int>("verbosity", 0);

//--- check that InputTag for MET collection has been defined,
//    in case it is needed for the reconstruction mode 
//    specified in the configuration parameter set
    if ( srcMET_.label() == "" && recoMode_ != "" ) {
      edm::LogError ("ConfigError") 
	<< " Configuration Parameter srcMET undefined," 
	<< " needed for recoMode = " << recoMode_ << " !!";
      cfgError_ = 1;
    }

    if ( srcMET_.label() != "" && srcBeamSpot_.label() != "" && srcPV_.label() != "" &&
	 TauVertex::typeIsSupportedBySVFitter<T1>() &&
	 TauVertex::typeIsSupportedBySVFitter<T2>() ) {
      // CV: disable "secondaryVertexFit" reconstruction mode for now
      //doSVreco_ = true;
    } else if ( recoMode_ == "secondaryVertexFit" ) {
      if ( !(srcMET_.label() == "" && srcBeamSpot_.label() != "" && srcPV_.label() != "") ) {
	edm::LogError ("ConfigError") 
	  << " One or more of configuration parameters srcMET(" << srcMET_.label() << "),"
	  << " srcBeamSpot(" << srcBeamSpot_.label() << ") , srcPrimaryVertex(" << srcPV_ << ") are undefined," 
	  << " and needed for recoMode = " << recoMode_ << " !!";
      }

      if ( !(TauVertex::typeIsSupportedBySVFitter<T1>() && TauVertex::typeIsSupportedBySVFitter<T2>()) ) {
	edm::LogError ("ConfigError") 
	  << " The input collection type (probably Candidate) is not supported" 
	  << " when using the secondaryVertexFit option !!";
      }

      cfgError_ = 1;
    }
    
    produces<CompositePtrCandidateCollection>("");
  }

  ~CompositePtrCandidateT1T2MEtProducer() {}

  void produce(edm::Event& evt, const edm::EventSetup& es)
  {
//--- print-out an error message and add an empty collection to the event 
//    in case of erroneous configuration parameters
    if ( cfgError_ ) {
      edm::LogError ("produce") 
	<< " Error in Configuration ParameterSet" 
	<< " --> CompositePtrCandidateT1T2MEt collection will NOT be produced !!";
      std::auto_ptr<CompositePtrCandidateCollection> emptyCompositePtrCandidateCollection(new CompositePtrCandidateCollection());
      evt.put(emptyCompositePtrCandidateCollection);
      return;
    }

    typedef edm::View<T1> T1View;
    edm::Handle<T1View> leg1Collection;
    pf::fetchCollection(leg1Collection, srcLeg1_, evt);
    typedef edm::View<T2> T2View;
    edm::Handle<T2View> leg2Collection;
    pf::fetchCollection(leg2Collection, srcLeg2_, evt);

    reco::CandidatePtr metPtr;
    if ( srcMET_.label() != "" ) {
      edm::Handle<reco::CandidateView> metCollection;
      pf::fetchCollection(metCollection, srcMET_, evt);
      
//--- check that there is exactly one MET object in the event
//    (missing transverse momentum is an **event level** quantity)
      if ( metCollection->size() == 1 ) {
	metPtr = metCollection->ptrAt(0);
      } else {
	edm::LogError ("produce") 
	  << " Found " << metCollection->size() << " MET objects in collection = " << srcMET_ << ","
	  << " --> CompositePtrCandidateT1T2MEt collection will NOT be produced !!";
	std::auto_ptr<CompositePtrCandidateCollection> emptyCompositePtrCandidateCollection(new CompositePtrCandidateCollection());
	evt.put(emptyCompositePtrCandidateCollection);
	return;
      }
    } 

    const reco::GenParticleCollection* genParticles = 0;
    if ( srcGenParticles_.label() != "" ) {
      edm::Handle<reco::GenParticleCollection> genParticleCollection;
      pf::fetchCollection(genParticleCollection, srcGenParticles_, evt);
      genParticles = genParticleCollection.product();
    }

    // Get primary vertex
    const reco::Vertex* pv = NULL;
    if ( srcPV_.label() != "" ) {
       edm::Handle<reco::VertexCollection> pvs;
       pf::fetchCollection(pvs, srcPV_, evt);
       pv = &((*pvs)[0]);
    }

    // Get beamspot
    const reco::BeamSpot* beamSpot = NULL;
    if ( srcBeamSpot_.label() != "" ) {
       edm::Handle<reco::BeamSpot> beamSpotHandle;
       pf::fetchCollection(beamSpotHandle, srcBeamSpot_, evt);
       beamSpot = beamSpotHandle.product();
    }

    const TransientTrackBuilder* trackBuilder = NULL;
    if ( doSVreco_ ) {
       edm::ESHandle<TransientTrackBuilder> myTransientTrackBuilder;
       es.get<TransientTrackRecord>().get("TransientTrackBuilder", myTransientTrackBuilder);
       trackBuilder = myTransientTrackBuilder.product();
       if ( !trackBuilder ) {
	 edm::LogError ("produce") << " Failed to access TransientTrackBuilder !!";
       }
    }

//--- check if only one combination of tau decay products 
//    (the combination of highest Pt object in leg1 collection + highest Pt object in leg2 collection)
//    shall be produced, or all possible combinations of leg1 and leg2 objects
    std::auto_ptr<CompositePtrCandidateCollection> compositePtrCandidateCollection(new CompositePtrCandidateCollection());
    if ( useLeadingTausOnly_ ) {

//--- find highest Pt particles in leg1 and leg2 collections
      int idxLeadingLeg1 = -1;
      double leg1PtMax = 0.;
      for ( unsigned idxLeg1 = 0, numLeg1 = leg1Collection->size(); 
	    idxLeg1 < numLeg1; ++idxLeg1 ) {
	T1Ptr leg1Ptr = leg1Collection->ptrAt(idxLeg1);
	if ( idxLeadingLeg1 == -1 || leg1Ptr->pt() > leg1PtMax ) {
	  idxLeadingLeg1 = idxLeg1;
	leg1PtMax = leg1Ptr->pt();
	}
      }
      
      int idxLeadingLeg2 = -1;
      double leg2PtMax = 0.;
      for ( unsigned idxLeg2 = 0, numLeg2 = leg2Collection->size(); 
	    idxLeg2 < numLeg2; ++idxLeg2 ) {
	T2Ptr leg2Ptr = leg2Collection->ptrAt(idxLeg2);

//--- do not create CompositePtrCandidateT1T2MEt object 
//    for combination of particle with itself
	if ( idxLeadingLeg1 != -1 ) {
	  T1Ptr leadingLeg1Ptr = leg1Collection->ptrAt(idxLeadingLeg1);
	  double dR = reco::deltaR(leadingLeg1Ptr->p4(), leg2Ptr->p4());
	  if ( dR < dRmin12_ ) continue;
	}
	
	if ( idxLeadingLeg2 == -1 || leg2Ptr->pt() > leg2PtMax ) {
	  idxLeadingLeg2 = idxLeg2;
	  leg2PtMax = leg2Ptr->pt();
	}
      }
      
      if ( idxLeadingLeg1 != -1 &&
	   idxLeadingLeg2 != -1 ) {
	T1Ptr leadingLeg1Ptr = leg1Collection->ptrAt(idxLeadingLeg1);
	T2Ptr leadingLeg2Ptr = leg2Collection->ptrAt(idxLeadingLeg2);
	
	CompositePtrCandidateT1T2MEt<T1,T2> compositePtrCandidate = 
	  algorithm_.buildCompositePtrCandidate(leadingLeg1Ptr, leadingLeg2Ptr, metPtr, genParticles, 
						pv, beamSpot, trackBuilder, doSVreco_);
	compositePtrCandidateCollection->push_back(compositePtrCandidate);
      } else {
	if ( verbosity_ >= 1 ) {
	  edm::LogInfo ("produce") 
	    << " Found no combination of particles in Collections" 
	    << " leg1 = " << srcLeg1_ << " and leg2 = " << srcLeg2_ << ".";
	}
      }
    } else {
//--- check if the same collection is used on both legs;
//    if so, skip diTau(j,i), j > i combination in order to avoid two diTau objects being produced
//    for combinations (i,j) and (j,i) of the same pair of particles in leg1 and leg2 collections
      bool sameCollection = (leg1Collection.id () == leg2Collection.id());
   
      for ( unsigned idxLeg1 = 0, numLeg1 = leg1Collection->size(); 
	    idxLeg1 < numLeg1; ++idxLeg1 ) {
	T1Ptr leg1Ptr = leg1Collection->ptrAt(idxLeg1);
	
	unsigned idxLeg2_first = ( sameCollection ) ? (idxLeg1 + 1) : 0;
	for ( unsigned idxLeg2 = idxLeg2_first, numLeg2 = leg2Collection->size(); 
	      idxLeg2 < numLeg2; ++idxLeg2 ) {
	  T2Ptr leg2Ptr = leg2Collection->ptrAt(idxLeg2);

//--- do not create CompositePtrCandidateT1T2MEt object 
//    for combination of particle with itself
	  double dR = reco::deltaR(leg1Ptr->p4(), leg2Ptr->p4());
	  if ( dR < dRmin12_ ) continue;
	  
	  CompositePtrCandidateT1T2MEt<T1,T2> compositePtrCandidate = 
	    algorithm_.buildCompositePtrCandidate(leg1Ptr, leg2Ptr, metPtr, genParticles, 
						  pv, beamSpot, trackBuilder, doSVreco_);
	  compositePtrCandidateCollection->push_back(compositePtrCandidate);
	}
      }
    }

//--- add the collection of reconstructed CompositePtrCandidateT1T2MEts to the event
    evt.put(compositePtrCandidateCollection);
  }

 private:

  CompositePtrCandidateT1T2MEtAlgorithm<T1,T2> algorithm_;
  
  bool useLeadingTausOnly_;
  edm::InputTag srcLeg1_;
  edm::InputTag srcLeg2_;
  double dRmin12_;
  edm::InputTag srcMET_;
  edm::InputTag srcGenParticles_;
  edm::InputTag srcPV_;
  edm::InputTag srcBeamSpot_;
  std::string recoMode_;
  bool doSVreco_;
  int verbosity_;

  int cfgError_;
};

#endif

