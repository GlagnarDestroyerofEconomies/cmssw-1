
#ifndef HepPDTAAnalyzer_H
#define HepPDTAAnalyzer_H

// -*- C++ -*-
//
// Package:    HepPDTAnalyzer
// Class:      HepPDTAnalyzer
// 
/**\class HepPDTAnalyzer HepPDTAnalyzer.cc test/HepPDTAnalyzer/src/HepPDTAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Filip Moortgat
//         Created:  Wed Jul 19 14:41:13 CEST 2006
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <string>
#include <vector>


//
// class decleration
//

#include <set>

namespace edm {
  class ParameterSet;
}

class HepPDTAnalyzer : public edm::EDAnalyzer {
   public:
      explicit HepPDTAnalyzer(const edm::ParameterSet&);
      ~HepPDTAnalyzer();


      virtual void analyze(const edm::Event&, const edm::EventSetup&);
   private:
      // ----------member data ---------------------------

    std::string pName;

};

#endif
