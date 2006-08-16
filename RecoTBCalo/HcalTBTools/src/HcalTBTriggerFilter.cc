#include "RecoTBCalo/HcalTBTools/interface/HcalTBTriggerFilter.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTriggerData.h"
#include "FWCore/Framework/interface/Handle.h"

HcalTBTriggerFilter::HcalTBTriggerFilter(const edm::ParameterSet& ps) :
  allowPedestal_(ps.getParameter<bool>("AllowPedestal")),
  allowPedestalInSpill_(ps.getParameter<bool>("AllowPedestalInSpill")),
  allowPedestalOutSpill_(ps.getParameter<bool>("AllowPedestalOutSpill")),
  allowLaser_(ps.getParameter<bool>("AllowLaser")),
  allowLED_(ps.getParameter<bool>("AllowLED")),
  allowBeam_(ps.getParameter<bool>("AllowBeam")),
  label_(ps.getUntrackedParameter<std::string>("label",""))
{
}

bool HcalTBTriggerFilter::filter(edm::Event& e, edm::EventSetup const& c) {
  edm::Handle<HcalTBTriggerData> h;

  if (label_.length()<2) 
    e.getByType(h);
  else
    e.getByLabel(label_,h);

  bool allowed=false;

  if (h->wasBeamTrigger()) {
    allowed=allowBeam_;
    //    if (!allowed) std::cout << "Rejecting beam trigger from " << e.id() << std::endl;
  }

  if (h->wasLEDTrigger()) {
    allowed=allowLED_;
    //    if (!allowed) std::cout << "Rejecting LED trigger from " << e.id() << std::endl;
  }

  if (h->wasLaserTrigger()) {
    allowed=allowLaser_;
    //    if (!allowed) std::cout << "Rejecting laser trigger from " << e.id() << std::endl;
  }

  if (h->wasInSpillPedestalTrigger() || h->wasOutSpillPedestalTrigger() || h->wasSpillIgnorantPedestalTrigger()) {
    allowed=allowPedestal_;
    //    if (!allowed) std::cout << "Rejecting pedestal trigger from " << e.id() << std::endl;
    if (allowed && h->wasInSpillPedestalTrigger() && !allowPedestalInSpill_) {
      allowed=false;
      //      if (!allowed) std::cout << "Rejecting in-spill pedestal trigger from " << e.id() << std::endl;
    }
    if (allowed && h->wasOutSpillPedestalTrigger() && !allowPedestalOutSpill_) {
      allowed=false;
      //      if (!allowed) std::cout << "Rejecting out-of-spill pedestal trigger from " << e.id() << std::endl;
    }
  }

  return allowed;
}
