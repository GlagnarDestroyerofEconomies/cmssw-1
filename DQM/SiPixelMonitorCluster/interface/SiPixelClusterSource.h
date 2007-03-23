#ifndef SiPixelMonitorCluster_SiPixelClusterSource_h
#define SiPixelMonitorCluster_SiPixelClusterSource_h
// -*- C++ -*-
//
// Package:     SiPixelMonitorCluster
// Class  :     SiPixelClusterSource
// 
/**

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  Vincenzo Chiochia
//         Created:  
// $Id: SiPixelClusterSource.h,v 1.2 2006/07/13 09:48:48 chiochia Exp $
//

#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"

#include "DQM/SiPixelMonitorCluster/interface/SiPixelClusterModule.h"

#include "DataFormats/Common/interface/DetSetVector.h"
//#include "DataFormats/SiPixelCluster/interface/PixelCluster.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/Common/interface/EDProduct.h"


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <boost/cstdint.hpp>

 class SiPixelClusterSource : public edm::EDAnalyzer {
    public:
       explicit SiPixelClusterSource(const edm::ParameterSet& conf);
       ~SiPixelClusterSource();

       typedef edm::DetSet<SiPixelCluster>::const_iterator    ClusterIterator;
       
       virtual void analyze(const edm::Event&, const edm::EventSetup&);
       virtual void beginJob(edm::EventSetup const&) ;
       virtual void endJob() ;

       virtual void buildStructure(edm::EventSetup const&);
       virtual void bookMEs();

    private:
       edm::ParameterSet conf_;
       int eventNo;
       DaqMonitorBEInterface* theDMBE;
       std::map<uint32_t,SiPixelClusterModule*> thePixelStructure;
 };

#endif
