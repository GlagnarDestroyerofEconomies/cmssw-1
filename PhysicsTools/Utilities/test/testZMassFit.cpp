#include "PhysicsTools/Utilities/interface/BreitWigner.h"
#include "PhysicsTools/Utilities/interface/HistoChiSquare.h"
#include "PhysicsTools/Utilities/interface/RootMinuitCommands.h"
#include "PhysicsTools/Utilities/interface/RootMinuit.h"
#include "PhysicsTools/Utilities/interface/Parameter.h"
#include "PhysicsTools/Utilities/interface/Constant.h"
#include "PhysicsTools/Utilities/interface/rootTf1.h"
#include "PhysicsTools/Utilities/interface/rootPlot.h"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TROOT.h"
#include <boost/shared_ptr.hpp>
#include <iostream>
#include "PhysicsTools/Utilities/interface/Operations.h"
//using namespace std;
//using namespace boost;

int main() { 
  gROOT->SetStyle("Plain");
  typedef funct::Product<funct::Constant, funct::BreitWigner>::type FitFunction;
  typedef fit::HistoChiSquare<FitFunction> ChiSquared;
  try {
    fit::RootMinuitCommands<ChiSquared> commands("PhysicsTools/Utilities/test/testZMassFit.txt");
    
    const char * kYield = "Yield";
    const char * kMass = "Mass";
    const char * kGamma = "Gamma";
    
    funct::Parameter yield(kYield, commands.par(kYield));
    funct::Parameter mass(kMass, commands.par(kMass));
    funct::Parameter gamma(kGamma, commands.par(kGamma));
    funct::BreitWigner bw(mass, gamma);
    funct::Constant c(yield);
    
    FitFunction f = c * bw;
    TF1 startFun = root::tf1("startFun", f, 0, 200, yield, mass, gamma);
    TH1D histo("histo", "Z mass (GeV/c)", 200, 0, 200);
    histo.FillRandom("startFun", yield);
    TCanvas canvas;
    startFun.Draw();
    canvas.SaveAs("breitWigned.eps");
    histo.Draw();
    canvas.SaveAs("breitWignedHisto.eps");
    startFun.Draw("same");
    canvas.SaveAs("breitWignedHistoFun.eps");
    histo.Draw("e");
    startFun.Draw("same");
    
    ChiSquared chi2(f, &histo, 0, 200);
    int fullBins = chi2.degreesOfFreedom();
    std::cout << "N. deg. of freedom: " << fullBins << std::endl;
    fit::RootMinuit<ChiSquared> minuit(chi2, true);
    commands.add(minuit, yield);
    commands.add(minuit, mass);
    commands.add(minuit, gamma);
    commands.run(minuit);
    root::plot<FitFunction>("breitWignedHistoFunFit.eps", histo, f, 0, 200, yield, mass, gamma);
    /*TF1 fun = root::tf1("fun", f, 80, 120, yield, mass, gamma);
    fun.SetParNames(yield.name().c_str(), mass.name().c_str(), gamma.name().c_str());
    fun.SetLineColor(kRed);
    fun.Draw("same");
    canvas.SaveAs("breitWignedHistoFunFit.eps");*/
  } catch(std::exception & err){
    std::cerr << "Exception caught:\n" << err.what() << std::endl;
    return 1;
  }
  
  return 0;
}
