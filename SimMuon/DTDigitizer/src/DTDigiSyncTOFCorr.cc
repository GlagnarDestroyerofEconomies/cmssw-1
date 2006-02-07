/*
 *  See header file for a description of this class.
 *
 *  $Date: 2006/02/02 18:24:02 $
 *  $Revision: 1.4 $
 *  \author N. Amapane, R. Bellan - INFN Torino
 */

#include "SimMuon/DTDigitizer/src/DTDigiSyncTOFCorr.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/MuonDetId/interface/DTWireId.h"

#include "Geometry/DTSimAlgo/interface/DTLayer.h"
#include "Geometry/DTSimAlgo/interface/DTChamber.h"

using namespace std;

DTDigiSyncTOFCorr::DTDigiSyncTOFCorr(const edm::ParameterSet& pSet){
  
  theOffset = pSet.getParameter<double>("offset"); //500ns
  corrType = pSet.getParameter<int>("TOFCorrection"); //1
}


DTDigiSyncTOFCorr::~DTDigiSyncTOFCorr(){}


// Delays to be added to digi times during digitization, in ns.
double DTDigiSyncTOFCorr::digitizerOffset(const DTWireId * id, const DTLayer* layer) const {

  double offset = theOffset;
  const double cSpeed = 29.9792458; // cm/ns

  if (corrType==1) {
    double flightL = layer->chamber()->surface().position().mag();
    offset -= flightL/cSpeed;
  } else if (corrType==2) {
    // Subtraction of assumed TOF, per WIRE 

    // Position of the wire in the Layer's reference frame
    float localXPos = layer->specificTopology().wirePosition(id->wire());
    LocalPoint localPos(localXPos,0,0); 

    // Distance of the wire to the CMS's I.P.
    double flightL = layer->surface().toGlobal(localPos).mag();

    offset -= flightL/cSpeed;

  } else if (corrType!=0){
    cout << "ERROR: SimMuon:DTDigitizer:DTDigiSyncTOFCorr:TOFCorrection = " << corrType
	 << "is not defined " << endl; 
  }
  return offset;
}


// Offset to obtain "raw" TDCs for the L1 emulator from digis.
double DTDigiSyncTOFCorr::emulatorOffset(const DTWireId * id) const {
  return theOffset;
}
