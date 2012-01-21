#ifndef HLTTriggerTypeFilter_h
#define HLTTriggerTypeFilter_h
// -*- C++ -*-
//
// Package:    HLTTriggerTypeFilter
// Class:      HLTTriggerTypeFilter
// 
/**\class HLTTriggerTypeFilter HLTTriggerTypeFilter.cc 

Description: <one line class summary>

Implementation:
<Notes on implementation>
*/
//
// Original Author:  Giovanni FRANZONI
//         Created:  Tue Jan 22 13:55:00 CET 2008
// $Id: HLTTriggerTypeFilter.h,v 1.2 2009/08/06 11:23:34 fwyzard Exp $
//
//


// include files
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"


//
// class declaration
//

class HLTTriggerTypeFilter : public HLTFilter {
public:
  explicit HLTTriggerTypeFilter(const edm::ParameterSet&);
  ~HLTTriggerTypeFilter();
  
private:
  virtual bool hltFilter(edm::Event&, const edm::EventSetup&, trigger::TriggerFilterObjectWithRefs & filterproduct);
  
  // ----------member data ---------------------------  
  unsigned short  SelectedTriggerType_;
  
};

#endif
