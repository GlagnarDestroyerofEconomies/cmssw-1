#ifndef DCCEETCCBLOCK_HH
#define DCCEETCCBLOCK_HH

/*
 *\ Class DCCEETCCBlock
 *
 * Class responsible for the EE Trigger Tower primitives unpacking.
 *
 * \file DCCEETCCBlock.h
 *
 * $Date: 2007/07/24 11:39:35 $
 * $Revision: 1.5 $
 *
 * \author N. Almeida
 *
*/

#include <iostream>                  
#include <string>
#include <vector>
#include <map>
#include <utility>


#include <DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h>
#include <DataFormats/EcalDigi/interface/EcalTriggerPrimitiveSample.h>
#include <DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h>
#include <DataFormats/EcalDigi/interface/EcalDigiCollections.h>

#include "DCCTCCBlock.h"

class DCCEETCCBlock : public DCCTCCBlock{
	
  public :
    /**
      Class constructor
    */
    DCCEETCCBlock( DCCDataUnpacker * u, EcalElectronicsMapper * m, DCCEventBlock * e, bool unpacking );    
  
    void updateCollectors();
	 
    void addTriggerPrimitivesToCollection();
  
  protected :
  
    bool checkTccIdAndNumbTTs();


};

#endif
