//
// $Id: PATMETProducer.h,v 1.7 2009/03/26 20:44:37 vadler Exp $
//

#ifndef PhysicsTools_PatAlgos_PATMETProducer_h
#define PhysicsTools_PatAlgos_PATMETProducer_h

/**
  \class    pat::PATMETProducer PATMETProducer.h "PhysicsTools/PatAlgos/interface/PATMETProducer.h"
  \brief    Produces the pat::MET

   The PATMETProducer produces the analysis-level pat::MET starting from
   a collection of objects of METType.

  \author   Steven Lowette
  \version  $Id: PATMETProducer.h,v 1.7 2009/03/26 20:44:37 vadler Exp $
*/


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/Utils/interface/EtComparator.h"

#include "DataFormats/PatCandidates/interface/MET.h"
#include "PhysicsTools/PatAlgos/interface/EfficiencyLoader.h"


#include "DataFormats/PatCandidates/interface/UserData.h"
#include "PhysicsTools/PatAlgos/interface/PATUserDataHelper.h"


namespace pat {

  class PATMETProducer : public edm::EDProducer {

    public:

      explicit PATMETProducer(const edm::ParameterSet & iConfig);
      ~PATMETProducer();

      virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

    private:

      // configurables
      edm::InputTag metSrc_;
      bool          addGenMET_;
      edm::InputTag genMETSrc_;
      bool          addResolutions_;
      bool          addMuonCorr_;
      edm::InputTag muonSrc_;
      // tools
      GreaterByEt<MET> eTComparator_;

      bool addEfficiencies_;
      pat::helper::EfficiencyLoader efficiencyLoader_;

      bool useUserData_;
      pat::PATUserDataHelper<pat::MET>      userDataHelper_;


  };


}

#endif
