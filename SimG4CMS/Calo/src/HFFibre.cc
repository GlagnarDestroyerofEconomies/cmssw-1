///////////////////////////////////////////////////////////////////////////////
// File: HFFibre.cc
// Description: Loads the table for attenuation length and calculates it
///////////////////////////////////////////////////////////////////////////////

#include "SimG4CMS/Calo/interface/HFFibre.h"

#include "DetectorDescription/Core/interface/DDFilter.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "DetectorDescription/Core/interface/DDValue.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "CLHEP/Units/SystemOfUnits.h"
#include <iostream>

//#define DebugLog

HFFibre::HFFibre(std::string & name, const DDCompactView & cpv, 
		 edm::ParameterSet const & p) {

  edm::ParameterSet m_HF = p.getParameter<edm::ParameterSet>("HFShower");
  cFibre           = c_light*(m_HF.getParameter<double>("CFibre"));
  
  edm::LogInfo("HFShower") << "HFFibre:: Speed of light in fibre " << cFibre
			   << " m/ns";

  std::string attribute = "Volume"; 
  std::string value     = "HF";
  DDSpecificsFilter filter1;
  DDValue           ddv1(attribute,value,0);
  filter1.setCriteria(ddv1,DDSpecificsFilter::equals);
  DDFilteredView fv1(cpv);
  fv1.addFilter(filter1);
  bool dodet = fv1.firstChild();

  if (dodet) {
    DDsvalues_type sv(fv1.mergedSpecifics());

    // Attenuation length
    nBinAtt      = -1;
    attL         = getDDDArray("attl",sv,nBinAtt);
    edm::LogInfo("HFShower") << "HFFibre: " << nBinAtt << " attL ";
    for (int it=0; it<nBinAtt; it++) 
      edm::LogInfo("HFShower") << "HFFibre: attL[" << it << "] = " 
			       << attL[it]*cm << "(1/cm)";

    // Limits on Lambda
    int nb   = 2;
    std::vector<double>  nvec = getDDDArray("lambLim",sv,nb);
    lambLim[0] = static_cast<int>(nvec[0]);
    lambLim[1] = static_cast<int>(nvec[1]);
    edm::LogInfo("HFShower") << "HFFibre: Limits on lambda " << lambLim[0]
			     << " and " << lambLim[1];

    // Fibre Lengths
    nb       = 0;
    longFL   = getDDDArray("LongFL",sv,nb);
    edm::LogInfo("HFShower") << "HFFibre: " << nb << " Long Fibre Length";
    for (int it=0; it<nb; it++) 
      edm::LogInfo("HFShower") << "HFFibre: longFL[" << it << "] = " 
			       << longFL[it]/cm << " cm";
    nb       = 0;
    shortFL   = getDDDArray("ShortFL",sv,nb);
    edm::LogInfo("HFShower") << "HFFibre: " << nb << " Short Fibre Length";
    for (int it=0; it<nb; it++) 
      edm::LogInfo("HFShower") << "HFFibre: shortFL[" << it << "] = " 
			       << shortFL[it]/cm << " cm";
  } else {
    edm::LogError("HFShower") << "HFFibre: cannot get filtered "
			      << " view for " << attribute << " matching "
			      << name;
    throw cms::Exception("Unknown", "HFFibre")
      << "cannot match " << attribute << " to " << name <<"\n";
  }

  // Now geometry parameters
  attribute = "ReadOutName";
  value     = name;
  DDSpecificsFilter filter2;
  DDValue           ddv2(attribute,value,0);
  filter2.setCriteria(ddv2,DDSpecificsFilter::equals);
  DDFilteredView fv2(cpv);
  fv2.addFilter(filter2);
  dodet     = fv2.firstChild();
  if (dodet) {
    DDsvalues_type sv(fv2.mergedSpecifics());

    //Special Geometry parameters
    int nb    = -1;
    gpar      = getDDDArray("gparHF",sv,nb);
    edm::LogInfo("HFShower") << "HFFibre: " << nb <<" gpar (cm)";
    for (int i=0; i<nb; i++)
      edm::LogInfo("HFShower") << "HFFibre: gpar[" << i << "] = "
			       << gpar[i]/cm << " cm";

    nBinR     = -1;
    radius    = getDDDArray("rTable",sv,nBinR);
    edm::LogInfo("HFShower") << "HFFibre: " << nBinR <<" rTable (cm)";
    for (int i=0; i<nBinR; i++)
      edm::LogInfo("HFShower") << "HFFibre: radius[" << i << "] = "
			       << radius[i]/cm << " cm";
  } else {
    edm::LogError("HFShower") << "HFFibre: cannot get filtered "
			      << " view for " << attribute << " matching "
			      << name;
    throw cms::Exception("Unknown", "HFFibre")
      << "cannot match " << attribute << " to " << name <<"\n";
  }
}

HFFibre::~HFFibre() {}

double HFFibre::attLength(double lambda) {

  int i = int(nBinAtt*(lambda - lambLim[0])/(lambLim[1]-lambLim[0]));

  int j =i;
  if (i >= nBinAtt) 
    j = nBinAtt-1;
  else if (i < 0)
    j = 0;
  double att = attL[j];
#ifdef DebugLog
  LogDebug("HFShower") << "HFFibre::attLength for Lambda " << lambda
		       << " index " << i  << " " << j << " Att. Length " 
		       << att;
#endif
  return att;
}

double HFFibre::tShift(G4ThreeVector point, int depth, int fromEndAbs) {

  double zFibre = 0;
  double hR     = sqrt((point.x())*(point.x())+(point.y())*(point.y()));
  int    ieta   = 0;
  double length = 250*cm;
  if (fromEndAbs < 0) {
    zFibre      = 0.5*gpar[1] - point.z();
  } else {
    for (int i = nBinR-1; i > 0; i--)
      if (hR < radius[i]) ieta = nBinR - i - 1;
    if (depth == 2) {
      if ((int)(shortFL.size()) > ieta) length = shortFL[ieta];
    } else {
      if ((int)(longFL.size())  > ieta) length = longFL[ieta];
    }
    zFibre = length;
    if (fromEndAbs > 0) {
      zFibre   -= gpar[1];
    } else {
      double zz = 0.5*gpar[1] + point.z();
      zFibre   -= zz;
    }
    if (depth == 2) zFibre += gpar[0];
  }
  double time   = zFibre/cFibre;
#ifdef DebugLog
  LogDebug("HFShower") << "HFFibre::tShift for point " << point
		       << " (R = " << hR/cm << " cm, Index = " << ieta 
		       << ", depth = " << depth << ", Fibre Length = " 
		       << length/cm       << " cm, traversed length = " 
		       << zFibre/cm  << " cm) = " << time/ns << " ns";
#endif
  return time;
}

std::vector<double> HFFibre::getDDDArray(const std::string & str, 
					 const DDsvalues_type & sv, 
					 int & nmin) {

#ifdef DebugLog
  LogDebug("HFShower") << "HFFibre:getDDDArray called for " << str 
		       << " with nMin " << nmin;
#endif
  DDValue value(str);
  if (DDfetch(&sv,value)) {
#ifdef DebugLog
    LogDebug("HFShower") << value;
#endif
    const std::vector<double> & fvec = value.doubles();
    int nval = fvec.size();
    if (nmin > 0) {
      if (nval < nmin) {
	edm::LogError("HFShower") << "HFFibre : # of " << str << " bins " 
				  << nval << " < " << nmin << " ==> illegal";
	throw cms::Exception("Unknown", "HFFibre")
	  << "nval < nmin for array " << str <<"\n";
      }
    } else {
      if (nval < 1 && nmin != 0) {
	edm::LogError("HFShower") << "HFFibre : # of " << str << " bins " 
				  << nval << " < 1 ==> illegal (nmin=" 
				  << nmin << ")";
	throw cms::Exception("Unknown", "HFFibre")
	  << "nval < 1 for array " << str <<"\n";
      }
    }
    nmin = nval;
    return fvec;
  } else {
    if (nmin != 0) {
      edm::LogError("HFShower") << "HFFibre : cannot get array " << str;
      throw cms::Exception("Unknown", "HFFibre")
	<< "cannot get array " << str <<"\n";
    } else {
      std::vector<double> fvec;
      return fvec;
    }
  }
}
