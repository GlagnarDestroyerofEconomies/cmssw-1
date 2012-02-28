/*
 * \file EETriggerTowerClient.cc
 *
 * $Date: 2011/09/02 13:55:03 $
 * $Revision: 1.99 $
 * \author G. Della Ricca
 * \author F. Cossutti
 *
*/

#include <memory>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DQM/EcalCommon/interface/UtilsClient.h"
#include "DQM/EcalCommon/interface/Numbers.h"

#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalDetId/interface/EcalTriggerElectronicsId.h"

#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"

#include "DQM/EcalEndcapMonitorClient/interface/EETriggerTowerClient.h"

EETriggerTowerClient::EETriggerTowerClient(const edm::ParameterSet& ps) {

  // cloneME switch
  cloneME_ = ps.getUntrackedParameter<bool>("cloneME", true);

  // verbose switch
  verbose_ = ps.getUntrackedParameter<bool>("verbose", true);

  // debug switch
  debug_ = ps.getUntrackedParameter<bool>("debug", false);

  // prefixME path
  prefixME_ = ps.getUntrackedParameter<std::string>("prefixME", "");

  // enableCleanup_ switch
  enableCleanup_ = ps.getUntrackedParameter<bool>("enableCleanup", false);

  // vector of selected Super Modules (Defaults to all 18).
  superModules_.reserve(18);
  for ( unsigned int i = 1; i <= 18; i++ ) superModules_.push_back(i);
  superModules_ = ps.getUntrackedParameter<std::vector<int> >("superModules", superModules_);

  for ( unsigned int i=0; i<superModules_.size(); i++ ) {

    int ism = superModules_[i];

    o01_[ism-1] = 0;

    meo01_[ism-1] = 0;

  }

  for ( unsigned int i=0; i<superModules_.size(); i++ ) {

    int ism = superModules_[i];

    me_o01_[ism-1] = 0;
    me_o02_[ism-1] = 0;

  }

  ievt_ = 0;
  jevt_ = 0;
  dqmStore_ = 0;

}

EETriggerTowerClient::~EETriggerTowerClient() {

}

void EETriggerTowerClient::beginJob(void) {

  dqmStore_ = edm::Service<DQMStore>().operator->();

  if ( debug_ ) std::cout << "EETriggerTowerClient: beginJob" << std::endl;

  ievt_ = 0;
  jevt_ = 0;

}

void EETriggerTowerClient::beginRun(void) {

  if ( debug_ ) std::cout << "EETriggerTowerClient: beginRun" << std::endl;

  jevt_ = 0;

  this->setup();

}

void EETriggerTowerClient::endJob(void) {

  if ( debug_ ) std::cout << "EETriggerTowerClient: endJob, ievt = " << ievt_ << std::endl;

  this->cleanup();

}

void EETriggerTowerClient::endRun(void) {

  if ( debug_ ) std::cout << "EETriggerTowerClient: endRun, jevt = " << jevt_ << std::endl;

  this->cleanup();

}

void EETriggerTowerClient::setup(void) {

  std::string name;

  dqmStore_->setCurrentFolder( prefixME_ + "/TriggerPrimitives/Timing" );

  for ( unsigned int i=0; i<superModules_.size(); i++ ) {

    int ism = superModules_[i];

    if ( me_o01_[ism-1] ) dqmStore_->removeElement( me_o01_[ism-1]->getName() );
    name = "TrigPrimClient Timing " + Numbers::sEE(ism);
    me_o01_[ism-1] = dqmStore_->book2D(name, name, 50, Numbers::ix0EE(ism)+0., Numbers::ix0EE(ism)+50., 50, Numbers::iy0EE(ism)+0., Numbers::iy0EE(ism)+50.);
    me_o01_[ism-1]->setAxisTitle("ix", 1);
    if ( ism >= 1 && ism <= 9 ) me_o01_[ism-1]->setAxisTitle("101-ix", 1);
    me_o01_[ism-1]->setAxisTitle("iy", 2);

//     if ( me_o02_[ism-1] ) dqmStore_->removeElement( me_o02_[ism-1]->getName() );
//     name = "TrigPrimClient Non Single Timing " + Numbers::sEE(ism);
//     me_o02_[ism-1] = dqmStore_->book2D(name, name, 50, Numbers::ix0EE(ism)+0., Numbers::ix0EE(ism)+50., 50, Numbers::iy0EE(ism)+0., Numbers::iy0EE(ism)+50.);
//     me_o02_[ism-1]->setAxisTitle("ix", 1);
//     if ( ism >= 1 && ism <= 9 ) me_o02_[ism-1]->setAxisTitle("101-ix", 1);
//     me_o02_[ism-1]->setAxisTitle("iy", 2);
//     me_o02_[ism-1]->setAxisTitle("fraction", 3);

  }

  for ( unsigned int i=0; i<superModules_.size(); i++ ) {

    int ism = superModules_[i];

    if ( me_o01_[ism-1] ) me_o01_[ism-1]->Reset();
    if ( me_o02_[ism-1] ) me_o02_[ism-1]->Reset();

  }

}

void EETriggerTowerClient::cleanup(void) {

  if ( ! enableCleanup_ ) return;

  for ( unsigned int i=0; i<superModules_.size(); i++ ) {

    int ism = superModules_[i];

    if ( cloneME_ ) {
      if ( o01_[ism-1] ) delete o01_[ism-1];
    }

    o01_[ism-1] = 0;

    mel01_[ism-1] = 0;
    meo01_[ism-1] = 0;

  }

  for ( unsigned int i=0; i<superModules_.size(); i++ ) {

    int ism = superModules_[i];

    if ( me_o01_[ism-1] ) dqmStore_->removeElement( me_o01_[ism-1]->getFullname() );
    me_o01_[ism-1] = 0;
    if ( me_o02_[ism-1] ) dqmStore_->removeElement( me_o02_[ism-1]->getFullname() );
    me_o02_[ism-1] = 0;

  }

}

#ifdef WITH_ECAL_COND_DB
bool EETriggerTowerClient::writeDb(EcalCondDBInterface* econn, RunIOV* runiov, MonRunIOV* moniov, bool& status) {

  status = true;

  return true;

}
#endif

void EETriggerTowerClient::analyze(void) {

  ievt_++;
  jevt_++;
  if ( ievt_ % 10 == 0 ) {
    if ( debug_ ) std::cout << "EETriggerTowerClient: ievt/jevt = " << ievt_ << "/" << jevt_ << std::endl;
  }

  MonitorElement* me;
  std::string name;
  std::stringstream ss;

  for ( unsigned int i=0; i<superModules_.size(); i++ ) {

    int ism = superModules_[i];
    int zside(ism <= 9 ? -1 : 1);

    me = dqmStore_->get( prefixME_ + "/TriggerPrimitives/EmulMatching/TrigPrimTask matching index " + Numbers::sEE(ism) );
    o01_[ism-1] = UtilsClient::getHisto( me, cloneME_, o01_[ism-1] );
    meo01_[ism-1] = me;

    if ( me_o01_[ism-1] ) me_o01_[ism-1]->Reset();
    if ( me_o02_[ism-1] ) me_o02_[ism-1]->Reset();

    if ( o01_[ism-1] ) {
      for (int ix = 1; ix <= 50; ix++) {
	for (int iy = 1; iy <= 50; iy++) {

	  int jx(ix + Numbers::ix0EE(ism));
	  int jy(iy + Numbers::iy0EE(ism));
	  if(zside < 0) jx = 101 - jx;
	  
	  if(!EEDetId::validDetId(jx, jy, zside)) continue;

	  EEDetId id(jx, jy, zside);
	  if(Numbers::iSM(id) != (unsigned)ism) continue;

          // find the most frequent TP timing that matches the emulator
          float index=-1;
          double max=0;
          double total=0;

	  EcalTriggerElectronicsId teid(Numbers::getElectronicsMapping()->getTriggerElectronicsId(id));
	  int itcc(teid.tccId());
	  int itt(teid.ttId());

	  int xitt;
	  if(itcc <= 18){ //inner EE-
	    xitt = ((itcc-1) % 2) * 24 + itt;
	  }else if(itcc <= 36){ //outer EE-
	    xitt = 48 + ((itcc-1) % 2) * 16 + itt;
	  }else if(itcc <= 90){ //outer EE+
	    xitt = 48 + ((itcc-1) % 2) * 16 + itt;
	  }else{
	    xitt = ((itcc-1) % 2) * 24 + itt;
	  }

          for (int j = -1; j<6; j++) {
            double sampleEntries = o01_[ism-1]->GetBinContent(xitt, j+2);
            if(sampleEntries > max) {
              index = j;
              max = sampleEntries;
            }
            total += sampleEntries;
          }
          if ( max > 0 ) {
	    me_o01_[ism-1]->setBinContent(ix, iy, index );
          }

	  if ((int)total != (int)max) {
	    ss.str("");
	    ss << "TT " << itcc << " " << itt;
	    name = "TrigPrimClient non single timing " + ss.str();
	    dqmStore_->setCurrentFolder(prefixME_ + "/TriggerPrimitives/EmulationErrors/Timing");
	    me = dqmStore_->book1D(name, name, 1, 0., 1.);
	    me->setBinContent(1, 1.0 - max / total);
	  }

        }

      }
    }

  }

}

