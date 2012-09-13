#ifndef DQWorkerClient_H
#define DQWorkerClient_H

#include <utility>

#include "DQM/EcalCommon/interface/DQWorker.h"

namespace ecaldqm {
  class DQWorkerClient : public DQWorker {
  public:
    DQWorkerClient(edm::ParameterSet const&, edm::ParameterSet const&, std::string const&);
    virtual ~DQWorkerClient() {}

    void endLuminosityBlock(const edm::LuminosityBlock &, const edm::EventSetup &);

    void initialize();

    void reset();

    virtual void producePlots() = 0;

    enum Quality {
      kBad = 0,
      kGood = 1,
      kUnknown = 2,
      kMBad = 3,
      kMGood = 4,
      kMUnknown = 5
    };

  protected:
    void source_(unsigned, std::string const&, unsigned, edm::ParameterSet const&);
    bool applyMask_(unsigned, DetId const&, uint32_t);
    void towerAverage_(unsigned, unsigned, float);
    bool using_(unsigned _s) { return usedSources_ & (0x1 << _s); }

    std::vector<MESet const*> sources_;
    uint32_t usedSources_;
  };

}
#endif
