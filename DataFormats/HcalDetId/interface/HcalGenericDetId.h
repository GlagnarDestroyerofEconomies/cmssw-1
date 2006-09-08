#ifndef HCALGENERICDETID_H
#define HCALGENERICDETID_H

/** \class HcalGenericDetId
    \author F.Ratnikov, UMd
   Generic HCAL detector ID suitable for all Hcal subdetectors
   $Id: HcalGenericDetId.h,v 1.1 2006/07/31 18:31:02 fedor Exp $
*/

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"


class HcalGenericDetId : public DetId {
 public:
  enum HcalGenericSubdetector {HcalGenEmpty=0, HcalGenBarrel=1, HcalGenEndcap=2, HcalGenOuter=3, HcalGenForward=4, 
			       HcalGenTriggerTower=5, HcalGenZDC=8, HcalGenCalibration=9};
  HcalGenericDetId () : DetId () {}
  HcalGenericDetId (uint32_t rawid) : DetId (rawid) {}
  HcalGenericDetId (const DetId& id) : DetId (id) {}
  HcalSubdetector subdet() const;
  HcalOtherSubdetector otherSubdet () const;
  HcalGenericSubdetector genericSubdet () const;
  bool isHcalDetId () const;
  bool isHcalCalibDetId () const;
  bool isHcalTrigTowerDetId () const;
  bool isHcalZDCDetId () const;
};

#endif
