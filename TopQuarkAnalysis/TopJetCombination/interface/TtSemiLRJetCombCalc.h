//
// Author:  Jan Heyninck
// Created: Tue Apr  3 17:33:23 PDT 2007
//
// $Id: TtSemiLRJetCombCalc.h,v 1.2 2007/05/22 16:43:36 heyninck Exp $
//

#ifndef TtSemiLRJetCombCalc_h
#define TtSemiLRJetCombCalc_h

/**
  \class    TtSemiLRJetCombCalc TtSemiLRJetCombCalc.h "TopQuarkAnalysis/TopLeptonSelection/interface/TtSemiLRJetCombCalc.h"
  \brief    Class to calculate the jet combination LR value and purity from a root-file with fit functions

  \author   Jan Heyninck
  \version  $Id: TtSemiLRJetCombCalc.h,v 1.2 2007/05/22 16:43:36 heyninck Exp $
*/


#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "AnalysisDataFormats/TopObjects/interface/TtSemiEvtSolution.h"
#include "TopQuarkAnalysis/TopTools/interface/LRHelpFunctions.h"

#include "TF1.h"
#include "TH1.h"
#include "TFile.h"
#include "TKey.h"
#include "TString.h"


using namespace std;


class TtSemiLRJetCombCalc {

  public:
    TtSemiLRJetCombCalc();
    TtSemiLRJetCombCalc(TString);
    ~TtSemiLRJetCombCalc();	

    void  operator()(TtSemiEvtSolution&);

  private:
    LRHelpFunctions * myLR;

};

#endif
