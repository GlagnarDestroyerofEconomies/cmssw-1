// Last commit: $Id: testSiStripKey.h,v 1.2 2007/07/31 15:20:25 ratnik Exp $

#ifndef DataFormats_SiStripCommon_testSiStripKey_H
#define DataFormats_SiStripCommon_testSiStripKey_H

#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include <string>

/**
   @class testSiStripKey 
   @author R.Bainbridge
   @brief Simple class that tests SiStripKey.
*/
class testSiStripKey : public edm::EDAnalyzer {

 public:
  
  testSiStripKey( const edm::ParameterSet& );
  ~testSiStripKey();
  
  void beginJob( edm::EventSetup const& );
  void analyze( const edm::Event&, const edm::EventSetup& );
  void endJob() {;}

 private:

  sistrip::KeyType keyType_;
  uint32_t key_;
  std::string path_;
  
};

#endif // DataFormats_SiStripCommon_testSiStripKey_H

