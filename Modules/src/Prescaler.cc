
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"

namespace edm {
  class Prescaler : public EDFilter {
  public:
    explicit Prescaler(ParameterSet const&);
    virtual ~Prescaler();

    virtual bool filter(Event& e, EventSetup const& c);
    void endJob();

  private:
    int count_;
    int n_; // accept one in n
    int offset_; // with offset, ie. sequence of events does not have to start at first event
  };

  Prescaler::Prescaler(ParameterSet const& ps) :
    count_(),
    n_(ps.getParameter<int>("prescaleFactor")),
    offset_(ps.getParameter<int>("prescaleOffset")) {
  }

  Prescaler::~Prescaler() {
  }

  bool Prescaler::filter(Event & e, EventSetup const&) {
    ++count_;
    return count_ % n_ == offset_ ? true : false;
  }

  void Prescaler::endJob() {
  }
}

using edm::Prescaler;
DEFINE_FWK_MODULE(Prescaler);
