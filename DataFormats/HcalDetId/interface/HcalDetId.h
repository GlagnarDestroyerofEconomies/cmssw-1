#ifndef DATAFORMATS_HCALDETID_HCALDETID_H
#define DATAFORMATS_HCALDETID_HCALDETID_H 1

#include <ostream>
#include <boost/cstdint.hpp>
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"


/** \class HcalDetId
 *  Cell identifier class for the HCAL subdetectors, precision readout cells only
 *
 *  There are two encodings for this class.  Version 0 (original)
 *  was supplanted by version 1 when the ECAL/HCAL iphi=1 alignment occurred
 *  (0_9_0)
 *
 *  $Date: 2006/02/15 20:14:17 $
 *  $Revision: 1.7 $
 *  \author J. Mans - Minnesota
 */
class HcalDetId : public DetId {
public:
  /** Create a null cellid*/
  HcalDetId();
  /** Create cellid from raw id (0=invalid tower id) */
  HcalDetId(uint32_t rawid);
  /** Constructor from subdetector, signed tower ieta,iphi,and depth */
  HcalDetId(HcalSubdetector subdet, int tower_ieta, int tower_iphi, int depth);
  /** Constructor from a generic cell id */
  HcalDetId(const DetId& id);
  /** Assignment from a generic cell id */
  HcalDetId& operator=(const DetId& id);
  /** Constructor from a hcal cell id (required for conversion) */
  HcalDetId(const HcalDetId& id);
  /** Assignment from an hcal cell id (required for conversion) */
  HcalDetId& operator=(const HcalDetId& id);

  /// get the subdetector
  HcalSubdetector subdet() const { return (HcalSubdetector)(subdetId()); }
  /// get the z-side of the cell (1/-1)
  int zside() const { return (id_&0x2000)?(1):(-1); }
  /// get the absolute value of the cell ieta
  int ietaAbs() const { return (id_>>7)&0x3f; }
  /// get the cell ieta
  int ieta() const { return zside()*ietaAbs(); }
  /// get the cell iphi 
  int iphi() const; 
  /// get the tower depth
  int depth() const { return (id_>>14)&0x7; }
  /// get the smallest crystal_ieta of the crystal in front of this tower (HB and HE tower 17 only)
  int crystal_ieta_low() const { return ((ieta()-zside())*5)+zside(); }
  /// get the largest crystal_ieta of the crystal in front of this tower (HB and HE tower 17 only)
  int crystal_ieta_high() const { return ((ieta()-zside())*5)+5*zside(); }
  /// get the smallest crystal_iphi of the crystal in front of this tower (HB and HE tower 17 only)
  int crystal_iphi_low() const { return ((iphi()-1)*5)+1; }
  /// get the largest crystal_iphi of the crystal in front of this tower (HB and HE tower 17 only)
  int crystal_iphi_high() const { return ((iphi()-1)*5)+5; }

  /// get the encoding version (for expert use)
  int encodingVersion() const { return (id_>>17)&0x3; }

  static const HcalDetId Undefined;

};

std::ostream& operator<<(std::ostream&,const HcalDetId& id);

#endif
