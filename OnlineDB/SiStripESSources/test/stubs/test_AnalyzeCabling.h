// Last commit: $Id: $

#ifndef OnlineDB_SiStripESSources_test_AnalyzeCabling_H
#define OnlineDB_SiStripESSources_test_AnalyzeCabling_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

/**
   @class test_AnalyzeCabling 
   @brief Analyzes FEC (and FED) cabling object(s)
*/
class test_AnalyzeCabling : public edm::EDAnalyzer {

 public:
  
  test_AnalyzeCabling( const edm::ParameterSet& ) {;}
  virtual ~test_AnalyzeCabling() {;}
  
  void beginJob( edm::EventSetup const& );
  void analyze( const edm::Event&, const edm::EventSetup& ) {;}
  void endJob() {;}
  
};

#endif // OnlineDB_SiStripESSources_test_AnalyzeCabling_H

