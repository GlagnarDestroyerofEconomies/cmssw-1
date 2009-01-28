#include "CondFormats/RPCObjects/interface/ChamberStripSpec.h"
#include "CondFormats/RPCObjects/interface/FebLocationSpec.h"
#include "CondFormats/RPCObjects/interface/ChamberLocationSpec.h"

#include "CondFormats/RPCObjects/interface/FebConnectorSpec.h"
#include "CondFormats/RPCObjects/interface/L1RPCConeBuilder.h"
#include "CondFormats/RPCObjects/interface/L1RPCHwConfig.h"

#include "CondFormats/RPCObjects/interface/RPCStripNoises.h"

#include "CondFormats/RPCObjects/interface/RPCObGas.h"
#include "CondFormats/RPCObjects/interface/RPCObCond.h"
#include "CondFormats/RPCObjects/interface/RPCObPVSSmap.h"
#include "CondFormats/RPCObjects/interface/LinkBoardSpec.h"
#include "CondFormats/RPCObjects/interface/LinkConnSpec.h"
#include "CondFormats/RPCObjects/interface/TriggerBoardSpec.h"
#include "CondFormats/RPCObjects/interface/DccSpec.h"
#include "CondFormats/RPCObjects/interface/RPCReadOutMapping.h"
#include "CondFormats/RPCObjects/interface/RPCEMap.h"

#include "CondFormats/RPCObjects/interface/RPCTechTriggerConfig.h"
#include "CondFormats/RPCObjects/interface/RBCBoardSpecs.h"
#include "CondFormats/RPCObjects/interface/TTUBoardSpecs.h"

namespace{
  struct dictionary {
    std::vector<ChamberStripSpec> theStrips;
 
    std::vector<FebConnectorSpec> theFebs;
 
    std::vector<LinkBoardSpec> theLBs; 
 
    std::vector<LinkConnSpec> theLinks; 
 
    std::vector<TriggerBoardSpec> theTBs; 
 
    std::map<int, DccSpec> theFeds; 
  };
}

