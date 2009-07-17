#ifndef _HFDUMPTRACKS_h_
#define _HFDUMPTRACKS_h_

#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TFile;
class TTree;
class TAna01Event;

class TrackAssociatorBase;

// ----------------------------------------------------------------------
class HFDumpTracks : public edm::EDAnalyzer {
 public:
  explicit HFDumpTracks(const edm::ParameterSet&);
  ~HFDumpTracks();
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  std::string          fTracksLabel, 
                       fGenEventLabel, fSimTracksLabel,
                       fAssociatorLabel, fTrackingParticlesLabel;
  edm::InputTag        fMuonsLabel;

  int                  fVerbose, fDoTruthMatching; 

  TrackAssociatorBase *fAssociator;
};

#endif
