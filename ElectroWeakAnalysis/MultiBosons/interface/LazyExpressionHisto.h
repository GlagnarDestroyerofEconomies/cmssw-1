#ifndef ElectroWeakAnalysis_MultiBosons_LazyExpressionHisto_h
#define ElectroWeakAnalysis_MultiBosons_LazyExpressionHisto_h
// -*- C++ -*-
//
// Package:     MultiBosons
// Class  :     LazyExpressionHisto
// 
/**\class LazyExpressionHisto LazyExpressionHisto.h ElectroWeakAnalysis/MultiBosons/interface/LazyExpressionHisto.h

 Description: Histogram tool using expressions, lazy evaluation version of the tool in CommonTools/UtilAlgos

 Usage:
    <usage>

*/
//
// Original Author: Benedikt HEGNER
//         Created:  Fri Jun  1 14:35:22 CEST 2007
// $Id: LazyExpressionHisto.h,v 1.2 2009/07/09 10:52:01 gpetrucc Exp $
//

// system include files

// user include files
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH1.h"

template<typename T>
class LazyExpressionHisto {
public:
  LazyExpressionHisto(const edm::ParameterSet& iConfig);
  ~LazyExpressionHisto();
  
  void initialize(edm::Service<TFileService>& fs);
/** Plot the quantity for the specified element and index.
    Returns true if the quantity has been plotted, false otherwise.
    A return value of "false" means "please don't send any more elements".
    The default "i = 0" is to keep backwards compatibility with usages outside
    HistoAnalyzer */
  bool fill(const T& element, double weight=1.0, uint32_t i=0);  
  
private:
  double min, max;
  int nbins;
  std::string name, description;
  uint32_t nhistos;
  bool     separatePlots;
  TH1F ** hist;
  StringObjectFunction<T, true> function;
};

template<typename T>
LazyExpressionHisto<T>::LazyExpressionHisto(const edm::ParameterSet& iConfig):
  min(iConfig.template getUntrackedParameter<double>("min")),
  max(iConfig.template getUntrackedParameter<double>("max")),
  nbins(iConfig.template getUntrackedParameter<int>("nbins")),
  name(iConfig.template getUntrackedParameter<std::string>("name")),
  description(iConfig.template getUntrackedParameter<std::string>("description")),
  function(iConfig.template getUntrackedParameter<std::string>("plotquantity"), 
           iConfig.template getUntrackedParameter<bool>("lazyParsing", false)) {
  int32_t itemsToPlot = iConfig.template getUntrackedParameter<int32_t>("itemsToPlot", -1);
  if (itemsToPlot <= 0) {
      nhistos = 1; separatePlots = false;
  } else {
      nhistos = itemsToPlot; separatePlots = true;
  }
}

template<typename T>
LazyExpressionHisto<T>::~LazyExpressionHisto() {
}

template<typename T>
void LazyExpressionHisto<T>::initialize(edm::Service<TFileService>& fs) 
{
   hist = new TH1F*[nhistos];
   char buff[1024],baff[1024];
   if (separatePlots) {
       for (uint32_t i = 0; i < nhistos; i++) {
               if (strstr(name.c_str(), "%d") != 0) {
                       snprintf(buff, 1024, name.c_str(), i+1);
               } else {
                       snprintf(buff, 1024, "%s [#%d]", name.c_str(), i+1);
               }
               if (strstr(description.c_str(), "%d") != 0) {
                       snprintf(baff, 1024, description.c_str(), i+1);
               } else {
                       snprintf(baff, 1024, "%s [#%d]", description.c_str(), i+1);
               }
               hist[i] = fs->template make<TH1F>(buff,baff,nbins,min,max);
       }
    } else {
       hist[0] = fs->template make<TH1F>(name.c_str(),description.c_str(),nbins,min,max);
    }
}

template<typename T>
bool LazyExpressionHisto<T>::fill(const T& element, double weight, uint32_t i) 
{
  if (!separatePlots) hist[0]->Fill( function(element), weight );
  else if (i < nhistos)  hist[i]->Fill( function(element), weight );
  else return false;
  return true;
}

#endif
