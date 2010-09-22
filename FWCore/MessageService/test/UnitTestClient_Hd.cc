#define EDM_ML_DEBUG

#include "FWCore/MessageService/test/UnitTestClient_Hd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <iostream>
#include <string>

namespace edmtest
{


void
  UnitTestClient_Hd::analyze( edm::Event      const & e
                           , edm::EventSetup const & /*unused*/
                              )
{
       LogTrace    ("cat_A") << "LogTrace was used to send this mess" << "age";
       LogDebug    ("cat_B") << "LogDebug was used to send this other message";
  edm::LogVerbatim ("cat_A") << "LogVerbatim was us" << "ed to send this message";
  edm::LogInfo     ("cat_B") << "LogInfo was used to send this other message";
}  // MessageLoggerClient::analyze()


}  // namespace edmtest


using edmtest::UnitTestClient_Hd;
DEFINE_FWK_MODULE(UnitTestClient_Hd);
