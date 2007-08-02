///////////////////////////////////////////////////////////////////////////////
// File: ZdcNumberingScheme.cc
// Date: 02.04
// Description: Numbering scheme for Zdc
// Modifications:
///////////////////////////////////////////////////////////////////////////////
#include "SimG4CMS/Forward/interface/ZdcNumberingScheme.h"
#include "DataFormats/HcalDetId/interface/HcalZDCDetId.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include <iostream>
#undef debug

ZdcNumberingScheme::ZdcNumberingScheme(int iv){
  if (verbosity>0) 
    std::cout << "Creating ZDCNumberingScheme" << std::endl;
}

ZdcNumberingScheme::~ZdcNumberingScheme() {
  std::cout<< " Deleting ZdcNumberingScheme" <<  std::endl;
}


void ZdcNumberingScheme::setVerbosity(const int iv){ verbosity = iv; }

unsigned int ZdcNumberingScheme::getUnitID(const G4Step* aStep) const {

  unsigned intindex=0;
  uint32_t  index = 0;
  int level = detectorLevel(aStep);
 
  if (level > 0) {
    int*      copyno = new int[level];
    G4String* name   = new G4String[level];

    detectorLevel(aStep, level, copyno, name);

    int zside   = 0;
    int channel = 0;
    int fiber   = 0;
    int layer   = 0;
    HcalZDCDetId::Section section;
    
    for (int ich=0; ich  <  level; ich++) {
      if (name[ich] == "ZDC") {
	if(copyno[ich] == 1)zside = 1;
	if(copyno[ich] == 2)zside = -1;
      } 
      else if (name[ich] == "ZDC_EMLayer") {
	section = HcalZDCDetId::EM;
	layer = copyno[ich];
      }
      else if (name[ich] == "ZDC_EMFiber") {
	fiber = copyno[ich];
	if (fiber < 20)
	  channel = 1;
	else if (fiber < 39)            
	  channel = 2;                         
	else if (fiber < 58)
	  channel = 3; 
	else if (fiber < 77)
	  channel = 4;
	else
	  channel = 5;	
      } 
      else if (name[ich] == "ZDC_LumLayer") {
	section = HcalZDCDetId::LUM;
	layer = copyno[ich];
	channel = layer;
      }
      else if (name[ich] == "ZDC_LumGas") {
	fiber = 1;
      }
      else if (name[ich] == "ZDC_HadLayer") {
	section = HcalZDCDetId::HAD;
	layer = copyno[ich];
      	if (layer < 6)
	  channel = 1;
      	else if (layer < 12)
      	  channel = 2;
	else if (layer < 18)
	  channel = 3;
      	else
	  channel = 4;
      }
      else if (name[ich] == "ZDC_HadFiber") {
	fiber = copyno[ich];
      }
    }
 
    // intindex = myPacker.packZdcIndex (section, layer, fiber, channel, zside);
    intindex = packZdcIndex (section, layer, fiber, channel, zside);
    bool true_for_positive_eta;
    if(zside = 1)true_for_positive_eta = true;
    if(zside = 1)true_for_positive_eta = false;

    HcalZDCDetId zdcId = HcalZDCDetId(section, true_for_positive_eta, channel);
    index = zdcId.rawId();

#ifdef debug
    std::cout<<"DetectorId: ";
    std::cout<<zdcId<<std::endl;

    
    std::cout<< "ZdcNumberingScheme:" 
	     << "  getUnitID - # of levels = " 
	     << level << std::endl;
    for (int ich = 0; ich < level; ich++)
      std::cout<< "  " << ich  << ": copyno " << copyno[ich] 
	       << " name="  << name[ich]
	       << "  section " << section << " zside " << zside
	       << " layer " << layer << " fiber " << fiber
	       << " channel " << channel << "packedIndex ="
	       << intindex << " detId raw: "<<index<<std::endl;
               
#endif
    
    delete[] copyno;
    delete[] name;
  }
  
  return index;
  
}

unsigned ZdcNumberingScheme::packZdcIndex(int section, int layer, int fiber,
					  int channel, int z){
  unsigned int idx = ((z-1)&1)<<20;       //bit 20
  idx += (channel&7)<<17;                 //bits 17-19
  idx += (fiber&255)<<9;                  //bits 9-16
  idx += (layer&127)<<2;                  //bits 2-8
  idx += (section&3);                      //bits 0-1

  #ifdef debug
  std::cout<< "ZDC packing: section " << section << " layer  " << layer << " fiber "
	   << fiber  << " channel " <<  channel << " zside "  << z << "idx: " <<idx <<  std::endl;
  int newsubdet, newlayer, newfiber, newchannel, newz;
  unpackZdcIndex(idx, newsubdet, newlayer, newfiber, newchannel, newz);
  #endif

  return idx;
}


void ZdcNumberingScheme::unpackZdcIndex(const unsigned int& idx, int& section,
					int& layer, int& fiber,
					int& channel, int& z) {
  z = 1 + (idx>>20)&1;
  channel = (idx>>17)&7;
  fiber = (idx>>9)&255;
  layer = (idx>>2)&127;
  section = idx&3;
	  
  #ifdef debug
  std::cout<< "ZDC unpacking: idx:"<< idx << " -> section " << section
	   << " layer " << layer << " fiber " << fiber << " channel " 
	   << channel << " zside " << z <<  std::endl;
  #endif 
}

int ZdcNumberingScheme::detectorLevel(const G4Step* aStep) const {
  
  //Find number of levels
  const G4VTouchable* touch = aStep->GetPreStepPoint()->GetTouchable();
  int level = 0;
  if (touch) level = ((touch->GetHistoryDepth())+1);
  return level;
}
  
void ZdcNumberingScheme::detectorLevel(const G4Step* aStep, int& level,
                                          int* copyno, G4String* name) const {
 
  //Get name and copy numbers
  if (level > 0) {
    const G4VTouchable* touch = aStep->GetPreStepPoint()->GetTouchable();
    for (int ii = 0; ii < level; ii++) {
      int i      = level - ii - 1;
      name[ii]   = touch->GetVolume(i)->GetName();
      copyno[ii] = touch->GetReplicaNumber(i);
    }
  }
}
