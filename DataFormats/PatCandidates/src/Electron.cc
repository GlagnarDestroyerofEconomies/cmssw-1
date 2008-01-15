//
// $Id: Electron.cc,v 1.1 2008/01/07 11:48:25 lowette Exp $
//

#include "DataFormats/PatCandidates/interface/Electron.h"


using namespace pat;


/// default constructor
Electron::Electron() : Lepton<ElectronType>() {
}


/// constructor from ElectronType
Electron::Electron(const ElectronType & anElectron) : Lepton<ElectronType>(anElectron) {
}


/// destructor
Electron::~Electron() {
}


/// return the tracker isolation variable
float Electron::getTrackIso() const {
  return trackIso_;
}


/// return the calorimeter isolation variable
float Electron::getCaloIso() const {
  return caloIso_;
}


/// return the lepton ID discriminator
float Electron::getLeptonID() const {
  return leptonID_;
}


/// return the "robust cuts-based" electron id
float Electron::getElectronIDRobust() const {
  return electronIDRobust_;
}


/// return tracker isolation as calc. by Egamma POG producer
float Electron::getEgammaTkIso() const {
  return egammaTkIso_;
}


/// return "number of tracks" isolation as calc. by Egamma POG producer
int Electron::getEgammaTkNumIso() const {
  return egammaTkNumIso_;
}


/// return ecal isolation as calc. by Egamma POG producer
float Electron::getEgammaEcalIso() const {
  return egammaEcalIso_;
}


/// return hcal isolation as calc. by Egamma POG producer
float Electron::getEgammaHcalIso() const {
  return egammaHcalIso_;
}


/// method to set the tracker isolation variable
void Electron::setTrackIso(float trackIso) {
  trackIso_ = trackIso;
}


/// method to set the calorimeter isolation variable
void Electron::setCaloIso(float caloIso) {
  caloIso_ = caloIso;
}


/// method to set the lepton ID discriminator
void Electron::setLeptonID(float id) {
  leptonID_ = id;
}


/// method to set the "robust cuts-based" electron id
void Electron::setElectronIDRobust(float id) {
  electronIDRobust_ = id;
}


/// methods to set the isolation from the Egamma POG's producer
void Electron::setEgammaTkIso(float iso) {
  egammaTkIso_=iso;
}
void Electron::setEgammaTkNumIso(int iso) {
  egammaTkNumIso_=iso;
}
void Electron::setEgammaEcalIso(float iso) {
  egammaEcalIso_=iso;
}
void Electron::setEgammaHcalIso(float iso) {
  egammaHcalIso_=iso;
}
