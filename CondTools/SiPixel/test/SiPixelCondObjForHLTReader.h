#ifndef CondTools_SiPixel_SiPixelCondObjForHLTReader_H
#define CondTools_SiPixel_SiPixelCondObjForHLTReader_H
// -*- C++ -*-
//
// Package:    SiPixelCondObjForHLTReader
// Class:      SiPixelCondObjForHLTReader
// 
/**\class SiPixelCondObjForHLTReader SiPixelCondObjForHLTReader.h SiPixel/test/SiPixelCondObjForHLTReader.h

 Description: Test analyzer for reading pixel calibration from the DB

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Vincenzo CHIOCHIA
//         Created:  Tue Oct 17 17:40:56 CEST 2006
// $Id: SiPixelCondObjForHLTReader.h,v 1.4 2006/11/09 13:20:04 chiochia Exp $
//
//
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
//#include "CondFormats/SiPixelObjForHLTects/interface/SiPixelGainCalibration.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "CondTools/SiPixel/interface/SiPixelGainCalibrationForHLTService.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"

namespace cms{
class SiPixelCondObjForHLTReader : public edm::EDAnalyzer {

public:

  explicit SiPixelCondObjForHLTReader( const edm::ParameterSet& iConfig);

  ~SiPixelCondObjForHLTReader(){};
  virtual void beginJob( const edm::EventSetup& );
  virtual void analyze(const edm::Event& , const edm::EventSetup& );
  virtual void endJob() ;

private:

  edm::ParameterSet conf_;
  edm::ESHandle<TrackerGeometry> tkgeom;
  //edm::ESHandle<SiPixelGainCalibration> SiPixelGainCalibration_;
  SiPixelGainCalibrationForHLTService SiPixelGainCalibrationService_;

  std::map< uint32_t, TH1F* >  _TH1F_Pedestals_m;
  std::map< uint32_t, TH1F* >  _TH1F_Gains_m;
  std::string filename_;
  TFile* fFile;

};
}
#endif
