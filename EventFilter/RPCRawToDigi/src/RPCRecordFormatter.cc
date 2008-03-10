/** \file
 * Implementation of class RPCRecordFormatter
 *
 *  $Date: 2008/01/22 19:12:36 $
 *  $Revision: 1.30 $
 *
 * \author Ilaria Segoni
 */

#include "EventFilter/RPCRawToDigi/interface/RPCRecordFormatter.h"

#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/RPCDigi/interface/RPCDigi.h"

#include "CondFormats/RPCObjects/interface/RPCReadOutMapping.h"
#include "CondFormats/RPCObjects/interface/LinkBoardElectronicIndex.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"


#include <bitset>
#include <sstream>

using namespace std;
using namespace edm;
using namespace rpcrawtodigi;


RPCRecordFormatter::RPCRecordFormatter(int fedId, const RPCReadOutMapping *r)
 : currentFED(fedId), readoutMapping(r)
{ }

RPCRecordFormatter::~RPCRecordFormatter()
{ }


std::vector<EventRecords> RPCRecordFormatter::recordPack(
    uint32_t rawDetId, const RPCDigi & digi, int trigger_BX) const 
{
  std::vector<EventRecords> result;
   
  LogTrace("") << " DIGI;  det: " << rawDetId<<", strip: "<<digi.strip()<<", bx: "<<digi.bx();
  int stripInDU = digi.strip();

  // decode digi<->map
  typedef std::vector< std::pair< LinkBoardElectronicIndex, LinkBoardPackedStrip> > RawDataFrames;
  RPCReadOutMapping::StripInDetUnit duFrame(rawDetId, stripInDU);
  RawDataFrames rawDataFrames = readoutMapping->rawDataFrame(duFrame);

  for (RawDataFrames::const_iterator ir = rawDataFrames.begin(); ir != rawDataFrames.end(); ir++) {
    
    const LinkBoardElectronicIndex & eleIndex = (*ir).first;
    const LinkBoardPackedStrip & lbPackedStrip = (*ir).second;

    if (eleIndex.dccId == currentFED) {

      LogTrace("pack:")
           <<" dccId= "<<eleIndex.dccId
           <<" dccInputChannelNum= "<<eleIndex.dccInputChannelNum
           <<" tbLinkInputNum= "<<eleIndex.tbLinkInputNum
           <<" lbNumInLink="<<eleIndex.lbNumInLink;

      // BX 
      int current_BX = trigger_BX+digi.bx();
      RecordBX bxr(current_BX);

      // LB 
      int tbLinkInputNumber = eleIndex.tbLinkInputNum;
      int rmb = eleIndex.dccInputChannelNum; 
      RecordSLD lbr( tbLinkInputNumber, rmb);   

      // CD record
      int chamberInLink = eleIndex.lbNumInLink;
      int eod = 0;
      int halfP = 0;
      int packedStrip = lbPackedStrip.packedStrip();     
      int partitionNumber = packedStrip/8; 
      RecordCD cdr(chamberInLink, partitionNumber, eod, halfP, vector<int>(1,packedStrip) );

      result.push_back(  EventRecords(trigger_BX, bxr, lbr, cdr) );
    }
  }
  return result;
}

void RPCRecordFormatter::recordUnpack(
    const EventRecords & event, std::auto_ptr<RPCDigiCollection> & prod)
{
  int triggerBX = event.triggerBx();
  int currentBX = event.recordBX().bx();
  int currentRMB = event.recordSLD().rmb(); 
  int currentTbLinkInputNumber = event.recordSLD().tbLinkInputNumber();

  LinkBoardElectronicIndex eleIndex;
  eleIndex.dccId = currentFED;
  eleIndex.dccInputChannelNum = currentRMB;
  eleIndex.tbLinkInputNum = currentTbLinkInputNumber;
  eleIndex.lbNumInLink = event.recordCD().chamber();

  if(readoutMapping == 0) return;
  const LinkBoardSpec* linkBoard = readoutMapping->location(eleIndex);
  if (!linkBoard) {
    throw cms::Exception("Invalid Linkboard location!") 
              << "dccId: "<<eleIndex.dccId
              << "dccInputChannelNum: " <<eleIndex.dccInputChannelNum
              << " tbLinkInputNum: "<<eleIndex.tbLinkInputNum
              << " lbNumInLink: "<<eleIndex.lbNumInLink;
  }

  std::vector<int> packStrips = event.recordCD().packedStrips();
  for(std::vector<int>::iterator is = packStrips.begin(); is != packStrips.end(); ++is) {

    RPCReadOutMapping::StripInDetUnit duFrame = 
        readoutMapping->detUnitFrame(*linkBoard, LinkBoardPackedStrip(*is) );

    uint32_t rawDetId = duFrame.first;
    int geomStrip = duFrame.second;
    if (!rawDetId) {
      LogError("recordUnpack: problem with rawDetId, skip LB data");
      continue;
    }

    // Creating RPC digi
    RPCDigi digi(geomStrip,currentBX-triggerBX);

    /// Committing digi to the product
    LogTrace("")<<" DIGI;  det: "<<rawDetId<<", strip: "<<digi.strip()<<", bx: "<<digi.bx();
    LogTrace("") << " LinkBoardElectronicIndex: " << eleIndex.print(); 
    prod->insertDigi(RPCDetId(rawDetId),digi);
  }
}
