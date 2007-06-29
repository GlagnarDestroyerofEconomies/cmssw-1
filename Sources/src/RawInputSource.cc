/*----------------------------------------------------------------------
$Id$
----------------------------------------------------------------------*/

#include "FWCore/Sources/interface/RawInputSource.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/LuminosityBlockPrincipal.h"
#include "FWCore/Framework/interface/RunPrincipal.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/Event.h"

namespace edm {
  RawInputSource::RawInputSource(ParameterSet const& pset,
				       InputSourceDescription const& desc) :
    InputSource(pset, desc),
    remainingEvents_(maxEvents()),
    runNumber_(RunNumber_t()),
    newRun_(true),
    newLumi_(true),
    ep_(),
    lbp_()
  { }

  RawInputSource::~RawInputSource() {
  }

  void
  RawInputSource::setRun(RunNumber_t r) {
    // Do nothing if the run is not changed.
    if (r != runNumber_) {
      runNumber_ = r;
      newRun_ = newLumi_ = true;
    }
  }

  void
  RawInputSource::setLumi(LuminosityBlockNumber_t lb) {
    if (lb != luminosityBlockNumber_) {
      luminosityBlockNumber_ = lb;
      newLumi_ = true;
    }
  }

  boost::shared_ptr<RunPrincipal>
  RawInputSource::readRun_() {
    newRun_ = false;
    return boost::shared_ptr<RunPrincipal>(
	new RunPrincipal(runNumber_, productRegistry(), processConfiguration()));
  }

  boost::shared_ptr<LuminosityBlockPrincipal>
  RawInputSource::readLuminosityBlock_(boost::shared_ptr<RunPrincipal> rp) {
    if (newRun_) {
      return boost::shared_ptr<LuminosityBlockPrincipal>();
    }
    newLumi_ = false;
    return boost::shared_ptr<LuminosityBlockPrincipal>(
	new LuminosityBlockPrincipal(luminosityBlockNumber_, productRegistry(), rp, processConfiguration()));
  }

  std::auto_ptr<EventPrincipal>
  RawInputSource::readEvent_(boost::shared_ptr<LuminosityBlockPrincipal>) {
    if (remainingEvents_ == 0 || newRun_ || newLumi_) {
      return std::auto_ptr<EventPrincipal>(0); 
    }
    std::auto_ptr<Event> e(readOneEvent());
    if (e.get() == 0) {
      return std::auto_ptr<EventPrincipal>(0); 
    }
    --remainingEvents_;
    e->commit_();
    return ep_;
  }

  std::auto_ptr<Event>
  RawInputSource::makeEvent(EventID & eventId, Timestamp const& tstamp) {
    eventId = EventID(runNumber_, eventId.event());
    ep_ = std::auto_ptr<EventPrincipal>(
	new EventPrincipal(eventId, Timestamp(tstamp),
	productRegistry(), lbp_, processConfiguration(), true));
    std::auto_ptr<Event> e(new Event(*ep_, moduleDescription()));
    return e;
  }

  std::auto_ptr<EventPrincipal>
  RawInputSource::readIt(EventID const&) {
      throw cms::Exception("LogicError","RawInputSource::readEvent_(EventID const& eventID)")
        << "Random access read cannot be used for RawInputSource.\n"
        << "Contact a Framework developer.\n";
  }

  // Not yet implemented
  void
  RawInputSource::skip(int) {
      throw cms::Exception("LogicError","RawInputSource::skip(int offset)")
        << "Random access skip cannot be used for RawInputSource\n"
        << "Contact a Framework developer.\n";
  }

}
