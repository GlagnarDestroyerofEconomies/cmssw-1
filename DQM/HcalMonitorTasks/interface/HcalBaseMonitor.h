#ifndef DQM_HCALMONITORTASKS_HCALBASEMONITOR_H
#define DQM_HCALMONITORTASKS_HCALBASEMONITOR_H

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include <iostream>

using namespace std;
/** \class HcalBaseMonitor
  *  
  * $Date: 2005/11/17 17:55:35 $
  * $Revision: 1.3 $
  * \author W. Fisher - FNAL
  */
class HcalBaseMonitor {
public:
  HcalBaseMonitor(); 
  virtual ~HcalBaseMonitor(); 

  virtual void setup(const edm::ParameterSet& ps, DaqMonitorBEInterface* dbe);
  virtual void done();

  void setVerbosity(int verb) { fVerbosity = verb; }
  int getVerbosity() const { return fVerbosity; }

protected:
  
  int fVerbosity;
  DaqMonitorBEInterface* m_dbe;

};

#endif
