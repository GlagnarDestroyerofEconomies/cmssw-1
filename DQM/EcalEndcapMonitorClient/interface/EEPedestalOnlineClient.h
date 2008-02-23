#ifndef EEPedestalOnlineClient_H
#define EEPedestalOnlineClient_H

/*
 * \file EEPedestalOnlineClient.h
 *
 * $Date: 2008/01/18 18:04:09 $
 * $Revision: 1.11 $
 * \author G. Della Ricca
 * \author F. Cossutti
 *
*/

#include <vector>
#include <string>

#include "TROOT.h"
#include "TProfile2D.h"
#include "TH1F.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQM/EcalEndcapMonitorClient/interface/EEClient.h"

class MonitorElement;
class DaqMonitorBEInterface;
class EcalCondDBInterface;
class RunIOV;
class MonRunIOV;

class EEPedestalOnlineClient : public EEClient {

friend class EESummaryClient;

public:

/// Constructor
EEPedestalOnlineClient(const edm::ParameterSet& ps);

/// Destructor
virtual ~EEPedestalOnlineClient();

/// Analyze
void analyze(void);

/// BeginJob
void beginJob(DaqMonitorBEInterface* dbe);

/// EndJob
void endJob(void);

/// BeginRun
void beginRun(void);

/// EndRun
void endRun(void);

/// Setup
void setup(void);

/// Cleanup
void cleanup(void);

/// HtmlOutput
void htmlOutput(int run, std::string htmlDir, std::string htmlName);

/// WriteDB
bool writeDb(EcalCondDBInterface* econn, RunIOV* runiov, MonRunIOV* moniov);

/// Get Functions
inline int getEvtPerJob() { return ievt_; }
inline int getEvtPerRun() { return jevt_; }

private:

int ievt_;
int jevt_;

bool cloneME_;

bool verbose_;

bool enableMonitorDaemon_;

bool enableCleanup_;

std::string prefixME_;

std::vector<int> superModules_;

DaqMonitorBEInterface* dbe_;

MonitorElement* meh03_[18];

TProfile2D* h03_[18];

MonitorElement* meg03_[18];

MonitorElement* mep03_[18];

MonitorElement* mer03_[18];

// Quality check on crystals, one per each gain

float expectedMean_;
float discrepancyMean_;
float RMSThreshold_;

};

#endif
