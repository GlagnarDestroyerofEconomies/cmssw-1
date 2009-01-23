#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class DimuonMCMatcher : public edm::EDProducer {
public:
  DimuonMCMatcher(const edm::ParameterSet & cfg);
  virtual void produce(edm::Event&, const edm::EventSetup&);
private:
  edm::InputTag src_;
};

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/GenericParticle.h"
using namespace std;
using namespace reco;
using namespace edm;

DimuonMCMatcher::DimuonMCMatcher(const edm::ParameterSet & cfg) : 
  src_(cfg.getParameter<InputTag>("src")) {
  produces<vector<GenParticleRef> >();
}

void DimuonMCMatcher::produce(edm::Event& evt, const edm::EventSetup&) {
  Handle<CandidateView> src;
  evt.getByLabel(src_, src);
  auto_ptr<vector<GenParticleRef> > matched(new vector<GenParticleRef>);
  matched->reserve(src->size());
  for(CandidateView::const_iterator i = src->begin(); i != src->end(); ++i) {
    const Candidate * dau1 = i->daughter(0);
    const Candidate * dau2 = i->daughter(1);
    if(dau1 == 0|| dau2 == 0) 
      throw Exception(errors::InvalidReference) <<
	"one of the two daughter does not exist\n";
    const Candidate * c1 = dau1->masterClone().get();
    GenParticleRef mc1;
    const pat::Muon * mu1 = dynamic_cast<const pat::Muon*>(c1);
    if(mu1 != 0) {
      mc1 = mu1->genParticleRef();
    } else {
      const pat::GenericParticle * gp1 = dynamic_cast<const pat::GenericParticle*>(c1);
      if(gp1 == 0) 
	throw Exception(errors::InvalidReference) <<
	  "first of two daughter is neither a pat::Muon not pat::GenericParticle\n";
      mc1 = gp1->genParticleRef();
    }
    const Candidate * c2 = dau2->masterClone().get();
    GenParticleRef mc2;
    const pat::Muon * mu2 = dynamic_cast<const pat::Muon*>(c2);
    if(mu2 != 0) {
      mc2 = mu2->genParticleRef();
    } else {
      const pat::GenericParticle * gp2 = dynamic_cast<const pat::GenericParticle*>(c2);
      if(gp2 == 0) 
	throw Exception(errors::InvalidReference) <<
	  "first of two daughter is neither a pat::Muon not pat::GenericParticle\n";
      mc2 = gp2->genParticleRef();
    }
    GenParticleRef dimuonMatch;
    if(mc1.isNonnull() && mc2.isNonnull()) {
      do {
	mc1 = mc1->numberOfMothers() > 0 ? mc1->motherRef() : GenParticleRef();
	mc2 = mc2->numberOfMothers() > 0 ? mc2->motherRef() : GenParticleRef();
      } while(mc1 != mc2 && mc1.isNonnull() && mc2.isNonnull());
      if(mc1.isNonnull() && mc2.isNonnull()) {
	dimuonMatch = mc1;
      }
    }
    matched->push_back(dimuonMatch);
  }

  evt.put(matched);
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DimuonMCMatcher);
