#include "DQM/EcalCommon/interface/EcalDQMBinningService.h"
#include "DQM/EcalCommon/interface/EcalDQMCommonUtils.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/EcalDetId/interface/EcalPnDiodeDetId.h"

#include <algorithm>

#include "TMath.h"

using namespace ecaldqm;

EcalDQMBinningService::AxisSpecs
EcalDQMBinningService::getBinningEB_(BinningType _btype, bool _isMap, int _axis) const
{
  AxisSpecs specs;

  if(!_isMap){
    switch(_btype){
    case kTCC:
      specs.nbins = 36;
      specs.low = 9.;
      specs.high = 45.;
      specs.title = "iTCC";
      break;
    case kDCC:
      specs.nbins = 36;
      specs.low = 9.;
      specs.high = 45.;
      break;
    case kProjEta:
      specs.nbins = nEBEtaBins;
      specs.low = -etaBound;
      specs.high = etaBound;
      specs.title = "eta";
      break;
    case kProjPhi:
      specs.nbins = nPhiBins;
      specs.low = -TMath::Pi() / 18.;
      specs.high = TMath::Pi() * 35./18.;
      specs.title = "phi";
      break;
    default:
      break;
    }
  }
  else{
    switch(_btype){
    case kCrystal:
      if(_axis == 1)
        specs.nbins = 360;
      else if(_axis == 2)
        specs.nbins = 170;
      break;
    case kSuperCrystal:
    case kTriggerTower:
      if(_axis == 1)
        specs.nbins = 72;
      else if(_axis == 2)
        specs.nbins = 34;
      break;
    default:
      return specs;
    }

    if(_axis == 1){
      specs.low = 0.;
      specs.high = 360.;
      specs.title = "iphi";
    }
    else if(_axis == 2){
      specs.low = -85.;
      specs.high = 85.;
      specs.title = "ieta";
    }
  }

  return specs;
}

EcalDQMBinningService::AxisSpecs
EcalDQMBinningService::getBinningEE_(BinningType _btype, bool _isMap, int _zside, int _axis) const
{
  AxisSpecs specs;

  if(!_isMap){
    switch(_btype){
    case kTCC:
      specs.nbins = _zside ? 36 : 72;
      specs.low = 0.;
      specs.high = _zside ? 36. : 72.;
      specs.title = "iTCC";
      break;
    case kDCC:
      specs.nbins = _zside ? 9 : 18;
      specs.low = 0.;
      specs.high = _zside ? 9. : 18.;
      break;
    case kProjEta:
      if(_zside == 0) break;
      specs.nbins = nEEEtaBins;
      specs.low = _zside < 0 ? -3. : etaBound;
      specs.high = _zside < 0 ? -etaBound : 3.;
      specs.title = "eta";
      break;
    case kProjPhi:
      specs.nbins = nPhiBins;
      specs.low = -TMath::Pi() / 18.;
      specs.high = TMath::Pi() * 35./18.;
      specs.title = "phi";
      break;
    default:
      break;
    }
  }else{
    switch(_btype){
    case kCrystal:
    case kTriggerTower:
      if(_axis == 1)
        specs.nbins = _zside ? 100 : 200;
      if(_axis == 2)
        specs.nbins = 100;
      break;
    case kSuperCrystal:
      if(_axis == 1)
        specs.nbins = _zside ? 20 : 40;
      if(_axis == 2)
        specs.nbins = 20;
      break;
    default:
      return specs;
    }

    if(_axis == 1){
      specs.low = 0.;
      specs.high = _zside ? 100. : 200.;
      specs.title = "ix";
    }
    else if(_axis == 2){
      specs.low = 0.;
      specs.high = 100.;
      specs.title = "iy";
    }
  }

  return specs;
}

EcalDQMBinningService::AxisSpecs
EcalDQMBinningService::getBinningSM_(BinningType _btype, bool _isMap, unsigned _iObj, int _axis) const
{
  AxisSpecs specs;

  unsigned iSM(_iObj);

  const bool isBarrel(iSM >= kEBmLow && iSM <= kEBpHigh);

  if(!_isMap){
    switch(_btype){
    case kCrystal:
      specs.nbins = isBarrel ? 1700 : getElectronicsMap()->dccConstituents(iSM + 1).size();
      specs.low = 0.;
      specs.high = specs.nbins;
      specs.title = "crystal";
      break;
    case kTriggerTower:
      specs.nbins = isBarrel ? 68 : 80;
      specs.low = 0.;
      specs.high = specs.nbins;
      specs.title = "tower";
      break;
    case kSuperCrystal:
      specs.nbins = isBarrel ? 68 : nSuperCrystals(iSM + 1);
      specs.low = 0.;
      specs.high = specs.nbins;
      specs.title = "tower";
      break;
    default:
      break;
    }
  }else{
    int nEEX(nEESMX);
    if(iSM == kEEm02 || iSM == kEEm08 || iSM == kEEp02 || iSM == kEEp08) nEEX = nEESMXExt;

    switch(_btype){
    case kCrystal:
      if(_axis == 1)
        specs.nbins = isBarrel ? nEBSMEta : nEEX;
      else if(_axis == 2)
        specs.nbins = isBarrel ? nEBSMPhi : nEESMY;
      break;
    case kTriggerTower:
      if(_axis == 1)
        specs.nbins = isBarrel ? nEBSMEta / 5 : nEEX;
      else if(_axis == 2)
        specs.nbins = isBarrel ? nEBSMPhi / 5 : nEESMY;
      break;
    case kSuperCrystal:
      if(_axis == 1)
        specs.nbins = isBarrel ? nEBSMEta / 5 : nEEX / 5;
      else if(_axis == 2)
        specs.nbins = isBarrel ? nEBSMPhi / 5 : nEESMY / 5;
      break;
    default:
      return specs;
    }

    if(_axis == 1){
      specs.low = xlow(iSM);
      specs.high = specs.low + (isBarrel ? nEBSMEta : nEEX);
      specs.title = isBarrel ? (iSM < kEBpLow ? "-ieta" : "ieta") : "ix";
    }
    else if(_axis == 2){
      specs.low = ylow(iSM);
      specs.high = specs.low + (isBarrel ? nEBSMPhi : nEESMY);
      specs.title = isBarrel ? "iphi" : "iy";
    }
  }

  return specs;
}

EcalDQMBinningService::AxisSpecs
EcalDQMBinningService::getBinningSMMEM_(BinningType _btype, bool _isMap, unsigned _iObj, int _axis) const
{
  AxisSpecs specs;

  unsigned iSM(memDCCId(_iObj) - 1);

  if(iSM == unsigned(-1) || _btype != kCrystal) return specs;

  if(_axis == 1){
    specs.nbins = 10;
    specs.low = 0.;
    specs.high = 10.;
    if(_isMap) specs.title = "pseudo-strip";
    else specs.title = "iPN";
  }
  else if(_axis == 2){
    specs.nbins = 1;
    specs.low = 0.;
    specs.high = 5.;
    specs.title = "channel";
  }

  return specs;
}

EcalDQMBinningService::AxisSpecs
EcalDQMBinningService::getBinningEcal_(BinningType _btype, bool _isMap, int _axis) const
{
  AxisSpecs specs;

  if(!_isMap){
    switch(_btype){
    case kTCC:
      specs.nbins = 108;
      specs.low = 0.;
      specs.high = 108.;
      specs.title = "iTCC";
      break;
    case kDCC:
      specs.nbins = 54;
      specs.low = 0.;
      specs.high = 54.;
      specs.title = "iDCC";
      break;
    case kProjEta:
      specs.nbins = nEBEtaBins + 2 * nEEEtaBins;
      specs.edges = new double[specs.nbins + 1];
      for(int i(0); i <= nEEEtaBins; i++)
	specs.edges[i] = -3. + (3. - etaBound) / nEEEtaBins * i;
      for(int i(1); i <= nEBEtaBins; i++)
	specs.edges[i + nEEEtaBins] = -etaBound + 2. * etaBound / nEBEtaBins * i;
      for(int i(1); i <= nEEEtaBins; i++)
	specs.edges[i + nEEEtaBins + nEBEtaBins] = etaBound + (3. - etaBound) / nEEEtaBins * i;
      specs.title = "eta";
      break;
    case kProjPhi:
      specs.nbins = nPhiBins;
      specs.low = -TMath::Pi() / 18.;
      specs.high = TMath::Pi() * 35./18.;
      specs.title = "phi";
      break;
    default:
      break;
    }
  }
  else{
    switch(_btype){
    case kDCC:
      if(_axis == 1){
        specs.nbins = 9;
        specs.low = 0.;
        specs.high = 9.;
      }
      else if(_axis == 2){
        specs.nbins = 6;
        specs.low = 0.;
        specs.high = 6.;
      }
      break;
    default:
      break;
    }
  }

  return specs;
}

EcalDQMBinningService::AxisSpecs
EcalDQMBinningService::getBinningMEM_(BinningType _btype, bool _isMap, int _subdet, int _axis) const
{
  AxisSpecs specs;

  if(_btype != kCrystal) return specs;

  int nbins(44);
  if(_subdet == EcalBarrel) nbins = 36;
  else if(_subdet == EcalEndcap) nbins = 8;

  if(_axis == 1){
    specs.nbins = nbins;
    specs.low = 0.;
    specs.high = nbins;
  }
  else if(_axis == 2){
    specs.nbins = 10;
    specs.low = 0.;
    specs.high = 10.;
    specs.title = "iPN";
  }

  return specs;
}

int
EcalDQMBinningService::findBinCrystal_(ObjectType _otype, const DetId& _id, int _iSM/* = -1*/) const
{
  int xbin(0), ybin(0);
  int nbinsX(0);
  int subdet(_id.subdetId());

  if(subdet == EcalBarrel){
    EBDetId ebid(_id);
    int iphi(ebid.iphi());
    int ieta(ebid.ieta());
    switch(_otype){
    case kEB:
      xbin = iphi;
      ybin = ieta < 0 ? ieta + 86 : ieta + 85;
      nbinsX = 360;
      break;
    case kSM:
    case kEBSM:
      xbin = ieta < 0 ? -ieta : ieta;
      ybin = ieta < 0 ? (iphi - 1) % 20 + 1 : 20 - (iphi - 1) % 20;
      nbinsX = nEBSMEta;
      break;
    default:
      break;
    }
  }
  else if(subdet == EcalEndcap){
    EEDetId eeid(_id);
    int ix(eeid.ix());
    int iy(eeid.iy());
    switch(_otype){
    case kEE:
      xbin = eeid.zside() < 0 ? ix : ix + 100;
      ybin = iy;
      nbinsX = 200;
      break;
    case kEEm:
    case kEEp:
      xbin = ix;
      ybin = iy;
      nbinsX = 100;
      break;
    case kSM:
    case kEESM:
      {
        int iSM(_iSM >= 0 ? _iSM : dccId(_id) - 1);
        xbin = ix - xlow(iSM);
        ybin = iy - ylow(iSM);
        nbinsX = (iSM == kEEm02 ||
                  iSM == kEEm08 ||
                  iSM == kEEp02 ||
                  iSM == kEEp08) ? nEESMXExt : nEESMX;
      }
      break;
    default:
      break;
    }
  }
  else if(subdet == EcalLaserPnDiode){
    EcalPnDiodeDetId pnid(_id);
    switch(_otype){
    case kSMMEM:
    case kEBSMMEM:
    case kEESMMEM:
      xbin = pnid.iPnId();
      ybin = 1;
      nbinsX = 10;
      break;
    case kMEM:
      xbin = memDCCIndex(dccId(_id)) + 1;
      ybin = pnid.iPnId();
      nbinsX = 44;
      break;
    case kEBMEM:
      xbin = memDCCIndex(dccId(_id)) - 3;
      ybin = pnid.iPnId();
      nbinsX = 36;
      break;
    case kEEMEM:
      xbin = memDCCIndex(dccId(_id)) + 1;
      if(xbin > kEEmHigh + 1) xbin -= 36;
      ybin = pnid.iPnId();
      nbinsX = 8;
      break;
    default:
      break;
    }
  }

  return (nbinsX + 2) * ybin + xbin;
}

int
EcalDQMBinningService::findBinTriggerTower_(ObjectType _otype, DetId const& _id) const
{
  int xbin(0);
  int ybin(0);
  int nbinsX(0);
  int subdet(_id.subdetId());

  if((subdet == EcalTriggerTower && !isEndcapTTId(_id)) || subdet == EcalBarrel){
    EcalTrigTowerDetId ttid;
    if(subdet == EcalBarrel) ttid = EBDetId(_id).tower();
    else ttid = _id;

    int ieta(ttid.ieta());
    int iphi((ttid.iphi() + 1) % 72 + 1);
    switch(_otype){
    case kEB:
      xbin = iphi;
      ybin = ieta < 0 ? ieta + 18 : ieta + 17;
      nbinsX = 72;
      break;
    case kSM:
    case kEBSM:
      xbin = ieta < 0 ? -ieta : ieta;
      ybin = ieta < 0 ? (iphi - 1) % 4 + 1 : 4 - (iphi - 1) % 4;
      nbinsX = 17;
      break;
    default:
      break;
    }
  }
  else if(subdet == EcalEndcap){
    unsigned tccid(tccId(_id));
    unsigned iSM(tccid <= 36 ? tccid % 18 / 2 : (tccid - 72) % 18 / 2);
    return findBinCrystal_(_otype, _id, iSM);
  }

  return (nbinsX + 2) * ybin + xbin;
}

int
EcalDQMBinningService::findBinSuperCrystal_(ObjectType _otype, const DetId& _id, int _iSM/* -1*/) const
{
  int xbin(0);
  int ybin(0);
  int nbinsX(0);
  int subdet(_id.subdetId());

  if(subdet == EcalBarrel){
    EBDetId ebid(_id);
    int iphi(ebid.iphi());
    int ieta(ebid.ieta());
    switch(_otype){
    case kEB:
      xbin = (iphi - 1) / 5 + 1;
      ybin = (ieta < 0 ? ieta + 85 : ieta + 84) / 5 + 1;
      nbinsX = 72;
      break;
    case kSM:
    case kEBSM:
      xbin = (ieta < 0 ? -ieta - 1 : ieta - 1) / 5 + 1;
      ybin = (ieta < 0 ? (iphi - 1) % 20 : 19 - (iphi - 1) % 20) / 5 + 1;
      nbinsX = nEBSMEta / 5;
      break;
    default:
      break;
    }
  }
  else if(subdet == EcalEndcap){
    if(isEcalScDetId(_id)){
      EcalScDetId scid(_id);
      int ix(scid.ix());
      int iy(scid.iy());
      switch(_otype){
      case kEE:
        {
          int zside(scid.zside());
          xbin = zside < 0 ? ix : ix + 20;
          ybin = iy;
          nbinsX = 40;
        }
        break;
      case kEEm:
      case kEEp:
        xbin = ix;
        ybin = iy;
        nbinsX = 20;
        break;
      case kSM:
      case kEESM:
        {
          int iSM(_iSM >= 0 ? _iSM : dccId(_id) - 1);
          xbin = ix - xlow(iSM) / 5;
          ybin = iy - ylow(iSM) / 5;
          nbinsX = (iSM == kEEm02 ||
                    iSM == kEEm08 ||
                    iSM == kEEp02 ||
                    iSM == kEEp08) ? nEESMXExt / 5 : nEESMX / 5;
        }
        break;
      default:
        break;
      }
    }
    else{
      EEDetId eeid(_id);
      int ix(eeid.ix());
      int iy(eeid.iy());
      switch(_otype){
      case kEE:
        xbin = (eeid.zside() < 0 ? ix - 1 : ix + 99) / 5 + 1;
        ybin = (iy - 1) / 5 + 1;
        nbinsX = 40;
        break;
      case kEEm:
      case kEEp:
        xbin = (ix - 1) / 5 + 1;
        ybin = (iy - 1) / 5 + 1;
        nbinsX = 20;
        break;
      case kSM:
      case kEESM:
        {
          int iSM(_iSM >= 0 ? _iSM : dccId(_id) - 1);
          xbin = (ix - xlow(iSM) - 1) / 5 + 1;
          ybin = (iy - ylow(iSM) - 1) / 5 + 1;
          nbinsX = (iSM == kEEm02 ||
                    iSM == kEEm08 ||
                    iSM == kEEp02 ||
                    iSM == kEEp08) ? nEESMXExt / 5 : nEESMX / 5;
        }
        break;
      default:
        break;
      }
    }
  }
  else if(subdet == EcalTriggerTower && !isEndcapTTId(_id)){
    EcalTrigTowerDetId ttid(_id);
    int ieta(ttid.ieta());
    int iphi((ttid.iphi() + 1) % 72 + 1);
    switch(_otype){
    case kEB:
      xbin = iphi;
      ybin = ieta < 0 ? ieta + 18 : ieta + 17;
      nbinsX = 72;
      break;
    case kSM:
    case kEBSM:
      xbin = ieta < 0 ? -ieta : ieta;
      ybin = ieta < 0 ? (iphi - 1) % 4 + 1 : 4 - (iphi - 1) % 4;
      nbinsX = nEBSMEta / 5;
      break;
    default:
      break;
    }
  }

  return (nbinsX + 2) * ybin + xbin;
}

int
EcalDQMBinningService::findBinSuperCrystal_(ObjectType _otype, const EcalElectronicsId& _id) const
{
  int xbin(0);
  int ybin(0);
  int nbinsX(0);
  int idcc(_id.dccId());

  if(idcc >= kEBmLow && idcc <= kEBpHigh){
    unsigned towerid(_id.towerId());
    bool isEBm(idcc <= kEBmHigh);
    switch(_otype){
    case kEB:
      xbin = 4 * ((idcc - 9) % 18) + (isEBm ? towerid - 1 : 68 - towerid) % 4 + 1;
      ybin = (towerid - 1) / 4 * (isEBm ? -1 : 1) + (isEBm ? 18 : 17);
      break;
    case kSM:
    case kEBSM:
      xbin = (towerid - 1) / 4 + 1;
      ybin = (isEBm ? towerid - 1 : 68 - towerid) % 4 + 1;
      break;
    default:
      break;
    }
  }
  else{
    return findBinSuperCrystal_(_otype, EEDetId(getElectronicsMap()->getDetId(_id)).sc());
  }

  return (nbinsX + 2) * ybin + xbin;
}
