#ifndef EcalDCCUnpackingModule_H
#define EcalDCCUnpackingModule_H

/** \class EcalUnpackingModule
 * 
 *
 *  $Date: 2005/10/18 09:06:15 $
 *  $Revision: 1.3 $
 * \author N. Marinelli 
 * \author G. Della Ricca
 */

#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/EDProducer.h>

#include <DQM/EcalBarrelMonitorModule/interface/EcalBarrelMonitorDaemon.h>

#include <iostream>
#include <string>

using namespace std;

class EcalTBDaqFormatter;


  class EcalDCCUnpackingModule: public edm::EDProducer {
  public:
    /// Constructor
    EcalDCCUnpackingModule(const edm::ParameterSet& pset);

    /// Destructor
    virtual ~EcalDCCUnpackingModule();
    
    /// Produce digis out of raw data
    void produce(edm::Event & e, const edm::EventSetup& c);

    // BeginJob
    void beginJob(const edm::EventSetup& c);

    // EndJob
    void endJob(void);

  private:
    EcalTBDaqFormatter* formatter;

    DaqMonitorBEInterface* dbe;

    MonitorElement* meIntegrity[36];

    string outputFile;
  };

DEFINE_FWK_MODULE(EcalDCCUnpackingModule);

#endif
