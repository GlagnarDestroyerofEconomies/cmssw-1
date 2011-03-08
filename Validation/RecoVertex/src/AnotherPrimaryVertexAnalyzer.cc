// -*- C++ -*-
//
// Package:    Validation/RecoVertex
// Class:      AnotherPrimaryVertexAnalyzer
// 
/**\class AnotherPrimaryVertexAnalyzer AnotherPrimaryVertexAnalyzer.cc Validation/RecoVertex/plugins/AnotherPrimaryVertexAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Andrea Venturi
//         Created:  Mon Oct 27 17:37:53 CET 2008
// $Id: AnotherPrimaryVertexAnalyzer.cc,v 1.4 2010/11/30 09:07:06 venturia Exp $
//
//


// system include files
#include <memory>

// user include files

#include <vector>
#include <map>
#include <limits>
#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/InputTag.h"

#include "Validation/RecoVertex/interface/VertexHistogramMaker.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "CommonTools/TriggerUtils/interface/PrescaleWeightProvider.h"


//
// class decleration
//

class AnotherPrimaryVertexAnalyzer : public edm::EDAnalyzer {
   public:
      explicit AnotherPrimaryVertexAnalyzer(const edm::ParameterSet&);
      ~AnotherPrimaryVertexAnalyzer();


private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);
  virtual void endRun(const edm::Run&, const edm::EventSetup&);
  virtual void endJob() ;

      // ----------member data ---------------------------

  VertexHistogramMaker _vhm;
  edm::InputTag _pvcollection;
  bool _firstOnly;

  PrescaleWeightProvider* _weightprov;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
AnotherPrimaryVertexAnalyzer::AnotherPrimaryVertexAnalyzer(const edm::ParameterSet& iConfig):
  _vhm(iConfig.getParameter<edm::ParameterSet>("vHistogramMakerPSet")),
  _pvcollection(iConfig.getParameter<edm::InputTag>("pvCollection")),
  _firstOnly(iConfig.getUntrackedParameter<bool>("firstOnly",false)),
  _weightprov(iConfig.getParameter<bool>("usePrescaleWeight") ? 
	      new PrescaleWeightProvider(iConfig.getParameter<edm::ParameterSet>("prescaleWeightProviderPSet")) : 0)
{
   //now do what ever initialization is needed

  //

  _vhm.book();

}


AnotherPrimaryVertexAnalyzer::~AnotherPrimaryVertexAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

  delete _weightprov;

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
AnotherPrimaryVertexAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  
  // compute event weigth

  double weight = 1.;

  if(_weightprov) weight = _weightprov->prescaleWeight(iEvent,iSetup);

  // get PV

  Handle<reco::VertexCollection> pvcoll;
  iEvent.getByLabel(_pvcollection,pvcoll);

  if(_firstOnly) {
    reco::VertexCollection firstpv;
    if(pvcoll->size()) firstpv.push_back((*pvcoll)[0]);
    _vhm.fill(iEvent,firstpv,weight);
  }
  else {
    _vhm.fill(iEvent,*pvcoll,weight);
  }
}


// ------------ method called once each job just before starting event loop  ------------
void 
AnotherPrimaryVertexAnalyzer::beginJob()
{ }

void
AnotherPrimaryVertexAnalyzer::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {

  _vhm.beginRun(iRun.run());

  if(_weightprov) _weightprov->initRun(iRun,iSetup);

}

void
AnotherPrimaryVertexAnalyzer::endRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {

}
// ------------ method called once each job just after ending the event loop  ------------
void 
AnotherPrimaryVertexAnalyzer::endJob() {
}


//define this as a plug-in
DEFINE_FWK_MODULE(AnotherPrimaryVertexAnalyzer);
