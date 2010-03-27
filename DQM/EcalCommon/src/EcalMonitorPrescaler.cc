// $Id: EcalMonitorPrescaler.cc,v 1.12 2009/11/06 10:43:13 dellaric Exp $

/*!
  \file EcalMonitorPrescaler.cc
  \brief Ecal specific Prescaler
  \author G. Della Ricca
  \version $Revision: 1.12 $
  \date $Date: 2009/11/06 10:43:13 $
*/

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/EcalRawData/interface/EcalRawDataCollections.h"

#include <DQM/EcalCommon/interface/EcalMonitorPrescaler.h>

EcalMonitorPrescaler::EcalMonitorPrescaler(edm::ParameterSet const& ps) {

  count_ = 0;

  EcalRawDataCollection_ = ps.getParameter<edm::InputTag>("EcalRawDataCollection");

  occupancyPrescaleFactor_ = ps.getUntrackedParameter<int>("occupancyPrescaleFactor" , 0);
  integrityPrescaleFactor_ = ps.getUntrackedParameter<int>("integrityPrescaleFactor", 0);
  statusflagsPrescaleFactor_ = ps.getUntrackedParameter<int>("statusflagsPrescaleFactor" , 0);

  pedestalonlinePrescaleFactor_ = ps.getUntrackedParameter<int>("pedestalonlinePrescaleFactor", 0);

  laserPrescaleFactor_ = ps.getUntrackedParameter<int>("laserPrescaleFactor", 0);
  ledPrescaleFactor_ = ps.getUntrackedParameter<int>("ledPrescaleFactor", 0);
  pedestalPrescaleFactor_ = ps.getUntrackedParameter<int>("pedestalPrescaleFactor", 0);
  testpulsePrescaleFactor_ = ps.getUntrackedParameter<int>("testpulsePrescaleFactor", 0);

  pedestaloffsetPrescaleFactor_ = ps.getUntrackedParameter<int>("pedestaloffsetPrescaleFactor", 0);

  triggertowerPrescaleFactor_ = ps.getUntrackedParameter<int>("triggertowerPrescaleFactor" , 0);
  timingPrescaleFactor_ = ps.getUntrackedParameter<int>("timingPrescaleFactor" , 0);

  cosmicPrescaleFactor_ = ps.getUntrackedParameter<int>("cosmicPrescaleFactor", 0);

  physicsPrescaleFactor_ = ps.getUntrackedParameter<int>("physicsPrescaleFactor", 0);

  clusterPrescaleFactor_ = ps.getUntrackedParameter<int>("clusterPrescaleFactor", 0);

}
    
EcalMonitorPrescaler::~EcalMonitorPrescaler() { }

bool EcalMonitorPrescaler::filter(edm::Event &e, edm::EventSetup const &c) {

  count_++;

  bool status = false;

  if ( occupancyPrescaleFactor_ ) {
    if ( count_ % occupancyPrescaleFactor_ == 0 ) status = true;
  }
  if ( integrityPrescaleFactor_ ) {
    if ( count_ % integrityPrescaleFactor_ == 0 ) status = true;
  }
  if ( statusflagsPrescaleFactor_ ) {
    if ( count_ % statusflagsPrescaleFactor_ == 0 ) status = true;
  }

  if ( pedestalonlinePrescaleFactor_ ) {
    if ( count_ % pedestalonlinePrescaleFactor_ == 0 ) status = true;
  }

  if ( triggertowerPrescaleFactor_ ) {
    if ( count_ % triggertowerPrescaleFactor_ == 0 ) status = true;
  }
  if ( timingPrescaleFactor_ ) {
    if ( count_ % timingPrescaleFactor_ == 0 ) status = true;
  }

  edm::Handle<EcalRawDataCollection> dcchs;

  if ( e.getByLabel(EcalRawDataCollection_, dcchs) ) {

    for ( EcalRawDataCollection::const_iterator dcchItr = dcchs->begin(); dcchItr != dcchs->end(); ++dcchItr ) {

      EcalDCCHeaderBlock dcch = (*dcchItr);

      if ( dcch.getRunType() == EcalDCCHeaderBlock::LASER_STD ||
           dcch.getRunType() == EcalDCCHeaderBlock::LASER_GAP ) {
        if ( laserPrescaleFactor_ ) { 
          if ( count_ % laserPrescaleFactor_ == 0 ) status = true;
        }
      }
      if ( dcch.getRunType() == EcalDCCHeaderBlock::LED_STD ||
           dcch.getRunType() == EcalDCCHeaderBlock::LED_GAP ) {
        if ( ledPrescaleFactor_ ) {
          if ( count_ % ledPrescaleFactor_ == 0 ) status = true;
        }
      }
      if ( dcch.getRunType() == EcalDCCHeaderBlock::PEDESTAL_STD ||
           dcch.getRunType() == EcalDCCHeaderBlock::PEDESTAL_GAP ) {
        if ( pedestalPrescaleFactor_ ) { 
          if ( count_ % pedestalPrescaleFactor_ == 0 ) status = true;
        }
      }
      if ( dcch.getRunType() == EcalDCCHeaderBlock::TESTPULSE_MGPA ||
           dcch.getRunType() == EcalDCCHeaderBlock::TESTPULSE_GAP ) {
        if ( testpulsePrescaleFactor_ ) { 
          if ( count_ % testpulsePrescaleFactor_ == 0 ) status = true;
        }
      }

      if ( dcch.getRunType() == EcalDCCHeaderBlock::PEDESTAL_OFFSET_SCAN ) {
        if ( pedestaloffsetPrescaleFactor_ ) {
          if ( count_ % pedestaloffsetPrescaleFactor_ == 0 ) status = true;
        }
      }

      if ( dcch.getRunType() == EcalDCCHeaderBlock::COSMIC ||
           dcch.getRunType() == EcalDCCHeaderBlock::COSMICS_GLOBAL ||
           dcch.getRunType() == EcalDCCHeaderBlock::COSMICS_LOCAL ) {
        if ( cosmicPrescaleFactor_ ) {
          if ( count_ % cosmicPrescaleFactor_ == 0 ) status = true;
        }
      }

      if ( dcch.getRunType() == EcalDCCHeaderBlock::MTCC ||
           dcch.getRunType() == EcalDCCHeaderBlock::PHYSICS_GLOBAL ||
           dcch.getRunType() == EcalDCCHeaderBlock::PHYSICS_LOCAL ) {
        if ( physicsPrescaleFactor_ ) {
          if ( count_ % physicsPrescaleFactor_ == 0 ) status = true;
        }
      }

      if ( dcch.getRunType() == EcalDCCHeaderBlock::COSMIC ||
           dcch.getRunType() == EcalDCCHeaderBlock::MTCC ||
           dcch.getRunType() == EcalDCCHeaderBlock::COSMICS_GLOBAL ||
           dcch.getRunType() == EcalDCCHeaderBlock::PHYSICS_GLOBAL ||
           dcch.getRunType() == EcalDCCHeaderBlock::COSMICS_LOCAL ||
           dcch.getRunType() == EcalDCCHeaderBlock::PHYSICS_LOCAL ) {
        if ( clusterPrescaleFactor_ ) {
          if ( count_ % clusterPrescaleFactor_ == 0 ) status = true;
        }
      }

    }

  } else {

    edm::LogWarning("EcalMonitorPrescaler") << EcalRawDataCollection_ << " not available";

  }

  return status;

}

void EcalMonitorPrescaler::endJob() { }

