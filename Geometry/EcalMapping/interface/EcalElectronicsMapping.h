#ifndef ECALELECTRONICSMAPPING_H
#define ECALELECTRONICSMAPPING_H 1

#include <memory>
#include <iostream>
#include <string>

#include "DataFormats/EcalDetId/interface/EcalDetIdCollections.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"


#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/composite_key.hpp>

#include <vector>


/** \class EcalElectronicsMapping
  *  
  * $Id: EcalElectronicsMapping.h,v 1.1 2007/03/09 07:31:42 eperez Exp $
  * \author P.Meridiani (INFN Roma1),  E. Perez (CERN)  
  */

class EcalElectronicsMapping {

  
 public:
  
  EcalElectronicsMapping();
  
  /// Get the electronics id for this det id 
  EcalElectronicsId getElectronicsId(const DetId& id) const;
  
  /// Get the trigger electronics id for this det id 
  EcalTriggerElectronicsId getTriggerElectronicsId(const DetId& id) const;

  /// Get the detid  given an electronicsId
  DetId getDetId(const EcalElectronicsId& id) const;

  /// Get the trigger electronics id  given an electronicsId
  EcalTriggerElectronicsId getTriggerElectronicsId(const EcalElectronicsId& id) const;

  /// Get the detid  given a trigger electronicsId
  DetId getDetId(const EcalTriggerElectronicsId& id) const;

  /// Get the electronics id given a trigger electronicsId
  EcalElectronicsId getElectronicsId(const EcalTriggerElectronicsId& id) const;

  /// Get the constituent detids for this dccId 
  std::vector<DetId> dccConstituents(int dccId) const;

  /// Get the constituent detids for this dccId 
  std::vector<DetId> dccTowerConstituents(int dccId, int tower) const;

  /// Get the constituent detids for this dccId 
  std::vector<DetId> stripConstituents(int dccId, int tower, int strip) const;

  /// Get the constituent detids for this dccId 
  std::vector<DetId> tccConstituents(int tccId) const;

  /// Get the constituent detids for this dccId 
  std::vector<DetId> ttConstituents(int tccId, int tt) const;

  /// Get the constituent detids for this dccId 
  std::vector<DetId> pseudoStripConstituents(int tccId, int tt, int pseudostrip) const;

  /// set the association between a DetId and a tower
  void assign(const DetId& cell, const EcalElectronicsId&, const EcalTriggerElectronicsId& tower);

  /// returns the DCC and DCC_channel of an EcalScDetId
  std::pair<int, int> getDCCandSC(EcalScDetId id) const;

  /// builds an EcalScDetId from (DCC, DCC_channel)
  EcalScDetId getEcalScDetId(int DCCid, int DCC_Channel) const;

  /// returns the DCC of an EBDetId
  int DCCid(const EBDetId& id) const;

  /// returns the TCCid of an EBDetId
  int TCCid(const EBDetId& id) const;

  /// returns the index of a Trigger Tower within its TCC.
  int iTT(const EcalTrigTowerDetId& id) const;

  /// returns the TCCid of a Trigger Tower
  int TCCid(const EcalTrigTowerDetId& id) const;

  /// returns the DCCid (i.e. the FED) of a Trigger Tower 
  int DCCid(const EcalTrigTowerDetId& id) const;

  /// Builds a EcalTrigTowerDetID from the TCCid & TriggerTower index in TCC
  EcalTrigTowerDetId getTrigTowerDetId(int TCCid, int iTT) const;

  EcalSubdetector subdet(int dccid, int mode) const;
  int zside(int dcctcc, int mode) const;

  bool rightTower(int tower) const;


 // Geometry of SM in EB :
  static const int kCrystalsInPhi = EBDetId::kCrystalsInPhi ; // per SM
  static const int kTowersInPhi = EBDetId::kTowersInPhi ;  // per SM

 // Geometry of the Trigger Towers :

  static const int kEBTowersInPhi = EcalTrigTowerDetId::kEBTowersInPhi ; // per SM (in the Barrel)
  static const int kEBTowersPerSM = EcalTrigTowerDetId::kEBTowersPerSM ; // per SM (in the Barrel)
  static const int kEBTowersInEta = EcalTrigTowerDetId::kEBTowersInEta; // per SM (in the Barrel)
  static const int kEETowersInEta = EcalTrigTowerDetId::kEETowersInEta; // Endcap
  static const int kEETowersInPhiPerQuadrant = EcalTrigTowerDetId::kEETowersInPhiPerQuadrant; // per Quadrant (EE)
                                                                                                                                                    
  static const int kEETowersInPhiPerTCC = 4;    // each TCC contains 4 towers in phi
  static const int kEETowersInEtaPerInnerTCC = 7;   // each inner TCC contains 7 towers in eta
  static const int kEETowersInEtaPerOuterTCC = 4;   // each outer TCC contains 4 towers in eta
  static const int iEEEtaMinOuter = 18;         // outer TCC : ieta = 18 -> 21
  static const int iEEEtaMinInner = 22;         // inner TCC : ieta = 22 -> 28


 // DCC values :
  static const int MAX_DCCID = EcalElectronicsId::MAX_DCCID ; //To be updated with correct and final number
  static const int MIN_DCCID = EcalElectronicsId::MIN_DCCID;
  static const int MIN_DCCID_EEM = EcalElectronicsId::MIN_DCCID_EEM ;
  static const int MAX_DCCID_EEM = EcalElectronicsId::MAX_DCCID_EEM ;
  static const int MIN_DCCID_EBM = EcalElectronicsId::MIN_DCCID_EBM ;
  static const int MAX_DCCID_EBM = EcalElectronicsId::MAX_DCCID_EBM ;
  static const int MIN_DCCID_EBP = EcalElectronicsId::MIN_DCCID_EBP ;
  static const int MAX_DCCID_EBP = EcalElectronicsId::MAX_DCCID_EBP ;
  static const int MIN_DCCID_EEP = EcalElectronicsId::MIN_DCCID_EEP ;
  static const int MAX_DCCID_EEP = EcalElectronicsId::MAX_DCCID_EEP ;

  static const int DCCID_PHI0_EBM = EcalElectronicsId::DCCID_PHI0_EBM ;   // contains phi = 0 deg.
  static const int DCCID_PHI0_EBP = EcalElectronicsId::DCCID_PHI0_EBP ;

 // TCC values :
  static const int MAX_TCCID = EcalTriggerElectronicsId::MAX_TCCID ; //To be updated with correct and final number
  static const int MIN_TCCID = EcalTriggerElectronicsId::MIN_TCCID ;
  static const int MIN_TCCID_EEM = EcalTriggerElectronicsId::MIN_TCCID_EEM ;
  static const int MAX_TCCID_EEM = EcalTriggerElectronicsId::MAX_TCCID_EEM ;
  static const int MIN_TCCID_EBM = EcalTriggerElectronicsId::MIN_TCCID_EBM ;
  static const int MAX_TCCID_EBM = EcalTriggerElectronicsId::MAX_TCCID_EBM ;
  static const int MIN_TCCID_EBP = EcalTriggerElectronicsId::MIN_TCCID_EBP ;
  static const int MAX_TCCID_EBP = EcalTriggerElectronicsId::MAX_TCCID_EBP ;
  static const int MIN_TCCID_EEP = EcalTriggerElectronicsId::MIN_TCCID_EEP ;
  static const int MAX_TCCID_EEP = EcalTriggerElectronicsId::MAX_TCCID_EEP ;

  static const int TCCID_PHI0_EBM = EcalTriggerElectronicsId::TCCID_PHI0_EBM ;
  static const int TCCID_PHI0_EBP = EcalTriggerElectronicsId::TCCID_PHI0_EBP ;

  static const int TCCID_PHI0_EEM_IN = EcalTriggerElectronicsId::TCCID_PHI0_EEM_IN ;
  static const int TCCID_PHI0_EEM_OUT = EcalTriggerElectronicsId::TCCID_PHI0_EEM_OUT ;
  static const int TCCID_PHI0_EEP_IN = EcalTriggerElectronicsId::TCCID_PHI0_EEP_IN ;
  static const int TCCID_PHI0_EEP_OUT = EcalTriggerElectronicsId::TCCID_PHI0_EEP_OUT ;

  static const int kTCCinPhi = 18;      // Number of TCC "sectors" in phi


 private:

  static const int DCCMODE = 0;
  static const int TCCMODE = 1;

  /// Maybe these are needed
  /// Wrap a generic EEDetId to the equivalent one in z+ Quadrant 1 (from 0 < phi < pi/2) 
  //  DetId wrapEEDetId(const DetId& id) const;
  /// Wrap a generic EcalTrigTowerDetId to the equivalent one in z+ Quadrant 1 (from 0 < phi < pi/2) 
  //DetId wrapEcalTrigTowerDetId(const DetId& id) const;
  //DetId changeEEDetIdQuadrantAndZ(const DetId& fromid, const int& toQuadrant,const int& tozside) const;
  //  int changeTowerQuadrant(int phiTower, int fromQuadrant, int toQuadrant) const;

  struct MapItem {
    MapItem(const DetId& acell, const EcalElectronicsId& aelid, const EcalTriggerElectronicsId& atrelid) : cell(acell),elid(aelid),trelid(atrelid) { }
    DetId cell;
    EcalElectronicsId elid;
    EcalTriggerElectronicsId trelid;
    int dccId() const { return elid.dccId(); }
    int towerId() const {return elid.towerId(); }
    int stripId() const {return elid.stripId(); }
    int tccId() const { return trelid.tccId(); }
    int ttId() const { return trelid.ttId(); }
    int pseudoStripId() const { return trelid.pseudoStripId(); }
  };


  //hashed indexes to be preferred to ordered (faster for lookup, here we are not interested in ordering...)
  typedef boost::multi_index::multi_index_container<
    MapItem,
    boost::multi_index::indexed_by<
    // hashed_unique< member < MapItem,DetId,&MapItem::cell > >,
    // hashed_unique< member < MapItem,EcalElectronicsId,&MapItem::elid > >,
    // hashed_unique< member < MapItem,EcalTriggerElectronicsId,&MapItem::trelid > >
     boost::multi_index::ordered_unique< boost::multi_index::member < MapItem,DetId,&MapItem::cell > >,
     boost::multi_index::ordered_unique< boost::multi_index::member < MapItem,EcalElectronicsId,&MapItem::elid > >,
     boost::multi_index::ordered_unique< boost::multi_index::member < MapItem,EcalTriggerElectronicsId,&MapItem::trelid > >,
     boost::multi_index::ordered_non_unique< boost::multi_index::const_mem_fun < MapItem, int, &MapItem::dccId > >,
     boost::multi_index::ordered_non_unique<
	boost::multi_index::composite_key<
		MapItem,
		boost::multi_index::const_mem_fun < MapItem, int, &MapItem::dccId > ,
		boost::multi_index::const_mem_fun < MapItem, int, &MapItem::towerId  >  
	> >,
     boost::multi_index::ordered_non_unique<
        boost::multi_index::composite_key<
                MapItem,
                boost::multi_index::const_mem_fun < MapItem, int, &MapItem::dccId > ,
                boost::multi_index::const_mem_fun < MapItem, int, &MapItem::towerId  >,
		boost::multi_index::const_mem_fun < MapItem, int, &MapItem::stripId >
	> >,
     boost::multi_index::ordered_non_unique< boost::multi_index::const_mem_fun < MapItem, int, &MapItem::tccId > >,
     boost::multi_index::ordered_non_unique<
        boost::multi_index::composite_key<
                MapItem,
                boost::multi_index::const_mem_fun < MapItem, int, &MapItem::tccId > ,
                boost::multi_index::const_mem_fun < MapItem, int, &MapItem::ttId  >
        > >,
     boost::multi_index::ordered_non_unique<
        boost::multi_index::composite_key<
                MapItem,
                boost::multi_index::const_mem_fun < MapItem, int, &MapItem::tccId > ,
                boost::multi_index::const_mem_fun < MapItem, int, &MapItem::ttId  >,
                boost::multi_index::const_mem_fun < MapItem, int, &MapItem::pseudoStripId >
        > >
    >
    > EcalElectronicsMap;
  
   typedef EcalElectronicsMap::nth_index<0>::type EcalElectronicsMap_by_DetId;
   typedef EcalElectronicsMap::nth_index<1>::type EcalElectronicsMap_by_ElectronicsId;
   typedef EcalElectronicsMap::nth_index<2>::type EcalElectronicsMap_by_TriggerElectronicsId;

   typedef EcalElectronicsMap::nth_index<3>::type EcalElectronicsMap_by_DccId;
   typedef EcalElectronicsMap::nth_index<4>::type EcalElectronicsMap_by_DccId_and_TowerId;
   typedef EcalElectronicsMap::nth_index<5>::type EcalElectronicsMap_by_DccId_TowerId_and_StripId;
  
   typedef EcalElectronicsMap::nth_index<6>::type EcalElectronicsMap_by_TccId;
   typedef EcalElectronicsMap::nth_index<7>::type EcalElectronicsMap_by_TccId_and_TtId;
   typedef EcalElectronicsMap::nth_index<8>::type EcalElectronicsMap_by_TccId_TtId_and_PseudostripId;


  //Needed only in the EE (contains only first quadrant object) 
  EcalElectronicsMap m_items;


};

#endif
