#ifndef MuonAnalyzerBase_H
#define MuonAnalyzerBase_H


/** \class MuonAnalyzerBase
 *
 *  base class for all DQM monitor sources
 *
 *  $Date: 2008/03/26 11:51:10 $
 *  $Revision: 1.2 $
 *  \author G. Mila - INFN Torino
 */


#include <memory>
#include <fstream>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "DQMServices/Daemon/interface/MonitorDaemon.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"

class MuonAnalyzerBase {
 public:

  /// Constructor (histo booking)
  MuonAnalyzerBase(MuonServiceProxy *theServ, DaqMonitorBEInterface * Dbe) {theService=theServ; theDbe=Dbe;}
  
  /// Destructor
  virtual ~MuonAnalyzerBase() {}
  
  /// Get the analysis of the muon properties
  void analyze(const edm::Event&, const edm::EventSetup&, reco::Muon& recoMuon){}

  /// Get the analysis of the muon track properties
  void analyze(const edm::Event&, const edm::EventSetup&, reco::Track& recoTrack){}

  MuonServiceProxy* service() {return theService;}

  DaqMonitorBEInterface* dbe() {return theDbe;}

 private:
  // ----------member data ---------------------------
  MuonServiceProxy *theService;
  DaqMonitorBEInterface * theDbe;

};
#endif  
