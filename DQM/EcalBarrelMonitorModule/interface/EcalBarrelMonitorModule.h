#ifndef EcalBarrelMonitorModule_H
#define EcalBarrelMonitorModule_H

/*
 * \file EcalBarrelMonitorModule.h
 *
 * $Date: 2005/10/12 15:36:19 $
 * $Revision: 1.8 $
 * \author G. Della Ricca
 *
*/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include <FWCore/Framework/interface/EDAnalyzer.h>

#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/MakerMacros.h>

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "DQMServices/Daemon/interface/MonitorDaemon.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <DataFormats/EcalDigi/interface/EcalDigiCollections.h>
#include <DataFormats/EcalDetId/interface/EBDetId.h>

#include <DQM/EcalBarrelMonitorTasks/interface/EBPedestalTask.h>
#include <DQM/EcalBarrelMonitorTasks/interface/EBTestPulseTask.h>
#include <DQM/EcalBarrelMonitorTasks/interface/EBLaserTask.h>
#include <DQM/EcalBarrelMonitorTasks/interface/EBCosmicTask.h>

#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace cms;
using namespace std;

class EcalBarrelMonitorModule: public edm::EDAnalyzer{

public:

/// Constructor
EcalBarrelMonitorModule(const edm::ParameterSet& ps);

/// Destructor
virtual ~EcalBarrelMonitorModule();

protected:

/// Analyze
void analyze(const edm::Event& e, const edm::EventSetup& c);

// EndJob
virtual void endJob(void);

private:

int ievt;

DaqMonitorBEInterface* dbe;

MonitorElement* meStatus;

MonitorElement* meRun;
MonitorElement* meEvt;

MonitorElement* meEbarrel;

MonitorElement* meEvent[36];

EBPedestalTask*  pedestal_task;
EBTestPulseTask* testpulse_task;
EBLaserTask*     laser_task;
EBCosmicTask*    cosmic_task;

string outputFile;

ofstream logFile;

};

DEFINE_FWK_MODULE(EcalBarrelMonitorModule)

#endif
